#ifndef CIMA
#define CIMA 258
#endif
#ifndef BAIXO
#define BAIXO 259
#endif
#ifndef ESQ
#define ESQ 260
#endif
#ifndef DIR
#define DIR 261
#endif
#ifndef ESC_KEY
#define ESC_KEY 255 
#endif
#ifndef DEL_KEY
#define DEL_KEY 333 
#endif
#define INIT_TELA_STECLA -1


#include <termios.h>
#include <stdio.h>

void initTermios(int echo,int block);
void resetTermios(void);
char getch_(int echo,int block);
char getcha(void);
char getcha_noblock(void);
char getche(void);
char getche_noblock(void);
int ler_tecla(void);