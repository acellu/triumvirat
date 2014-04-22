#ifndef FSM_H_
#define FSM_H_

#define DEFAULT_NUMBER_OF_LINE 200	

/// Disable the motors
//#define NO_MOTOR

//Typedef enum de la FSM (machine a etat).
typedef enum{
	Init=0,
	Wait_start,
	Following_line,
	Stop
}etat_e;


//Initialise les variables globales
void init_fsm(void); 

//Machine à état
void fsm(void);


#endif /* FSM_MAIN_H_ */
