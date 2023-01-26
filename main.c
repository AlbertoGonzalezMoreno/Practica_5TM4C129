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
    /*Ya se pudo incluir la librería del PWM, no la estaba añadiendo en el make file jeje*/
    Configurar_PWM(frec_pwm,duty_pwm);
    /*En este código se realiza el experimento 2 y se varía el ancho de pulso mediante la lectura del ADC*/
    while(1)
    {
    ADC0_InSeq2(Result,duty); //llamada a la conversion por procesador
       PWM0->_0_CMPB = duty[0];
       PWM0->_1_CMPB = duty[1];
       PWM0->_2_CMPB = duty[2];
    }
}