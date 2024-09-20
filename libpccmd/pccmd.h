/*
 * pccmd.h
 *
 *  Created on: May 21, 2024
 *      Author: Manoel Casquilho - ON6RF
 */

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "stm32g4xx_hal.h"

//PC-CMDV2 line designators
#define PCCMD_BOL "\x02"
#define PCCMD_EOL "\x03"

//writes screen to the display (left padded), each line is 12 chars max
void pcCmdWriteDisplay(char line1[], char line2[]);

//Returns the screen's display to an empty state
void pcCmdBlankDisplay();

//Enables or disable audio out
void pcCmdAudioControl(bool status);

//Selects a specifc channel Nr
void pcCmdSetChannelNr(int channelNr);

//Sets the radio's volume to a specific value (0-255)
void pcCmdSetVolume(int volumeVal);

//Set the radio's power level (1 = LOW1, 2 = LOW2, 3 = HIGH)
void pcCmdSetPower(int powerLevel);

//Reads the profile comment line (1 or 2), and puts the result in a char pointer
//WIP
int pcCmdGetProfileComment(char* lineContents, int lineNr);

//Resets (reboots) the radio
void pcCmdResetRadio();

//Internal function to be modified to each specific's microcontroller/program's needs
void transmitSerial(char contents[]);
void receiveSerial(char* rxContents);

