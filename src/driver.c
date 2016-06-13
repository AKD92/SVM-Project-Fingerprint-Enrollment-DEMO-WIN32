

#include <stdio.h>
#include <stdlib.h>
#include "serial_win.h"


#define CMD_ENROLL						0xBB
#define CMD_GETENROLLED					0x55
#define CMD_SETTEMPLATE					0xBA
#define CMD_SCANVOTER					0xAB
#define CMD_IDENTIFY					0x77
#define RESP_OK							0xB5
#define RESP_ERROR						0x5B





int main(void) {
	
	char strPort[20];
	unsigned int baudrate;
	unsigned char bufTemplate[498];
	char strFile[50];
	unsigned int res;
	unsigned char byteRcv, byteCmd;
	int i;
	FILE *fpTemplate;
	
	printf("Enter COM Port Name and BaudRate: ");
	scanf("%s %u", strPort, &baudrate);
	res = serial_begin(strPort, baudrate);
	if (res != 0) {
		printf("Error connecting to %s\n", strPort);
		return 0;
	}
	else {
		printf("Connection established to: %s\n", strPort);
	}
	
	REPEAT:
	
	byteCmd = '1';
	serial_write(&byteCmd, 1);
	serial_read(&byteRcv, 1);
	if (byteRcv == RESP_OK) {
		printf("Press and hold your finger...\n");
	}
	else {
		printf("Enrollment Operation cancelled\n");
		serial_end();
		return 0;
	}
	
	for (i = 0; i < 3; i++) {
		serial_read(&byteRcv, 1);
		if (byteRcv == RESP_ERROR) {
			printf("Enrollment Operation Cancelled (%u)\n", i);
			serial_end();
			return 0;
		}
		else {
			printf("Enroll %u OK...", i);
		}
	}
	printf("\n");
	
	printf("Reading Template from MCU RAM... ");
	
	byteCmd = '2';
	serial_write(&byteCmd, 1);
	serial_read(bufTemplate, 498);
	
	printf("Template Filename: ");
	scanf("%s", strFile);
	
	fpTemplate = fopen(strFile, "wb");
	if (fpTemplate == 0) {
		printf("Error on opening file: %s\n", strFile);
		goto END;
	}
	for (i = 0; i < 498; i++) {
		fputc(*(bufTemplate + i), fpTemplate);
	}
	fclose(fpTemplate);
	printf("Template saved to %s\n", strFile);
	
	END:
	printf("Another finger? (y/n) ");
	
	REPEAT_2:
	i = fgetc(stdin);
	if (i != 'y' && i != 'n')
		goto REPEAT_2;
		
	if (i == 'y')
		goto REPEAT;
		
	serial_end();
	
	return 0;
}
