/**
* \file Camera
* \brief Fonction et procÃ©dure de controle de la camera
* \version 0.1
* \date 6 novembre 2013
*/

#ifndef CAMERA_H_
#define CAMERA_H_


/// Demi periode de la Serial Clock de la camera
//#define TIMER_FREQUENCY 0x000000953
//#define TIMER_FREQUENCY 0x000000A53
#define TIMER_FREQUENCY 0x000000753
/// Temps d attente avant de commencer un nouveau cycle de transfert de donnees de la camera
#define WAITING_TIME 0x00000D6A
//#define WAITING_TIME 0x00107F6A

/// Masque de config. output, localisation du bit SI dans le registre
#define LOC_SI 0x100
/// Localisatin du bit SCLK dans le registre
#define LOC_SCLK 0x200



/**
* Machine à état infinie d'acquisition des données de la caméra
* Attention : Cette fonction est autonome (rappelé automatiquement par PIT0) et
* doit être executer par init_camera() (besoin d'initialisation de plusieurs paramètres)
*/
void camera_acquisition(void);

/**
* Choisi et configure les pins de la carte qui vont dialoguer avec la camera
* Info erreur : Si l'adc n'a pas eu le temps de convertir un bit, le logo "%" s'affichera alors sur l'écran lcd
*/
void init_camera(void);

void isr_PIT0(void);

#endif /* CAMERA_H_ */
