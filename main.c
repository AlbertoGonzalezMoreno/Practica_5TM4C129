#include "lib/include.h"
int main(void)
{
    uint16_t valor = 0;
    char c = '5';
    char h = 0;
    int L = 10;
    char Num[5] = {'0','0','0',0,0};
    //  Configuración del PWM
    uint16_t frec_pwm = 50;
    uint16_t duty_pwm = 0.0;
    //int k = atoi("12");
    //
    //////////////////////////////////////////////////////////////Configuraciones
    Configurar_PLL();  //Confiuracion de velocidad de reloj
    Configurar_UART0();
    /*Ya se pudo incluir la li brería del PWM, no la estaba añadiendo en el make file jeje*/
    Configurar_PWM(frec_pwm,duty_pwm);
    /*Para el experimento 3 no pude hacerlo con simulink :(( sinceramente, no pude encontrar bloques que concatenen
    cadenas de caracteres para poder mandar qué color modificar y luego, cuál será su intensidad, entonces lo hice con un 
    programa de matlab*/
    duty_pwm = string2num(3,&Num[0]);
    while(1)
    {
    c = readChar(); 
         switch(c)
         {
             case 'r':
               h = readChar();
                 L = readString(',',&Num[0]);
                duty_pwm = string2num(L,Num);
                 printString("rojo");
                 printChar((char)valor);
                 PWM0->_0_CMPB = duty_pwm;
                   h = readChar();
                 break;
             case 'b':
                 h = readChar();
                     L = readString(',',&Num[0]);
                    duty_pwm = string2num(L,Num);
                 printString("azul");
                 printChar((char)valor);
                 PWM0->_1_CMPB = duty_pwm;
                   h = readChar();
                 break;
             case 'g':
                 h = readChar();
                     L = readString(',',&Num[0]);
                    duty_pwm = string2num(L,Num);
                 printString("verde");
                 printChar((char)valor);
                 PWM0->_2_CMPB = duty_pwm;
                   h = readChar();
                 break;
             default:
                if (c < 47)
                {

                }
            break;
         }
    }
}