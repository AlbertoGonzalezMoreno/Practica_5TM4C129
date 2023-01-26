#ifndef UART_UART_H_
#define UART_UART_H_

extern void Configurar_UART0(void);
extern char readChar(void);
extern void printChar(char c);
extern void printString(char* string);
extern int readString(char delimitador, char *string);
extern int string2num(int L, char *string);
#endif /* UART_UART_H_ */