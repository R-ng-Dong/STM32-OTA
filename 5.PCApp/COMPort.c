#include "COMPort.h"


static HANDLE hComm;
static char comPortID[20] = "\\\\.\\";


void comPort_Open(const char *comID){
    BOOL   Status; // Status
    DCB dcbSerialParams = { 0 };

    strcat(comPortID, comID);
    printf("comPort:%s\n", comPortID);

    hComm = CreateFileA(comPortID,                //port name
            GENERIC_READ | GENERIC_WRITE, //Read/Write
            0,                            // No Sharing
            NULL,                         // No Security
            OPEN_EXISTING,// Open existing port only
            0,            // Non Overlapped I/O
            NULL); 
    
    if (hComm == INVALID_HANDLE_VALUE)
        printf("Error in opening serial port\n");
    else
        printf("Opening serial port successful\n");

    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    Status = GetCommState(hComm, &dcbSerialParams); //retreives  the current settings
    if (Status == FALSE)
    {
        printf("\nError to Get the Com state\n\n");
    }

    dcbSerialParams.BaudRate = CBR_115200;      // Setting BaudRate = 9600
    dcbSerialParams.ByteSize = 8;             // Setting ByteSize = 8
    dcbSerialParams.StopBits = ONESTOPBIT;    // Setting StopBits = 1
    dcbSerialParams.Parity   = NOPARITY;    // Setting Parity = None 

    Status = SetCommState(hComm, &dcbSerialParams);

    COMMTIMEOUTS timeouts = { 0 };

    timeouts.ReadIntervalTimeout         = 50;
    timeouts.ReadTotalTimeoutConstant    = 50;
    timeouts.ReadTotalTimeoutMultiplier  = 10;
    timeouts.WriteTotalTimeoutConstant   = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (SetCommTimeouts(hComm, &timeouts) == FALSE)
        printf("\n   Error! in Setting Time Outs");
    else
        printf("\n\n Setting Serial Port Timeouts Successfull");


    char   lpBuffer[] = "A";		       // lpBuffer should be  char or byte array, otherwise write wil fail
    DWORD  dNoOFBytestoWrite;              // No of bytes to write into the port
    DWORD  dNoOfBytesWritten = 0;          // No of bytes written to the port
    
    dNoOFBytestoWrite = sizeof(lpBuffer); // Calculating the no of bytes to write into the port

    Status = WriteFile(hComm,               // Handle to the Serialport
                        lpBuffer,            // Data to be written to the port 
                        dNoOFBytestoWrite,   // No of bytes to write into the port
                        &dNoOfBytesWritten,  // No of bytes written to the port
                        NULL);

    if (Status == TRUE)
        printf("\n\n    %s - Written to %s", lpBuffer, comPortID);
    else
        printf("\n\n   Error %d in Writing to Serial Port",GetLastError());
}

void comPort_Send (const uint8_t *dataSend, uint32_t length){
    if(length % 4 != 0){
        printf("Wrong length of Data!\n");
        return;
    }

    for (uint32_t i= 0; i < length; i++){

    }
}

void comPort_Close (void){
    CloseHandle(hComm);
}



