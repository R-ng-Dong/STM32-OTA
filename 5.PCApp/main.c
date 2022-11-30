#include "FileManager.h"
#include "COMPort.h"



int main(int argc, char  **argv )
{
    if(argc < 2){
        printf("Provide COM arguments!\n");
        return -1;
    }

    comPort_Open(*(argv+1));

    uint8_t     bufferTemp[MAX_FRAME_SIZE];
    uint16_t    lengthRead;
    uint32_t    sizeBinFile;
    uint16_t    countRead = 0;

    fileManager_OpenFile("test.bin");
    sizeBinFile = fileManager_getSize();
    printf("Get size:%d\n", sizeBinFile);
    fileManager_gotoStart();

    lengthRead = MAX_FRAME_SIZE;
    while (lengthRead == MAX_FRAME_SIZE){
        lengthRead = fileManager_getData(bufferTemp, MAX_FRAME_SIZE);
        printf("\n-----------\nRead:%d bytes\n", lengthRead);

        // for (uint16_t i = 0; i < lengthRead; i++){
        //     if(i % 16 == 0){
        //         printf("\n");
        //     }
        //     printf("%2x ", bufferTemp[i]);
        // }
    }
    


    fileManager_CloseFile();

    return 0;
}