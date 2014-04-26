/*
 * blocks_main.c
 *
 *  Created on: Apr 25, 2014
 *      Author: Axel
 */

#include "Blocks/block_main.h"

Block block;

void init_blocks(void){
	block.straight = 0;
	block.right = 0;
	block.left = 0;
	block.hill = 0;
	block.chicanes = 0;
	block.start_finish = 0;
}

void block_next(void){
	if (block.straight) {
		block_straight();
	}
	
	if (block.start_finish) {
		block_start_finish();
	}
	
	if (block.left || block.right) {
		block_turn();
	}
	
	if (block.chicanes) {
		block_chicanes();
	}
	
	if (block.hill) {
		block_hill();
	}
	
	//wait_start
	
}
