/*
 * ADC.h
 *
 *  Created on: 04/11/2019
 *      Author: ravenelco
 */

#ifndef ADC_ADC_H_
#define ADC_ADC_H_

extern void Configura_Reg_ADC0(void);
extern void ADC0_InSeq2(uint16_t *Result, uint16_t *duty);
extern void Configurar_PWMMM(uint16_t frec_pwm,uint16_t duty_pwm);

#endif /* ADC_ADC_H_ */
