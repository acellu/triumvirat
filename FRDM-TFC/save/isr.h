/******************************************************************************
* File:    isr.h
* Purpose: Define interrupt service routines referenced by the vector table.
* Note: Only "vectors.c" should include this header file.
******************************************************************************/

#ifndef __ISR_H
#define __ISR_H 1


/* Example */
/*
#undef  VECTOR_101
#define VECTOR_101 lpt_isr

// ISR(s) are defined in your project directory.
extern void lpt_isr(void);
*/

#undef 	VECTOR_84
#define VECTOR_84 isr_PIT0

extern void isr_PIT0(void);

#undef 	VECTOR_85
#define VECTOR_85 isr_PIT1

extern void isr_PIT1(void);


#endif  //__ISR_H

/* End of "isr.h" */
