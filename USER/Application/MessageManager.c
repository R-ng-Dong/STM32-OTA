#include "MessageManager.h"

messageFrame_t *messageInput;
static uint8_t tempBuffer[TEMPBUFF_LENGTH] = {0};
static uint16_t	messageLength;
static uint8_t	messageHeader;

static bool	updateLasted = false;
static bool	checkNow = false;
static bool	restartMessage = true;
static bool messageUpdate = false;


/**
 * @brief Initialize the Message Processing of OTA
 * 
 */
void MessageProcess_Init(void){
    uartReceive_Init();
    messageInput = (messageFrame_t *)(tempBuffer);
}

/**
 * @brief Process the message for OTA
 * 
 */
void MessageProcess_Process(void){
    uint16_t i;

    if(!checkNow){
        if(guartRingBuffer.count > 0){
			mmuDebug("Has new byte!\n");
            if(updateLasted == false){
				//mmuDebug("Not lasted update!\n");
                if(restartMessage == true){
					//mmuDebug("Restart update!\n");
                    if(guartRingBuffer.count >= MESSAGE_HEADLENGTH){
						
						mmuDebug("Write 4 bytes data:");

                        for(i = 0; i < MESSAGE_HEADLENGTH; i++){
                            ringBuffer_Pop(&guartRingBuffer, &tempBuffer[i]);
							//printf("0x%2x-", tempBuffer[i]);
                        }
#if messageMU_DEBUG
						mmuDebug("\n");
#endif
						
						updateLasted = false;
                        messageUpdate = true;
                        restartMessage = false;
                    }
                    
                }
                else{
					mmuDebug("No restart update!\n");
                    ringBuffer_Pop(&guartRingBuffer, &tempBuffer[MESSAGE_HEADLENGTH - 1]);

                    messageUpdate = true;
                }


                if(messageUpdate == true){
                    messageUpdate = false;
					
					messageLength = (messageInput->Length[0]<<8) | messageInput->Length[1];
					mmuDebug("Frame data:");
#if messageMU_DEBUG
					for (i = 0; i < MESSAGE_HEADLENGTH; i++){
						printf("0x%x-", tempBuffer[i]);
					}
					printf("\n");
#endif
					
					mmuDebug("Check content!\n");
					mmuDebug("Header:0x%2x-0x%2x\n",messageInput->Start[0], messageInput->Start[1]);
					mmuDebug("Length:%d\n", messageLength);
                    if(messageInput->Start[0] == 0xAA && messageInput->Start[1] == 0x55){
						mmuDebug("Header is OK\n");
						if(guartRingBuffer.count >= messageLength){
							for(i = 0; i < messageLength ; i++){
								ringBuffer_Pop(&guartRingBuffer, &tempBuffer[MESSAGE_HEADLENGTH + i]);
							}
							mmuDebug("new Frame!\n");
							updateLasted = false;
							restartMessage = true;
							checkNow = true;
						}
						else{
							updateLasted = true;
							restartMessage = false;
							checkNow = false;
						}                   
                    }
                    else{
						mmuDebug("Header is not OK. Moving data\n");
                        for (i = 0; i < MESSAGE_HEADLENGTH - 1; i++){
                            tempBuffer[i] = tempBuffer[i+1];
                        }

                        restartMessage = false;
                        updateLasted = false;
                    }
                }
            }
            else{
				mmuDebug("Lasted update!\n");
				mmuDebug("length Data:%d\n", guartRingBuffer.count);
				mmuDebug("Header:0x%2x-0x%2x\n",messageInput->Start[0], messageInput->Start[1]);
                if(guartRingBuffer.count >= (messageLength)){
                    for(i = 0; i < (messageLength); i++){
                        ringBuffer_Pop(&guartRingBuffer, &tempBuffer[MESSAGE_HEADLENGTH + i]);
                    }

                    updateLasted = false;
                    checkNow = true;
                    restartMessage = true;
                }
            }
        }
    }    
}
/**
 * @brief Check if has new command from UART
 * 
 * @return true has new command
 * @return false 
 */
bool MessageProcess_hasMessage(void){
    return checkNow;
}

/**
 * @brief Copy message from this process to other for processing
 * 
 * @param Message buffer contains the destination message
 * @param header value of the header for processing
 * @return uint16_t length of the buffer
 */
uint16_t MessageProcess_copyMessage (uint8_t *Message, uint8_t *header){
    if(!checkNow){
        return 0;
    }

	checkNow = 0;
    *header = messageInput->Header;
    memcpy(Message, messageInput->Message, messageLength);

    /*TODO: Need process CRC before return. Now it is skiped*/

    return messageLength;
}
