
#include "soundApp.h"

//Commmands
const uint8 volume[ARRAY_SIZE] = {0x7E,0x06,0x00,0x00,0x1D,0xFF,0xDC,0xEF}; //Volume set to 29 (30=max)
const uint8 setSource[ARRAY_SIZE] = {0x7E,0x09,0x00,0x00,0x02,0xFF,0xF5,0xEF};  //Setting source to SD card

const uint8 playStart[ARRAY_SIZE] = {0x7E,0x0F,0x00,0x01,0x01,0xFF,0xEF,0xEF}; //First song on SD card
const uint8 playStop[ARRAY_SIZE] = {0x7E,0x0F,0x00,0x01,0x02,0xFF,0xEE,0xEF};  //Second song on SD card
const uint8 playPoint[ARRAY_SIZE] = {0x7E,0x0F,0x00,0x01,0x03,0xFF,0xED,0xEF}; //Third song on SD card

//Init driver and turn volume to max
void soundInit()
{
    soundUART_Start();
    soundUART_PutArray(setSource, ARRAY_SIZE);
    CyDelay(20);
    soundUART_PutArray(volume, ARRAY_SIZE);
    CyDelay(20);
}

//Send commands to SOMO
void sendToSOMO(int sound)
{
    switch(sound)
    {
        case 1: soundUART_PutArray(playStart, ARRAY_SIZE);
            break;
        case 2: soundUART_PutArray(playStop, ARRAY_SIZE);
            break;
        case 3: soundUART_PutArray(playPoint, ARRAY_SIZE);
            break;
        
        soundUART_ClearTxBuffer();
    }
}