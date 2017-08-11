#include "SPI.h"
#include <string>
#include <ctime>
#include <stdint.h>
#include <stdio.h>
#include "Lepton_I2C.h"
#include <iostream>
//#include <string.h>
#include <wiringPi.h>

#define ANSI_COLOR_RED 		"\x1b[31m"
#define ANSI_COLOR_GREEN  	"\x1b[32m"
#define ANSI_COLOR_RESET  	"\x1b[0m"
#define PACKET_SIZE 164
#define PACKET_SIZE_UINT16 (PACKET_SIZE/2)
#define PACKETS_PER_FRAME 60
#define FRAME_SIZE_UINT16 (PACKET_SIZE_UINT16*PACKETS_PER_FRAME)
#define FPS 106;


uint8_t result[PACKET_SIZE*PACKETS_PER_FRAME];
uint8_t result1[PACKET_SIZE*PACKETS_PER_FRAME];
uint8_t result2[PACKET_SIZE*PACKETS_PER_FRAME];
uint8_t result3[PACKET_SIZE*PACKETS_PER_FRAME];
uint8_t result4[PACKET_SIZE*PACKETS_PER_FRAME];
uint16_t *frameBuffer;
uint8_t mainBuffer[PACKET_SIZE*PACKETS_PER_FRAME*4];
uint16_t *mainframeBuffer; 

