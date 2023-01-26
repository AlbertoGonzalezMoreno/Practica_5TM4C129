#include "lib/include.h"
int main(void)
{
    uint16_t valor = 0;
    char c = '5';
    int L = 10;
    char Num[L];
    //  Configuración del PWM
    uint16_t frec_pwm = 50;
    uint16_t duty_pwm = 0.0;
    //////////////////////////////////////////////////////////////Configuraciones
    Configurar_PLL();  //Confiuracion de velocidad de reloj
    Configurar_UART0();
    /*Ya se pudo incluir la li brería del PWM, no la estaba añadiendo en el make file jeje*/
    Configurar_PWM(frec_pwm,duty_pwm);
    /*Para el experimento 3 no pude hacerlo con simulink :(( sinceramente, no pude encontrar bloques que concatenen
    cadenas de caracteres para poder mandar qué color modificar y luego, cuál será su intensidad, entonces lo hice con un 
    programa de matlab*/
    
    while(1)
    {
    c = readChar(); 
    L = readString(',',&Num[0]);
    duty_pwm = string2num(L,Num);
         switch(c)
         {
             case 'r':
                 printString("Ah loco cambiaste la intensidad del color rojo");
                 PWM0->_0_CMPB = duty_pwm;
                 break;
             case 'b':
                 printString("Ah canijo cambiaste la intensidad del color azul");
                 PWM0->_1_CMPB = duty_pwm;
                 break;
             case 'g':
                 printString("Ah buenisimo cambiaste la intensidad del color verde");
                 PWM0->_2_CMPB = duty_pwm;
                 break;
             default:
                if (c < 47)
                {
                PWM0->_0_CMPB = 6250;
                PWM0->_1_CMPB = 6250;
                PWM0->_2_CMPB = 6250;
                }
                else 
                {

                }
            printChar((char)valor);
            break;
         }
    }
}