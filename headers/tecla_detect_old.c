#include <termios.h>
#include <stdio.h>
#include "tecla_detect.h"
//#include <wchar.h>

static struct termios old, new;              /*IMPORTANTE!!!!!!*/

/* Initialize new terminal i/o settings */
void initTermios(int echo) 
{
    tcgetattr(0, &old); /* grab old terminal i/o settings */
    new = old; /* make new settings same as old settings */
    new.c_lflag &= ~ICANON; /* disable buffered i/o */
    new.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
    tcsetattr(0, TCSANOW, &new); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void) 
{
    tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo) 
{
    char ch;
    initTermios(echo);
    ch = getchar();
    resetTermios();
    return ch;
}


/* Read 1 character without echo */
char getcha(void) 
{
    return getch_(0);
}

/* Read 1 character with echo */
char getche(void) 
{
    return getch_(1);
}

/* Descobri que a seta é composta por: 'esc' + '[' + 'A' ou 'B' ou 'C' ou 'D'*/
int ler_tecla(void){
    char caractere=getcha();

    if(caractere=='\e'){        //se é '\e', entao pode ser uma seta direcional ou esc

    getcha();
    switch( getcha()){
        case 'A':
        return CIMA;
        break;

        case 'B':
        return BAIXO;
        break;

        case 'C':
        return DIR;
        break;

        case 'D':
        return ESQ;
        break;
    }
    }else{
        return caractere;
    }
}





/* Let's test it out 
int main(void) {
  char c;
  int temp;
  printf("(getche example) please type a letter: ");
  c = getche();
  temp=c;
  printf("\nYou typed: %c    \t of int: ", c);
  printf(" %i \n",c);

  if(c == 27){
    printf("\t\tFOI!!!!!!!!!!: %i\n",'^');
    c=getch();
    printf("%c %i",c,c );
    c=getch();
  printf("%c %i\n\n",c,c );
  }

  printf("(getch example) please type a letter...");
  c = getch();
  printf("\nYou typed: %c\n", c);
  return 0;
}
 */