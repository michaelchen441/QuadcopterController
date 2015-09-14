//
//  quadcopter.cpp
//  QuadcopterControl
//
//  Created by Dennis Chen on 9/13/15.
//  Copyright (c) 2015. All rights reserved.

//


#include <stdio.h>
#include "quadcopter.h"
#include "quadcopter_sim.h"

// --------------------------------------------------------------------------
//	Use global variables to track quadcopter state.  Encapsulate this in a c++ class later
//	Pitch, Roll, Height

// orientation: 0 -> flat, (-)127 -> max tilt
// leaning forwards / backwards: positive -> forwards
float	g_QC_Pitch;		//	[-127, 127]

// leaning right / left: positive -> right
float	g_QC_Roll;		//	[-127, 127]

float	g_QC_Height;	//	inches - [0, 255]

const float g_QC_RoterHoverSpeed = 50;


// --------------------------------------------------------------------------
//	Rotor settings

// set the motor strength: 0 -> off, 255 -> max thrust
uint8_t	g_QC_RotorFR;
uint8_t	g_QC_RotorFL;
uint8_t	g_QC_RotorBR;
uint8_t	g_QC_RotorBL;

// --------------------------------------------------------------------------


// get the orientation: 0 -> flat, (-)127 -> max tilt
// leaning forwards / backwards: positive -> forwards
int8_t getPitch(void)
{
    return((int8_t)g_QC_Pitch);
}

// leaning right / left: positive -> right
int8_t getRoll()
{
    return((int8_t)g_QC_Roll);
}

// get the height: 0 -> on the ground, 255 -> 255 inches high
uint8_t getHeight(void)
{
    return((uint8_t)g_QC_Height);
}

// --------------------------------------------------------------------------

// front right
void setFR(uint8_t inSetting)
{
    g_QC_RotorFR = inSetting;
}

// front left
void setFL(uint8_t inSetting)
{
    g_QC_RotorFL = inSetting;
}

// back right
void setBR(uint8_t inSetting)
{
    g_QC_RotorBR = inSetting;
}

// back left
void setBL(uint8_t inSetting)
{
    g_QC_RotorBL = inSetting;
}

// --------------------------------------------------------------------------

//	initilize the quadcopter's states.
//
//	This is useful to put the quadcopter in an unusual attitude

void qc_init(int8_t		inPitch,
             int8_t		inRoll,
             uint8_t	inHeight)
{
    g_QC_Pitch	= inPitch;
    g_QC_Roll	= inRoll;
    g_QC_Height	= inHeight;
    
}


//	Calculate quadcopter's stats based on the rotor settings.
//
//	1, each call to the qc_step fucntion simulates a fixed period of time
//	2, the simulation of the flight dynmaic is fairly crude

void qc_RunSimulationCycle(void)
{
    // calculate the height change
    // ASSUMPTION: rotor speed of g_QC_RoterHoverSpeed maintains height
    float	averageRotor = (g_QC_RotorFR + g_QC_RotorFL + g_QC_RotorBR + g_QC_RotorBL) / 4;
    float	deltaHeight = (averageRotor - g_QC_RoterHoverSpeed) / 10;
    float   value;

    value = (float)g_QC_Height + deltaHeight;
    if (value < 0)
        value = 0;
    if (value > 255)
        value = 255;
    g_QC_Height = value;
    
    // calculate pitch change
    float	frontRotor = g_QC_RotorFR + g_QC_RotorFL;
    float	backRotor = g_QC_RotorBR + g_QC_RotorBL;
    float	deltaPitch = (backRotor - frontRotor) / 2;
    
    value = (float)g_QC_Pitch + deltaPitch;
    if (value < -127)
        value = -127;
    if (value > 127)
        value = 127;
    g_QC_Pitch = value;
    
    
    // calculate pitch change
    float	leftRotor = g_QC_RotorFL + g_QC_RotorBL;
    float	rightRotor = g_QC_RotorFR + g_QC_RotorBR;
    float	deltaRoll = (leftRotor - rightRotor) / 2;
    
    value = (float)g_QC_Roll + deltaRoll;
    if (value < -127)
        value = -127;
    if (value > 127)
        value = 127;
    g_QC_Roll = value;
}


