#ifndef PWM_H_
#define PWM_H_


/************************* MOTOR PWM **************************/
/// Modulo of Flex timer 0 for obtening an 10,35kHz (max 11kHz)
#define PWM_MOD_VALUE 0x1200
/// Prescaler value for the count period
#define MOTOR_PRESC_VALUE 0b000
/// FTM0_CHANNEL0 is an output for the pin PTD4
#define LOC_LEFT_PWM 0x10
/// FTM0_CHANNEL2 is an output for the pin PTD5
#define LOC_RIGHT_PWM 0x20
/// Location of the enable pin for the left motor H-bridge (PTE24)
#define LOC_LEFT_ENABLE 0x1000000
/// Location of the enable pin for the right motor H-bridge (PTE25)
#define LOC_RIGHT_ENABLE 0x2000000


/**
 * Initialisation fonction of the PWM of the left and right motor and the control bytes.
 * The default duty cycle is 50%, and the left and right motors are disabled
 */
void init_motor_pwm(void);

/**
 * This function is used to set a duty value of the PWM send to the left motor
 * \param duty_cycle : the duty cycle of the pwm (must be between 0 and 100%) 
 */
void set_left_pwm(float duty_cycle);

/**
 * This function is used to set a duty value of the PWM send to the right motor
 * \param duty_cycle : the duty cycle of the pwm (must be between 0 and 100%) 
 */
void set_right_pwm(float duty_cycle);

/**
 * The fonction enable the left motor
 */
void enable_left_motor(void);

/**
 * The fonction disable the left motor
 */
void disable_left_motor(void);

/**
 * The fonction enable the right motor
 */
void enable_right_motor(void);

/**
 * The fonction disable the right motor
 */
void disable_right_motor(void);


/************************** SERVO PWM **************************/
/// Location of the servo pwm pin : PTA8
#define LOC_SERVO_PWM 0x100
/// Modulo of the flex timer for an 20ms period (0x3A2C)
#define SERVO_MOD_VALUE 0x3A30
/// Prescaler of the flex timer for optening the correct count period
#define SERVO_PRESC_VALUE 0b110
/// Distinguable value for PWM : value for 1 degrees
#define SERVO_PWM_1DEGREE 7,32
/// Distinguable value for PWM : value for middle position
#define SERVO_PWM_MIDDLE 1117
/// Enable or not the verification of the angles submit to the function set_servo_angle
#define VERIFY_ANGLE

/**
 * Initialization of the pwm that control the servomotor.
 * By default, the servomotor will be positionned as the wheel of the car make him go straight foward (0 degree)
 */
void init_servo_pwm(void);

/**
 * Set the angle of the wheel with the servo motor
 * \param angle : the angle of the wheel, positive is right and negative angle is left
 * @precondition : -30 <= angle <= 30
 */
void set_servo_angle(int angle);


#endif /* PWM_H_ */
