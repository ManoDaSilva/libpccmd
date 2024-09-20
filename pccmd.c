/*
 * pccmd.c
 *
 * Implements basic PCCMDV2 commands for use in Micro17
 *  Created on: May 21, 2024
 *      Author: Manoel Casquilho - ON6RF
 */


#include "pccmd.h"


extern UART_HandleTypeDef huart1;

void pcCmdWriteDisplay(char line1[], char line2[]){
	char outString[58];
	sprintf(outString,"*SET,UI,TEXT,%-12s%-12s",line1,line2);
	transmitSerial(outString);
}

void pcCmdBlankDisplay(){
	transmitSerial("*SET,UI,TEXT,");
}

void pcCmdAudioControl(bool status){
	char cmd[4]="OFF";
	if(status){
		strcpy(cmd , "ON");
	}
	char outString[29];
	sprintf(outString,"*SET,CTRL,AUD,%s",cmd);
	transmitSerial(outString);
}

void pcCmdSetChannelNr(int channelNr){
	char outString[12];
	sprintf(outString,"C%i",channelNr);
	transmitSerial(outString);
}

void pcCmdSetVolume(int volumeVal){
	char outString[18];
	sprintf(outString,"*SET,UI,AFVOL,%i",volumeVal);
	transmitSerial(outString);
}

void pcCmdSetPower(int powerLevel){
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
	transmitSerial(outString);

}


int pcCmdGetProfileComment(char* lineContents, int lineNr){
	//char outString[30];
	//sprintf(outString,"*GET,INFO,COMMENT,%i",lineNr);
	//transmitSerial(outString);
	transmitSerial("*GET,INFO,COMMENT,1");
	HAL_Delay(100);
	char inString[80];
	//TODO: check if the Rx strings make it into the buffer
	receiveSerial(inString);
	strcpy(lineContents,inString);
	return 1;
}

void pcCmdResetRadio(){
	transmitSerial("*SET,UI,RESET");
}


void transmitSerial(char contents[]){
	uint8_t txBuffer[strlen(contents)+4];
	sprintf((char*)txBuffer, "%s%s%s", PCCMD_BOL, contents, PCCMD_EOL); //Adding BOL and EOL designators to the char line
	HAL_UART_Transmit(&huart1, txBuffer, strlen((char*)txBuffer), 500);
}

void receiveSerial(char* rxContents){
	uint8_t rxBuffer[80];//="*NTF,INFO,COMMENT,N0CALL";
	int rxSize = 80;
	HAL_UART_Receive(&huart1,rxBuffer, rxSize, 1000);
	sprintf(rxContents,(char*)rxBuffer);
}
