#declare the variable
CC=g++
CFLAGS= -c -Wall
#LIBS          = -L/usr/lib/arm-linux-gnueabihf -L../raspberrypi_libs/leptonSDKEmb32PUB/Debug
#-L../leptonSDKEmb32PUB/Debug
all: main

main: main.o SPI.o Lepton_I2C.o
		$(CC) main.o SPI.o Lepton_I2C.o -o main leptonSDKEmb32PUB/Debug/LEPTON_SDK.o leptonSDKEmb32PUB/Debug/LEPTON_OEM.o leptonSDKEmb32PUB/Debug/LEPTON_SYS.o leptonSDKEmb32PUB/Debug/LEPTON_I2C_Protocol.o leptonSDKEmb32PUB/Debug/LEPTON_I2C_Service.o leptonSDKEmb32PUB/Debug/crc16fast.o leptonSDKEmb32PUB/Debug/raspi_I2C.o -lwiringPi

main.o: main.cpp
		$(CC) $(CFLAGS) main.cpp 
		
SPI.o: SPI.cpp
		$(CC) $(CFLAGS) SPI.cpp
		
Lepton_I2C.o: Lepton_I2C.cpp
		$(CC) $(CFLAGS) Lepton_I2C.cpp 
clean: 
		rm -rf *o main
