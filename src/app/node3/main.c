/*
 * main.c
 *
 *  Created on: Dec 8, 2012
 *      Author: igor
 */
#include "platform.h"


PLATFORM_CONFIGURATION()
{
	AGENTS_CONFIGURATION(){
		
		//led
		AGENT_INIT(0x00, 0x03, 1111101000001101011111010000010001000000111111111111110011111100),
	},

	BOARD_ID(0x00)
};
