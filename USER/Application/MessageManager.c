#include "MessageManager.h"

messageFrame_t *messageInput;
static uint8_t tempBuffer[TEMPBUFF_LENGTH] = {0};
static uint16_t	messageLength;
static uint8_t	messageHeader;

static bool	updateLasted = false;
static bool	checkNow = false;
static bool	restartMessage = true;
static bool messageUpdate = false;

void MessageProcess_Init(void){
    uartReceive_Init();
    messageInput = (messageFrame_t *)(tempBuffer);
}

void MessageProcess_Process(void){
    uint16_t i;

    if(!checkNow){
        if(guartRingBuffer.count > 0){
            if(updateLasted == false){
                if(restartMessage == true){
                    if(guartRingBuffer.count >= MESSAGE_HEADLENGTH){
                        for(i = 0; i < MESSAGE_HEADLENGTH; i++){
                            ringBuffer_Pop(&guartRingBuffer, &tempBuffer[i]);
                        }
                        messageLength = (messageInput->Length[0]<<8) | messageInput->Length[1];
                        messageHeader = messageInput->Header;

                        messageUpdate = true;
                        restartMessage = false;
                    }
                    
                }
                else{
                    ringBuffer_Pop(&guartRingBuffer, &tempBuffer[MESSAGE_HEADLENGTH - 1]);
                    messageLength = (messageInput->Length[0]<<8) | messageInput->Length[1];
                    messageHeader = messageInput->Header;

                    messageUpdate = true;
                }


                if(messageUpdate == true){
                    messageUpdate = false;

                    if(messageInput->Start[0] == 0xAA && messageInput->Start[1] == 0x55){
                        if(messageHeader <= 0x07){
                            if(guartRingBuffer.count >= messageLength - MESSAGE_OFFSET){
                                for(i = 0; i < messageLength - MESSAGE_OFFSET ; i++){
                                    ringBuffer_Pop(&guartRingBuffer, &tempBuffer[MESSAGE_HEADLENGTH + i]);
                                }
                                updateLasted = false;
                                checkNow = true;
                                restartMessage = true;
                            }
                            else{
                                updateLasted = true;
                                restartMessage = false;
                                checkNow = false;
                            }
                        }                            
                    }
                    else{
                        for (i = 0; i < MESSAGE_HEADLENGTH - 2; i++){
                            tempBuffer[i] = tempBuffer[i+1];
                        }

                        restartMessage = false;
                        updateLasted = false;
                    }
                }
            }
            else{
                if(guartRingBuffer.count >= (messageLength - MESSAGE_OFFSET)){
                    for(i = 0; i < (messageLength - MESSAGE_OFFSET); i++){
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

bool MessageProcess_hasMessage(void){
    return checkNow;
}


uint16_t MessageProcess_copyMessage (uint8_t *Message, uint8_t *header){
    if(!checkNow){
        return 0;
    }

    *header = messageInput->Header;
    memcpy(Message, messageInput->Message, messageLength);

    /*TODO: Need process CRC before return. Now it is skiped*/

    return messageLength;
}
