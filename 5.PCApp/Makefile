CC = gcc
CFLAGS = -Wall
LDFLAGS =
OBJFILES = FileManager.o COMPort.o OTA.o main.o
TARGET = run.exe

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	del $(OBJFILES) $(TARGET)

cleanTmp:
	del $(OBJFILES)