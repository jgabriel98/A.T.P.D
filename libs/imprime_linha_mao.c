#if defined (_WIN32)||defined(__WINDOWS__)|| defined(_WIN64)												//para futuros usos em compilação no windows
	#include <conio.h>
	#define "clear" "\"cls\""
	#define sleep Sleep
	#define seg 1000

#elif linux																									//caso seja OS linux
	#include <ncurses.h>
	#include <term.h>                                            
	#include <unistd.h>												
	#define seg 1

#endif

#include <string.h>
#include <termios.h>
#include <wchar.h>

#define COR_ATK 9
#define COR_DEF 226
#define COR_LIVRE 240
#define COR_USADA 231


#define SIZE_HP 8000

#define SIZE_DECK 30
#define SIZE_MAO 5
#define SIZE_MESA 3

#define MODO_LIVRE 0	
#define MODO_USADA	-1
#define MODO_ATK 1
#define MODO_DEF 2



typedef struct{
	char nome[25];
	int atk;
	int def;
	short modo;														//por padrao é -1, para indicar mão descupada ( ver função gerar mão)
}Carta;



int imprime_linha_mao(int espaco,int num, char *str,Carta *mao, int POS){												//imprime uma string que vc quer e deixa o espaço entre elas como vc quiser (contando com o tamanho da string) parametros:(int intervalo,int numero de vezes(cartas),char *string,struct *mao)

	setupterm(NULL, STDOUT_FILENO, NULL);
  	putp(exit_attribute_mode);

	char aux[40];
	int i,c,retorno=0;
	int tam_str=strlen(str);

	
	for(i=0;i<num;i++){

		if(POS>=0){
			if(i==POS){
				str[8]='X';
			}else{
				str[8]=' ';
			}
		}
		putp(exit_attribute_mode);
		switch((mao+i)->modo){
			case MODO_ATK:
			putp(tparm(set_a_foreground, COR_ATK));
			break;
			case MODO_DEF:
			putp(tparm(set_a_foreground, COR_DEF));
			break;
			case MODO_USADA:
			putp(tparm(set_a_foreground, COR_USADA));
			break;
			case MODO_LIVRE:
			putp(tparm(set_a_foreground, COR_LIVRE));
		}

		if(!i){
			for(c=0;c<espaco;c++){
				printf(" ");
				retorno++;
			}
		}		

		for(c=0;c<tam_str/2;c++){
			if(str[c]==' ' && str[c+1]=='%'){
				strcpy(aux,(mao+i)->nome);
				aux[13]='\0';
				break;
			}
			else{
				

				if(str[c]=='A' || str[c]=='a'){
					sprintf(aux,"%i",(mao+i)->atk);
					if((mao+i)->modo == MODO_ATK)
						putp(enter_bold_mode);

					break;
				}
			
				else{
					if(str[c]=='D' || str[c]=='d'){
						sprintf(aux,"%i",(mao+i)->def);
						if((mao+i)->modo == MODO_DEF)
							putp(enter_bold_mode);
						break;
					}
				
				}
			}
		}

		printf(str,aux);

		for(c=0;c<espaco;c++){
			printf(" ");
			retorno++;
		}
		
		retorno+=tam_str;
	}

	putp(exit_attribute_mode);

	return retorno;
}