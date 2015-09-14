//
//  main.cpp
//  QuadcopterControl
//
//  Created by Michael Chen on 9/9/15.
//  Copyright (c) 2015 Michael Chen. All rights reserved.
//

#include <iostream>
#include "quadcopter_sim.h"
#include "quadcopter.h"

using namespace std;

int main ()
{
	float	Height_Kp = 3;
	float	Height_Ki = 1;
	float	Height_Kd = 1;

	float	Pitch_Kp = 0.3;
	float	Pitch_Ki = 0.1;
	float	Pitch_Kd = 0.1;

	float	Roll_Kp = 0.3;
	float	Roll_Ki = 0.1;
	float	Roll_Kd = 0.1;

	float	initialPitch = 10;
	float	initialRoll = 10;
	float	initialHeight = 0;
	
	qc_init(initialPitch, initialRoll, initialHeight);
	
	//--------------------------
	int desiredHeight = 100;
	int desiredPitch = 0;
	int desiredRoll = 0;
	//--------------------------
	int cumulativeErrorHeight = 0;
	int errorHeightChange = 0;
	int previousErrorHeight = 0;

	int cumulativeErrorPitch = 0;
	int errorPitchChange = 0;
	int previousErrorPitch = 0;

	int cumulativeErrorRoll = 0;
	int errorRollChange = 0;
	int previousErrorRoll = 0;
	
	cout << "trial  | height    Pitch     Roll   | Height_Change   P_Change   R_Change    | rotorFL    rotorFR    rotorBL    rotorBR\n\n";
	for (int loop = 0; loop < 100; loop++)
	{
		int currentHeight = getHeight();
		int errorHeight = desiredHeight - currentHeight;
		cumulativeErrorHeight += errorHeight;
		
		errorHeightChange = errorHeight - previousErrorHeight;
		
		float rotorSpeedHeight = Height_Kp * errorHeight
								+ Height_Ki * cumulativeErrorHeight
								+ Height_Kd * errorHeightChange;
		
		//----------------------------------------------
		
		int currentPitch = getPitch();
		int errorPitch = desiredPitch - currentPitch;
		cumulativeErrorPitch += errorPitch;
		
		errorPitchChange = errorPitch - previousErrorPitch;
		
		float rotorSpeedPitch = Pitch_Kp * errorPitch
								+ Pitch_Ki * cumulativeErrorPitch
								+ Pitch_Kd * errorPitchChange;

		//----------------------------------------------

		int currentRoll = getRoll();
		int errorRoll = desiredRoll - currentRoll;
		cumulativeErrorRoll += errorRoll;
		
		errorRollChange = errorRoll - previousErrorRoll;
		
		float rotorSpeedRoll = Roll_Kp * errorRoll
								+ Roll_Ki * cumulativeErrorRoll
								+ Roll_Kd * errorRollChange;
		
		//--------------------------
		float rotorFR = rotorSpeedHeight - rotorSpeedPitch - rotorSpeedRoll;
		float rotorFL = rotorSpeedHeight - rotorSpeedPitch + rotorSpeedRoll;
		float rotorBR = rotorSpeedHeight + rotorSpeedPitch - rotorSpeedRoll;
		float rotorBL = rotorSpeedHeight + rotorSpeedPitch + rotorSpeedRoll;
		
		if (rotorFR > 255)
			rotorFR = 255;
		if (rotorFR < 0)
			rotorFR = 0;
		
		if (rotorFL > 255)
			rotorFL = 255;
		if (rotorFL < 0)
			rotorFL = 0;
		
		if (rotorBR > 255)
			rotorBR = 255;
		if (rotorBR < 0)
			rotorBR = 0;
		
		if (rotorBL > 255)
			rotorBL = 255;
		if (rotorBL < 0)
			rotorBL = 0;
		
		setFR(rotorFR);
		setFL(rotorFL);
		setBR(rotorBR);
		setBL(rotorBL);
		
		previousErrorHeight = errorHeight;
		previousErrorPitch = errorPitch;
		previousErrorRoll = errorRoll;
		
		cout << loop << "      |  " << currentHeight << "        " << currentPitch << "        " << currentRoll << "      |  "
				<< rotorSpeedHeight << "          "
				<< rotorSpeedPitch << "          "
				<< rotorSpeedRoll << "        |  "
				<< rotorFL << "          "
				<< rotorFR << "          "
				<< rotorBL << "          "
				<< rotorBR <<"\n";
		
		qc_RunSimulationCycle();
	}
	
	
	return 1;
}



