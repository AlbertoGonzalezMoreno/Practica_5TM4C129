#include "lib/include.h"
int main(void)
{
    //  Configuración del PWM
    uint16_t frec_pwm = 50;
    uint16_t duty_pwm = 50.0;
    uint16_t Result[3];
    uint16_t duty[3];
    //////////////////////////////////////////////////////////////Configuraciones
    Configurar_PLL();  //Confiuracion de velocidad de reloj
    Configurar_UART0();
    Configura_Reg_ADC0();
    /*Se incluyó la configuración del PWM en el main porque al hacerlo en una función en alguna otra librería
    me marcaba error, incluso al realizar una librería nueva, por lo que decidí dejarlo aquí como parte 
    de la función principal */
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

    //Habilitar el módulo
    PWM0->ENABLE = (1<<1)|(1<<3)|(1<<5); /*habilitar el bloque pa que pase Pag 1247*/
    /*En este código se realiza el experimento 2 y se varía el ancho de pulso mediante la lectura del ADC*/
    while(1)
    {
    ADC0_InSeq2(Result,duty); //llamada a la conversion por procesador
       PWM0->_0_CMPB = duty[0];
       PWM0->_1_CMPB = duty[1];
       PWM0->_2_CMPB = duty[2];
    }
}