/*
 * ADC.c
 *
 *  Created on: 04/11/2019
 *      Author: ravenelco
 */
#include "lib/include.h"

/* Se configurarán los siguientes módulos:
ADC módulo 1: secuenciador 1: canal 1 2 6 7
Se omitirá la lectura del canal 4 ya que entre el secuenciador 1 y 3 
solamente se pueden leer 5 canales como máximo 
*/
extern void Configura_Reg_ADC0(void)
{
     //Pag 396 para inicializar el modulo de reloj del adc RCGCADC
    SYSCTL->RCGCADC =  (1<<1);  // Se activan los canales 1 y 0
    //Pag 382 (RGCGPIO) Puertos base habilitación del reloj
    /* Los canales respectivamente pertenecen a: 
    1 : PE2
    2 : PE1
    9 : PE4
    */
    //                    E   
    SYSCTL->RCGCGPIO |= (1<<4);
    //Pag 760 (GPIODIR) Habilta los pines como I/O un cero para entrada y un uno para salida
            // Para el puerto E
            GPIOE_AHB->DIR = (0<<1) | (0<<2) | (0<<4); //PE1, PE2 y PE4
    //(GPIOAFSEL) pag.770 Enable alternate función para que el modulo analógico tenga control de esos pines
            // Para el puerto E
            GPIOE_AHB->AFSEL =  (1<<1) | (1<<2) | (1<<4); //PE1, PE2 y PE4 
    //(GPIODEN) pag.781 desabilita el modo digital
            // Para el puerto E
            GPIOE_AHB->DEN=  (1<<1) | (1<<2) | (1<<4); ////PE1, PE2 y PE4
    //Pag 787 GPIOPCTL registro combinado con el GPIOAFSEL y la tabla pag 1808
            // Para el puerto E
            GPIOE_AHB->PCTL = GPIOE_AHB->PCTL & (0xFFF0F00F); //PE1, PE2 y PE4
    //(GPIOAMSEL) pag.786 habilitar analogico
            // Para el puerto E
            GPIOE_AHB->AMSEL =  (1<<1) | (1<<2) | (1<<4); //PE1, PE2 y PE4
    //Pag 1159 El registro (ADCPC) establece la velocidad de conversión por segundo
            // Para el módulo 1
            ADC1->PC = ((1<<2)|(0<<1)|(1<<0)); //1 Msps
    //Pag 1099 Este registro (ADCSSPRI) configura la prioridad de los secuenciadores
            // Para el módulo 1
            ADC1->SSPRI = 0x3102;          // Se le da mayot prioridad al sec 1 (0 alta - 3 baja)
    //Pag 1077 (ADCACTSS) Este registro controla la desactivación de los secuenciadores
            // Para el módulo 1
            ADC1->ACTSS  =   (0<<3) | (0<<2) | (0<<1) | (0<<0);
    //Pag 1091 Este registro (ADCEMUX) selecciona el evento que activa la conversión (trigger)
            // Para el módulo 1 
            ADC1->EMUX  = (0x0000);
    //Pag 1129 Este registro (ADCSSMUX2) define las entradas analógicas con el canal y secuenciador seleccionado
            // Para el módulo 1
            ADC1->SSMUX1 = 0x0921;                            // Lectura 4 Canal 7  Lectura 3 Canal 6  Lectura 2 Canal 1 Lectura 1 Canal 2 
    //pag 868 Este registro (ADCSSCTL2), configura el bit de control de muestreo y la interrupción
            // Para el módulo 1
            ADC1->SSCTL1 = (1<<14) | (1<<13); // La cuarta muestra es la última
    /* Enable ADC Interrupt */
            // Para el módulo 1
            ADC1->IM |= (1<<1); /* Unmask ADC1 sequence 1 interrupt pag 1082*/
    //Pag 1077 (ADCACTSS) Este registro controla la activación de los secuenciadores                  //
            // Para el módulo 11
            ADC1->ACTSS = (0<<3) | (0<<2) | (1<<1) | (0<<0); // Activa sec 1 
            ADC1->PSSI |= (1<<1);                            //
}

extern void ADC0_InSeq2(uint16_t *Result, uint16_t *duty){
    //Para el módulo 1
        ADC1->PSSI = 0x00000002;             // Activa sec 1
        while((ADC1->RIS&0x02)==0){};        // espera al convertidor
        Result[0] = ADC1->SSFIFO1&0xFFF;     // Leer el resultado del canal 9
        Result[1] = ADC1->SSFIFO1&0xFFF;     // Leer el resultado del canal 2
        Result[2] = ADC1->SSFIFO1&0xFFF;     // Leer el resultado del canal 1
        ADC1->ISC = 0x0002;                  //Conversion finalizada
       duty[0] = (int) ((Result[0] * 6250) / 4096.0); 
       duty[1] = (int) ((Result[1] * 6250) / 4096.0); 
       duty[2] = (int) ((Result[2] * 6250) / 4096.0); 
       ADC1->ISC = 0x0004;  //Conversion finalizada
}


