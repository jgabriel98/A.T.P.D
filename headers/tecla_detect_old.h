#ifndef CIMA
#define CIMA 258
#endif
#ifndef BAIXO
#define BAIXO 852
#endif
#ifndef ESQ
#define ESQ 654
#endif
#ifndef DIR
#define DIR 456
#endif
#define INIT_TELA_STECLA -1


#include <termios.h>
#include <stdio.h>

void initTermios(int echo);
void resetTermios(void);
char getch_(int echo);
char getcha(void);
char getche(void);
int ler_tecla(void);