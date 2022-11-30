## STEP GUIDE
1. Copy the binary file to same directory of the Application.
1. Using terminal. Determine the COMPORT, name of binary file.
1. Type command follow this guide
    ```shell
    run.exe <COMPORT> <File name> <MajorVerion> <MinorVersion>
    ```
    > Example: run.exe COM3 Program.bin 1 10

## Install GCC for Windows
Follow [this guide](https://www.scaler.com/topics/c/c-compiler-for-windows/) to install gcc for Windows and build program.

Build the program using **make**.
> make