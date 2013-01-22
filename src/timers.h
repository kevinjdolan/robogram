#define PWM_DISCONNECTED 0
#define PWM_NONINVERTED 2
#define PWM_INVERTED 3

/**
 * 8-bit timer
 *
 * mode:
 *  0: Normal
 *  1: PWM Phase Corrected, 0xFF top
 *  2: CTC, OCR0A top
 *  3: Fast PWM, 0xFF top
 *  4: Reserved
 *  5: PWM Phase Corrected, OCR0A top
 *  6: Reserved
 *  7: Fast PWM, OCR0A top
 *
 * prescaler:
 *   0: disabled
 *   1: no prescaling
 *   2: clock/8
 *   3: clock/64
 *   4: clock/256
 *   5: clock/1024
 *
 * enableInterruptX:
 *   0: no interrupt
 *   1: interrupt on TIMER0_COMPX_vect, TIMER0_OVF_vect
 *
 * outModeXX:
 *   PWM_DISCONNECTED
 *   PWM_NONINVERTED
 *   PWM_INVERTED
 */
void timer0Init(char mode, char prescaler, char outModeD5, char outModeD6, char enableInterruptA, char enableInterruptB, char enableInterruptOverflow);

/**
 * 16-bit timer 
 *
 * mode:
 *  0: Normal, 0xFFFF top
 *  1: PWM, Phase Corrected, 8bit, 0x00FF top
 *  2: PWM, Phase Corrected, 9bit, 0x01FF top
 *  3: PWM, Phase Corrected, 10bit, 0x03FF top
 *  4: CTC,	OCR1A top 
 *  5: Fast PWM, 8bit, 0x00FF top 
 *  6: Fast PWM, 9bit, 0x01FF top 
 *  7: Fast PWM, 10bit, 0x03FF top 
 *  8: PWM, Phase and Frequency Corrected, ICR1 top 
 *  9: PWM, Phase and Frequency Corrected, OCR1A top 
 * 10: PWM, Phase Correct, ICR1 top
 * 11: PWM, Phase Correct, OCR1A top
 * 12: CTC, ICR1 top
 * 13: RESERVED	 
 * 14: Fast PWM, ICR1 top
 * 15: Fast PWM, OCR1A top 
 *
 * prescaler:
 *   0: disabled
 *   1: no prescaling
 *   2: clock/8
 *   3: clock/64
 *   4: clock/256
 *   5: clock/1024
 *
 * enableInterruptX:
 *   0: no interrupt
 *   1: interrupt on TIMER1_COMPX_vect, TIMER1_OVF_vect
 *
 * outModeXX:
 *   PWM_DISCONNECTED
 *   PWM_NONINVERTED
 *   PWM_INVERTED
 */
void timer1Init(char mode, char prescaler, char outModeB1, char outModeB2, char enableInterruptA, char enableInterruptB, char enableInterruptOverflow);

/**
 * 8-bit timer
 *
 * mode:
 *  0: Normal, 0xFF top
 *  1: PWM Phase Corrected
 *  2: CTC, OCR2 top
 *  3: Fast PWM, 0xFF top
 *
 * prescaler:
 *   0: disabled 
 *   1: no prescaling
 *   2: clock/8
 *   3: clock/32
 *   4: clock/64
 *   5: clock/128
 *   6: clock/256
 *   7: clock/1024
 *
 * enableInterruptX:
 *   0: no interrupt
 *   1: interrupt on TIMER2_COMPX_vect, TIMER2_OVF_vect
 *
 * outModeXXX
 *   PWM_DISCONNECTED
 *   PWM_NONINVERTED
 *   PWM_INVERTED
 */
void timer2Init(char mode, char prescaler, char outModeB3, char outModeD3, char enableInterruptA, char enableInterruptB, char enableInterruptOverflow);