/*
En esta función se configuran las siguientes salidas de PWM
PWM 1: PF1 gen 0 - B
PWM 3: PF3 gen 1 - B
PWM 5: PG1 gen 2 - B
*/
extern void Configurar_PWMMM(uint16_t frec_pwm,uint16_t duty_pwm){
    SYSCTL->RCGCPWM |= (1<<0); /*Enable reloj de modulo PWM0 pag 398*/
    SYSCTL->RCGCGPIO |= (1<<5) | (1<<6); /*Enable reloj de GPIO Puerto F y G pag 340 pin 5*/
    PWM0-> CC |= (1 << 8) | (1 << 2) | (1 << 0);

    //Para el puerto F
    GPIOF_AHB->AFSEL |= (0x0A); //Indicamos que el pin PF3 y PF1  tiene una función alternativa
    //Para el puerto G
    GPIOG_AHB->AFSEL |= (0x02); //Indicamos que el pin PG1 tiene una función alternativa

    //Para el puerto F
    GPIOF_AHB->PCTL &= ~(0x0000F0F0);
    GPIOF_AHB->PCTL |=  (0x00006060); /*Combinado con la tabla Pag 1351 y el registro PCTL le digo que es pwm Pag 689*/
    //Para el puerto G
    GPIOG_AHB->PCTL &= ~(0x000000F0);
    GPIOG_AHB->PCTL |=  (0x00000060); /*Combinado con la tabla Pag 1351 y el registro PCTL le digo que es pwm Pag 689*/

    //Para el puerto F
    GPIOF_AHB->DEN |= (0x0A); /* para decirle si es digital o no Pag 682*/
    //Para el puerto G
    GPIOG_AHB->DEN |= (0x02); /* para decirle si es digital o no Pag 682*/

    //Para el generador 0
    PWM0->_0_CTL &= ~(1<<0); /*Bloqueo y desbloqueo*/
    PWM0->_0_CTL &= ~(1<<1); //Modo de cuenta abajo
    //Para el generador 1
    PWM0->_1_CTL &= ~(1<<0); /*Bloqueo y desbloqueo*/
    PWM0->_1_CTL &= ~(1<<1); //Modo de cuenta abajo
    //Para el generador 2
    PWM0->_2_CTL &= ~(1<<0); /*Bloqueo y desbloqueo*/
    PWM0->_2_CTL &= ~(1<<1); //Modo de cuenta abajo

    //Para el generador 0
    PWM0->_0_GENB = (0x0000080C); /*Registro de las acciones del pwm Pag 1282*/
    //Para el generador 1
    PWM0->_1_GENB = (0x0000080C); /*Registro de las acciones del pwm Pag 1282*/
    //Para el generador 2
    PWM0->_2_GENB = (0x0000080C); /*Registro de las acciones del pwm Pag 1282*/

    //Para el generador 0
    PWM0->_0_LOAD = (int)(312500/frec_pwm); /*cuentas=fclk/fpwm  para 1khz cuentas = (16,000,000/1000)*/
    //Para el generador 1
    PWM0->_1_LOAD = (int)(312500/frec_pwm); /*cuentas=fclk/fpwm  para 1khz cuentas = (16,000,000/1000)*/
    //Para el generador 2
    PWM0->_2_LOAD = (int)(312500/frec_pwm); /*cuentas=fclk/fpwm  para 1khz cuentas = (16,000,000/1000)*/

    //Para el generador 0
    PWM0->_0_CMPB = (int)((1.0 - (duty_pwm / 100.0)) * (312500/frec_pwm));
    //Para el generador 1
    PWM0->_1_CMPB = (int)((1.0 - (duty_pwm / 100.0)) * (312500/frec_pwm));
    //Para el generador 2
    PWM0->_2_CMPB = (int)((1.0 - (duty_pwm / 100.0)) * (312500/frec_pwm));

    //Para el generador 1
    PWM0->_0_CTL = (1<<0);// Se activa el generador 
    //Para el generador 1
    PWM0->_1_CTL = (1<<0);// Se activa el generador
    //Para el generador 1
    PWM0->_2_CTL = (1<<0);// Se activa el generador


    PWM0->ENABLE = (1<<1)|(1<<3)|(1<<5); /*habilitar el bloque pa que pase Pag 1247*/
}