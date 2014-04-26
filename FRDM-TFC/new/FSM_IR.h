/*
 * FSM-IR.h
 *
 *  Created on: Apr 24, 2014
 *      Author: Muu La Fraga
 */

#ifndef FSM_IR_H_
#define FSM_IR_H_

// Masque emplacement des bits de controle du mux
#define CTRL_MUX_MASK 0x0C40

/// Typedef enum de la FSM IR
	typedef enum{
		Etat1 = 0,
		Etat2,
		Etat3,
		Etat4,
		Etat5,
		Etat6,
		Etat7,
		Etat8
	}etat_IR;



#endif /* FSM_IR_H_ */