//static int pixel_value[19680];
//static int object_detection_counter;
static	float main_diff1;
char* STATUS; 
int main()
{
	//int c=1;
	//int b=0;
	int m;
	//int counter = 0;
	//int display_flag = 0;
	int error_counter = 0;
	int reboot_counter = 0;
	//int on_wait_counter = 0;
	//int off_wait_counter = 0;
	/*Segment received flags */
	int segmentReceived_flag1 = 0;
	int segmentReceived_flag2 = 0;
	int segmentReceived_flag3 = 0;
	int segmentReceived_flag4 = 0;
	
	
	//strcpy(STATUS,"CLEAR");
	STATUS = "CLEAR";
	int segmentId = 0;
	//int frame_counter = 0;
	
	int pin = 7;
	int pin_flag = 0;
	wiringPiSetup();
	pinMode(pin, OUTPUT);
	
	SpiOpenPort(0);	

	while(true)
		{
			int resets = 0;
			for(int j=0;j<PACKETS_PER_FRAME;j++) 
				{
					//if it's a drop packet, reset j to 0, set to -1 so he'll be at 0 again loop
					read(spi_cs0_fd, result+sizeof(uint8_t)*PACKET_SIZE*j, sizeof(uint8_t)*PACKET_SIZE);
					
					//printf("%d\n",sizeof(result));	
					
				
					int packetNumber = result[j*PACKET_SIZE+1];  //1st byte of 0-163 byte packet is a packet number ,result buffer has 60 packets
					if(packetNumber != j) 
						{
							j = -1;
							resets += 1;
							usleep(10);
							if(resets == 2750) 
								{
									error_counter++;
									//printf("error_counter = %d\n",error_counter);
									//printf("resets = %d\n",resets);
									resets = 0;
									//printf("Lost Camera Connection\n");
									SpiClosePort(0);
									usleep(75000);
									
									SpiOpenPort(0);
								//	printf("Reset Camera Connection\n");
								}
								
								
							//printf("error_counter = %d\n",error_counter);
							
							if (error_counter > 10){
								reboot_counter++;
								printf("reboot_counter = %d\n",reboot_counter);
								lepton_perform_reboot();
								error_counter = 0;
								usleep(3000000);
							}
								
						}
						
				}
		
		
			//printf("error_counter = %d\n",error_counter);
		
			if (error_counter > 100){
				reboot_counter++;
				printf("reboot_counter = %d\n",reboot_counter);
				lepton_perform_reboot();
				error_counter = 0;
				usleep(3000000);
			}
			
			
			
			
			/*
				int counter = 0;
				for(int i=0; i<9840 ; i++)
					{
						printf("%02x",result[i]);
						counter++;
						if (counter > 163)
							{
								printf("\n");
								counter = 0;
								
							}
						//printf("\n");
					}
				printf("\n");
				printf("\n");
				
				
			*/
				
				segmentId = (result[3280]/16);
		

	/************************************************* Check segmentID and organize segments in mainframe_buffer  ********************************************************************/

			if (segmentId != 0)
				{
				
					if (segmentId == 1 )
						{
							m = 0;
							
							//result1 = (uint8_t *)result;
							for (int n=0;n<9840;n++)
								{
									//result1[n] = result[n];
									mainBuffer[m] = result[n];
									m++;
								}
						//	printf("result1 Segment ID = %d\n",(result[3280]/16));
							segmentReceived_flag1 = 1;
							segmentReceived_flag2 = 0;
							segmentReceived_flag3 = 0;
							segmentReceived_flag4 = 0;
						}
					if (segmentId == 2 && segmentReceived_flag1 == 1)
						{
							//result2 = (uint8_t *)result;
							for (int n=0;n<9840;n++)
								{
									//result2[n] = result[n];
									mainBuffer[m] = result[n];
									m++;
								}
							//printf("result2 Segment ID = %d\n",(result[3280]/16));
							segmentReceived_flag2 = 1;
							segmentReceived_flag1 = 0;
					
						}
					if (segmentId == 3 && segmentReceived_flag2 == 1)
						{
							//result3 = (uint8_t *)result;
							for (int n=0;n<9840;n++)
								{
									//result3[n] = result[n];
									mainBuffer[m] = result[n];
									m++;	
								}
							//printf("result3 Segment ID = %d\n",(result[3280]/16));
							segmentReceived_flag3 = 1;
							segmentReceived_flag2 = 0;
						}
					if (segmentId == 4 && segmentReceived_flag3 == 1)
						{
							//result4 = (uint8_t *)result;
							for (int n=0;n<9840;n++)
								{
									//result4[n] = result[n];
									mainBuffer[m] = result[n];
									m++;	
								}
							//printf("result4 Segment ID = %d\n",(result[3280]/16));
							segmentReceived_flag4 = 1;
							segmentReceived_flag3 = 0;
					
							//fprintf(pFile,"segmentReceived_flag4 = %d\n",segmentReceived_flag4);
						}
			
			
				
					if(segmentReceived_flag4 ==1)
						{
					
							segmentReceived_flag4 = 0;
					
/**************************************************MAIN FRAME BUFFER *********************************************************/
							mainframeBuffer = (uint16_t *)mainBuffer;
					
							int main_row, main_column;
							uint16_t main_value;
							uint16_t main_minValue = 65535;
							uint16_t main_maxValue = 0;

					
							for(int i=0;i<19680;i++) 
								{
									//skip the first 2 uint16_t's of every packet, they're 4 header bytes
						
									if(i % PACKET_SIZE_UINT16 < 2) 
										{
											//printf("\n i =%d and i % PACKET_SIZE_UINT16  %d\n",i,i % PACKET_SIZE_UINT16);
											continue;
										}
						
						
									//flip the MSB and LSB at the last second
									int temp = mainBuffer[i*2];
									mainBuffer[i*2] = mainBuffer[i*2+1];
									mainBuffer[i*2+1] = temp;
									
									main_value = mainframeBuffer[i];
									if(main_value > main_maxValue) 
										{
											main_maxValue = main_value;
						
										}
									if(main_value < main_minValue && main_value > 0 ) 
										{
											main_minValue = main_value;
						
										}
									main_column = i % PACKET_SIZE_UINT16 - 2;
									main_row = i / PACKET_SIZE_UINT16 ;
						
							
								}
					
							//printf("[row,coulm]=[%d,%d]",main_column,main_row);
							float main_diff = main_maxValue - main_minValue;
						
							if(main_diff < 10000)
								{
									main_diff1 = main_diff;
								}
							else
								{
									main_diff = main_diff1;
								}
					
					
							float main_scale = 255/main_diff;
					
							if(pin_flag == 0 && main_diff > 200)
								{
			
									digitalWrite(pin,HIGH) ;
									printf(ANSI_COLOR_RED);
									pin_flag = 1;
									STATUS = "DETECTED";
					
								}
							if(pin_flag == 1 && main_diff < 200)
								{
					
									digitalWrite(pin, LOW) ;
								
									printf(ANSI_COLOR_GREEN);
									pin_flag = 0;
									STATUS = "CLEAR";
								}
							printf("[ObjectPixelValue = %5.1f] STATUS = %s\n",main_diff,STATUS);
							//printf("%5.1f\n",main_diff);
							//usleep(25000);
						}
				}		
		}
	
	return 0;
}
