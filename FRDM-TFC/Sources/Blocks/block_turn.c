
#include "Blocks/block_main.h"

extern Block block;

void block_turn(void){
	braking();
	corner_entry();
	toning_medium();
	corner_exit();
}

void braking(void){
	//setMotorPWM();
	//suivi de ligne
}

void corner_entry(void){
	//freinage

	if (block.right) {
		//virage
	}
	if (block.left) {
		//virage
	}
}

void toning_medium(void){
	//acceleration
	
	if (block.right) {
		//virage
	}
	if (block.left) {
		//virage
	}
}

void corner_exit(void){
	//suivi de ligne
	//vitesse de croisiere
}
