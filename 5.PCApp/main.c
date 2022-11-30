#include "FileManager.h"
#include "COMPort.h"



int main(int argc, char  **argv )
{
    uint16_t major, minor;

    if(argc < 5){
        printf("Provide COM arguments!\n");
        return -1;
    }
    major = atoi(*(argv+3));
    minor = atoi(*(argv+4));
    printf("Version: %d.%d\n", major, minor);

    if(comPort_Open(*(argv+1)) != 0){
        return -1;
    }
    ota_sendOTAFile(*(argv+2));
    
    fileManager_CloseFile();
    comPort_Close();

    return 0;
}