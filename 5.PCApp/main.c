#include "FileManager.h"
#include "COMPort.h"



int main(int argc, char  **argv )
{
    if(argc < 2){
        printf("Provide COM arguments!\n");
        return -1;
    }

    comPort_Open(*(argv+1));
    ota_sendOTAFile(*(argv+2));
    
    fileManager_CloseFile();
    comPort_Close();

    return 0;
}