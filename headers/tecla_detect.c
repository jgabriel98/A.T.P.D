
#include <termios.h>
#include <stdio.h>
#include <stdio.h>
#include <stdio_ext.h>
#include "tecla_detect.h"

static struct termios old, new;

/* Initialize new terminal i/o settings */
void initTermios(int echo, int block) 
{
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  new = old; /* make new settings same as old settings */
  new.c_lflag &= ~ICANON; /* disable buffered i/o */
  new.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
  new.c_cc[VMIN] = block ? 1 : 0; // bytes until read unblocks.
  new.c_cc[VTIME] = 0; // timeout.
  tcsetattr(0, TCSANOW, &new); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void) 
{
  tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo,int block) 
{
    char ch;
    initTermios(echo,block);
    ch = getc(stdin);
    resetTermios();
    return ch;
}

/* Read 1 character without echo */
char getcha(void)         { return getch_(0,1); }
char getcha_noblock(void) { return getch_(0,0); }

/* Read 1 character with echo */
char getche(void)         { return getch_(1,1); }
char getche_noblock(void) { return getch_(1,0); }



/* Descobri que a seta é composta por: 'esc' + '[' + 'A' ou 'B' ou 'C' ou 'D'*/
int ler_tecla(void){
  //aguarda entrada no teclado
  char caractere=getcha();
  int retorno = (int)caractere;

  if(caractere=='\e'){        //se é '\e', entao pode ser uma seta direcional ou esc
    //verifica se mais algo além do '\e' estava no buffer stdin, e se sim, remove o '['
    if( getcha_noblock() == EOF ){
      retorno = ESC_KEY;
    }
    else{
      switch( getcha()){
        case 'A':
        retorno = CIMA;
        break;

        case 'B':
        retorno = BAIXO;
        break;

        case 'C':
        retorno = DIR;
        break;

        case 'D':
        retorno = ESQ;
        break;

        case '3': //tecla del
        if(getcha_noblock() == '~')
          retorno = DEL_KEY;
        break;
      }
    }

  }

  __fpurge(stdin);
  return retorno;
}
