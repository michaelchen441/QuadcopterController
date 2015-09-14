//
//  quadcopter_sim.h
//  QuadSim
//
//  Created by Dennis Chen on 9/13/15.
//  Copyright (c) 2015 ChenDennis. All rights reserved.
//

#ifndef QuadSim_quadcopter_sim_h
#define QuadSim_quadcopter_sim_h

#include <stdint.h>

void qc_init(int8_t		inPitch,
			 int8_t		inRoll,
			 uint8_t	inHeight);

void qc_RunSimulationCycle(void);

#endif
