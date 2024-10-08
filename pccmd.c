/*
 * pccmd.c
 *
 * Implements basic PCCMDV2 commands for use in Micro17
 *  Created on: May 21, 2024
 *      Authors: Manoel - ON6RF
 *				 Wojciech - SP5WWP
 */


#include "pccmd.h"


void pcCmdWriteDisplay(UART_HandleTypeDef *uart, char line1[], char line2[]){
	char outString[58];
	sprintf(outString,"*SET,UI,TEXT,%-12s%-12s",line1,line2);
	transmitSerial(uart, outString);
}

void pcCmdBlankDisplay(UART_HandleTypeDef *uart){
	transmitSerial(uart, "*SET,UI,TEXT,");
}

void pcCmdAudioControl(UART_HandleTypeDef *uart, bool status){
	char cmd[4]="OFF";
	if(status){
		strcpy(cmd , "ON");
	}
	char outString[29];
	sprintf(outString,"*SET,CTRL,AUD,%s",cmd);
	transmitSerial(uart, outString);
}

void pcCmdSetChannelNr(UART_HandleTypeDef *uart, int channelNr){
	char outString[12];
	sprintf(outString,"C%i",channelNr);
	transmitSerial(uart, outString);
}

void pcCmdSetVolume(UART_HandleTypeDef *uart, int volumeVal){
	char outString[18];
	sprintf(outString,"*SET,UI,AFVOL,%i",volumeVal);
	transmitSerial(uart, outString);
}

void pcCmdSetPower(UART_HandleTypeDef *uart, int powerLevel){
	char powerString[6];
	switch(powerLevel){
	case 1:
		sprintf(powerString,"LOW1");
		break;
	case 2:
		sprintf(powerString,"LOW2");
		break;
	case 3:
		sprintf(powerString,"HIGH");
		break;
	default:
		sprintf(powerString,"LOW1");
	}
	char outString[22];
	sprintf(outString,"*SET,MCH,RFPWR,%s",powerString);
	transmitSerial(uart, outString);

}


int pcCmdGetProfileComment(UART_HandleTypeDef *uart, char* lineContents, int lineNr){
	//char outString[30];
	//sprintf(outString,"*GET,INFO,COMMENT,%i",lineNr);
	//transmitSerial(outString);
	transmitSerial(uart, "*GET,INFO,COMMENT,1");
	HAL_Delay(100);
	char inString[80];
	//TODO: check if the Rx strings make it into the buffer
	receiveSerial(uart, inString);
	strcpy(lineContents,inString);
	return 1;
}

void pcCmdResetRadio(UART_HandleTypeDef *uart){
	transmitSerial(uart, "*SET,UI,RESET");
}


void transmitSerial(UART_HandleTypeDef *uart, char contents[]){
	uint8_t txBuffer[strlen(contents)+4];
	sprintf((char*)txBuffer, "%s%s%s", PCCMD_BOL, contents, PCCMD_EOL); //Adding BOL and EOL designators to the char line
	HAL_UART_Transmit(uart, txBuffer, strlen((char*)txBuffer), 500);
}

void receiveSerial(UART_HandleTypeDef *uart, char* rxContents){
	uint8_t rxBuffer[80];//="*NTF,INFO,COMMENT,N0CALL";
	int rxSize = 80;
	HAL_UART_Receive(uart, rxBuffer, rxSize, 1000);
	sprintf(rxContents,(char*)rxBuffer);
}
