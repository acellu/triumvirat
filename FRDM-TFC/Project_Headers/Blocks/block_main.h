/*
 * blocks_main.h
 *
 *  Created on: Apr 25, 2014
 *      Author: Axel
 */

#ifndef BLOCKS_MAIN_H_
#define BLOCKS_MAIN_H_

#include "TFC\TFC_arm_cm0.h"

#include "Blocks/block_straight.h"
#include "Blocks/block_turn.h"
#include "Blocks/block_hill.h"
#include "Blocks/block_chicanes.h"
#include "Blocks/block_start_finish.h"
#include "Blocks/block_unknown.h"

typedef struct{
	uint8 straight;
	uint8 right;
	uint8 left;
	uint8 chicanes;
	uint8 hill;
	uint8 start_finish;
}Block;

#endif /* BLOCKS_MAIN_H_ */
