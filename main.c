/*  Nome do programa:       A.T.P.D-C.A.R.D
	Autor: João Gabriel Silva Fernandes
	Data de criação: 15/nov/2016
	Versão atual: 1.3
*/

#if defined (_WIN32)||defined(__WINDOWS__)|| defined(_WIN64)												//para futuros usos em compilação no windows
	#include <conio.h>
	#define "clear" "\"cls\""
	#define sleep Sleep
	#define seg 1000

#elif linux																									//caso seja OS linux
	#include <ncurses.h>
	#include <term.h>                                            
	#include <unistd.h>											
	#define seg 0

#endif

#include "headers/tecla_detect.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

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

void finaliza_programa(void);


typedef struct{
	char nome[25];
	int atk;
	int def;
	short modo;														//por padrao é -1, para indicar mão descupada ( ver função gerar mão)
}Carta;

const Carta CARTA_VAZIA={};

int nlinhas,ncolunas;


char aux_end[]="baralhos/";														//funcoes e variaveis globais para tranformar o aux_end (global) no endereço desejado
void make_ende(char *str){
	strcat(aux_end,str);
}
void rest_ende(){
	aux_end[9]='\0';
}																				//fim das funções para manuseio de endereço

void obter_tela(int *,int *);
void ir_tela(const int,const int);

int listar_baralhos();																						//funções para vizualizar e escolher baralho aleatório

void sys_randdeck();

short imprimir_baralho(char *);

void IA_ordena(Carta *,int);

void IA_escolhecarta(Carta *,Carta *,int *,int *,char *);

void IA_selecionacarta(Carta *,int *,char *);


int gerar_mesa(int ,char ,Carta *,Carta *);

void mostrar_mesa(Carta *,int,Carta *,int,int);

void gerar_mao(Carta *,Carta *,bool *);

int imprime_linha_mesa(int ,char *,Carta *,Carta *);

int imprime_linha_mao(int ,int , char *,Carta *, int);

void mostrar_tela_do_jogo(Carta *,Carta *,Carta *,int ,int,int );

void tela_vitoria(int );


short sys_duelo_start(char *, char *, short);

short sys_duelo(const short);


/*********************************************************************************INICIO INT MAIN************************************************************************************************/


int main(){
	
	int i;
	short menu,mododejogo;
	char nomebaralho[40];
	char temp;

	int posx,posy;

	obter_tela(&nlinhas,&ncolunas);

	if(nlinhas<40 || ncolunas<101){
		printf("ATENÇÃO!!! NUMERO DE LINHAS E/OU COLUNAS INSUFICIENTE. REDIMENSIONE O SEU TERMINAL\nLinhas fornecidas:%i\t\tColunas fornecidas:%i\n\nLinhas minimas: 40\t\tColunas minimas: 101\n",nlinhas,ncolunas);
		printf("\n aumente a tela do terminal\n");
		exit(1);
	}

	//restaura o terminal
	atexit( &finaliza_programa );

	posx=(ncolunas-strlen("Bem vindo ao"))/2;
	ir_tela(0,posx);

	system("clear");

	printf("Bem vindo ao");
	posx=(ncolunas-strlen("A . T . P . D  -  C . A . R . D"))/2;
	ir_tela(2,posx);	
	printf("A . T . P . D  -  C . A . R . D\n");
	posx=(ncolunas-strlen("(Apenas um Trabalho Porém Divertido)"))/2;
	ir_tela(1,posx);
	//system("color 70");
	printf("(Apenas um Trabalho Porém Divertido)\n");

	ler_tecla();	//lê qualquer tecla

	posx=(ncolunas-strlen("A.T.P.D-C.A.R.D"))/2;
	posy=1;


	//system("stty -echo");
	//system("tput civis -- invisible");
	do{
		system("clear");
		ir_tela(posy,posx);
		printf("A.T.P.D-C.A.R.D");
		ir_tela(nlinhas-7,0);																									//6 linhas(opcoes do menu)+ 2 linhas(numero de linhas usadas pelo título)-1 linha para o usuário digitar a opcao;
		printf("1-Novo jogo\n2-Vizualizar baralhos\n3-Abrir editor de baralhos\n4-ler regras\n5-Atualizar A.T.P.D-C.A.R.D\n0-Sair\n");
		menu = ler_tecla()-'0';
		switch(menu){
			case 1:
			system("clear");
			printf("Selecione o modo de jogo:\n1-um jogador\n2-dois jogadores");
			//scanf(" %hi",&mododejogo);
			mododejogo = ler_tecla()-'0';
			sys_duelo(mododejogo);
				
			break;

			case 2:
			system("clear");
			printf("lista de baralhos:\n\n");
			if(listar_baralhos()){
				printf("digite o nome do baralho \n");
				scanf(" %[^\n]s",nomebaralho);
			}
			
			
			if(!imprimir_baralho(nomebaralho)){
				printf("erro ao imprimir baralho\n");
				sleep(1);
			}else{
				getc(stdin);
				ler_tecla();
			}


			break;

			case 3:
			printf("Compilando e executando \"atpd_deck\"\n\n");
			sleep (1*seg);
			system("gcc atpd_deck.c -o atpd_deck && ./atpd_deck");

			break;

			case 4:

			if(system("gedit manual_atpd.txt &")!=0){
				printf("seu sistema nao possui o editor de texto gedit!!\n\nabrindo manual pelo terminal:");
				system("sleep 1.5");
				system("more manual_atpd.txt");

			}
			break;

			case 5:
			printf("Compilando e executando atpd.c\n");
			return system("make && echo \"Atualizado com sucesso.\" ");

			break;

			case 6:
			tela_vitoria(1);
			break;
			case 7:
			printf("%i\n",getppid() );

		}


	}while(menu);


	return 0;

}


/****************************************************************************************************** FIM DO INT MAIN - INICIO DAS FUINÇÕES*************************************************************************************************/


void obter_tela(int *nlinhas,int *ncolunas){
	initscr();
	getmaxyx(stdscr,*nlinhas,*ncolunas);
	endwin();
}
void ir_tela(const int py,const int px){
	int x,y;
	for(y=0;y<py;y++)
		printf("\n");
	for(x=0;x<px;x++)
		printf(" ");
}


int listar_baralhos(){
	int quant,i;
	char nome[40];
	FILE *arq=fopen("baralhos/lista_baralhos","r");
	FILE *info=fopen("baralhos/info","rb");
	if(!arq|| !info){
		return 0;
	}

	fread(&quant,sizeof(int),1,info);
	

	for(i=0;i<quant;i++){
		fscanf(arq,"%[^\n]s",nome);
		fscanf(arq,"\n");
		printf("%s\n",nome );
		if(feof(arq))
			break;
	}

	if(i+1!=quant){
		printf("AVISO!! (quantidade de baralhos existentes!=quantidade informada- OU -baralhos inexistentes )\n");
		printf("quant informada: %i\n",quant );
	}
	if(!quant){
		fclose(arq);
		fclose(info);
		return 0;
	}

	fclose(info);
	fclose(arq);
	return 1;
}

void sys_randdeck(char *str){																					//escolhe um baralho aleatório e retorna um ponteiro para a string com o nome dele
	srand(time(0));
	int quant,i;
	FILE *info=fopen("baralhos/info","rb");
	FILE *arq=fopen("baralhos/lista_baralhos","r");
	if(!arq){
		printf("erro ao selecionar baralho aleatório\n");
	}

	fread(&quant,sizeof(int),1,info);
	quant=1+(rand()%quant);												//gera uma nuemro aleatoria entre 1 - quantidade de baralhos que vai indicar a posicao do baralho escolhido
	for(i=0;i<quant;i++){
		fscanf(arq,"%[^\n]s",str);
		fscanf(arq,"\n");
	}

	fclose(info);
	fclose(arq);
}

short imprimir_baralho(char *nome){
	int i;
	make_ende(nome);
	
	Carta carta;	
	FILE *deck=fopen(aux_end,"rb");
	rest_ende();

	if(!deck)
		return 0;
	system("clear");
	
	for(i=0;i<SIZE_DECK;i++){
		fread(&carta,sizeof(Carta),1,deck);
		printf("\n%-30.30s\t\tataque: %.4i\tdefesa: %.4i\n",carta.nome,carta.atk,carta.def);
	}
	fclose(deck);

	return 1;
}

/*************inicio das funcoes usadas no sistema de batalha************************/



			/**************Funcoes IA******************/

void IA_ordena(Carta *mao,int tam){																			//ok!
	int i,j;
	Carta *aux=(Carta*)malloc(sizeof(Carta));
	for(i=tam-1;i>0;i--){
		for(j=0;j<i;j++){
			if((mao+j)->atk + (mao+j)->def < (mao+j+1)->atk + (mao+j+1)->def){
				*aux=*(mao+j);
				*(mao+j)=*(mao+j+1);
				*(mao+j+1)=*aux;
			}
		}
	}

	free(aux);
}

void IA_escolhecarta(Carta *mesa,Carta *mesainimiga,int *carta_selecionada,int *carta_atacada,char *modo_da_carta){
	srand(time(0));
	int i=0,c,atributo_inimigo;
	int indice_melhor_carta=0;
	bool ataque_direto=true;
	short quantidade_cartas=0;
				
	*carta_selecionada=-1;						

	for(i=0;i<SIZE_MESA;i++){
	
		if((mesa+i)->modo == MODO_ATK){
			if((mesa+i)->atk > (mesa+indice_melhor_carta)->atk){
				indice_melhor_carta=i;
			}
		}
		
		if((mesa+i)->modo != MODO_LIVRE)
			quantidade_cartas++;
						
	}


	if(quantidade_cartas){

		for(c=0;c<SIZE_MESA;c++){
			if((mesainimiga+c)->modo!=MODO_LIVRE){
				ataque_direto=false;
					
				if((mesainimiga+c)->modo == MODO_DEF)
					atributo_inimigo = (mesainimiga+c)->def;
				else
					atributo_inimigo = (mesainimiga+c)->atk;
					

				if((mesa+indice_melhor_carta)->atk >= atributo_inimigo ){
					*carta_selecionada=indice_melhor_carta;
					*carta_atacada=c;
					break;
				}

			}
		}

	}
	
	
	if(*carta_selecionada < 0){
		if(ataque_direto){
			*carta_selecionada= indice_melhor_carta;
			*carta_atacada=0;
		}
		else{
			if(rand()%5 && quantidade_cartas){											//1/5 de chance de ela optar por alterar o modo da carta, apenas se tiver uma quantidade de cartas na mesa maior que 0
				system("sleep 0.1");
				*carta_selecionada=rand()%quantidade_cartas;
				if(mesa[*carta_selecionada].modo != MODO_LIVRE){
					if(mesa[*carta_selecionada].modo == MODO_ATK)
						*modo_da_carta='d';
					else
						*modo_da_carta='a';
				}

			}
		}
	}
	

}

void IA_selecionacarta(Carta *mao,int *carta_selecionada,char *modo_carta){							//usar quicksort aqui para colocar as cartas da mão em ordem de maior poder ( função comparar recebe o struct)
	srand(time(0));
	int i;																					

	*carta_selecionada=rand();																//escolhe o indice aleatóriamente de uma das 3 melhores cartas
	*carta_selecionada%=(SIZE_MAO-2);

	IA_ordena(mao,SIZE_MAO);																	//ordena a mao em ordem crescente de melhores cartas - ok!
	
	if(mao[*carta_selecionada].modo!=MODO_LIVRE){
		if(mao[*carta_selecionada].atk >= mao[*carta_selecionada].def){
			*modo_carta='a';													//modo ataque
		}
		else
			*modo_carta='d';													//modo defesa
	}
	else{													//caso a posicao da carta selecionada esteja vazia, ira automaticamente para a melhor carta da mão (garantindo jogar alguma carta caso exista em mao)
		*carta_selecionada=0;
		if(mao[*carta_selecionada].atk >= mao[*carta_selecionada].def){
			*modo_carta='a';													//modo ataque
		}
		else
			*modo_carta='d';
	}
	system("clear");
	
}




			/*****************Funcoes da vizualização do jogo********************/

int imprime_linha_mesa(int espaco,char *str,Carta *mesa1,Carta *mesa2){

	char **aux=(char**)malloc(SIZE_MESA*sizeof(char*));

	int i,c,retorno=0;
	int tam_str=strlen(str);

	short carta_num=0;

	for(i=0;i<SIZE_MESA;i++){								//alocando memoria para os atributos das cartas
		*(aux+i)=(char*)malloc(40*sizeof(char));
		if(!*(aux+i)){
			printf("erro ao alocar memoria- func imprime_linha_mesa\n");
			sleep(1);
		}
	}

	for(i=0;i<espaco;i++){
		printf(" ");
		retorno++;
	}


	for(i=0;i<strlen(str);i++){
		if(str[i]=='|'){
			if(str[i+1]=='a'){
				for(c=0;c<SIZE_MESA;c++)
					sprintf(*(aux+c),"%i",(mesa2+c)->atk);
				
				break;
				
			}
			if(str[i+1]=='d'){
				for(c=0;c<SIZE_MESA;c++){
					sprintf(*(aux+c),"%i",(mesa2+c)->def);
				}
				break;
			}
		}
		else{

			if(str[i]==' ' && str[i+1]=='%'){
				for(c=0;c<SIZE_MESA;c++){
					strcpy(aux[c],(mesa1+c)->nome);
					aux[c][7]='\0';
				}
				break;
					
			}
				else{

					if(str[i]=='A' || str[i]=='a'){
						for(c=0;c<SIZE_MESA;c++)
							sprintf(aux[c],"%i",(mesa1+c)->atk);
						
						
					}
			
					else{
						if(str[i]=='D' || str[i]=='d'){
							for(c=0;c<SIZE_MESA;c++)
								sprintf(aux[c],"%i",(mesa1+c)->def);
							
							break;
						}
				
					}
				}

		
		}
	}


	printf(str,aux[0],aux[1],aux[2]);

	retorno+=tam_str;

	for(i=0;i<SIZE_MESA;i++)
		free(*(aux+i));

	free(aux);

	return retorno;

}



void mostrar_tela_do_jogo(Carta *mao1,Carta *mesa1,Carta *mesa2,int hp1,int hp2, int POS){												//imprime na tela as cartas da mao e da mesa ( não tente entender, muito complexo)
	int i,c;
	int posicao_carta=0,flag=0;
	int quant_linhas_carta=0,quant_linhas_mesa=0;
	char str_aux[100];


	FILE *modelo_carta=fopen("arquivos/modelo_carta","r");
	FILE *modelo_mesa=fopen("arquivos/modelo_mesa","r");

	if(!modelo_carta || !modelo_mesa){
		printf("ERRO AO LER MODELO DE CARTA\n");
		strcpy(str_aux,"erro");
		sleep(1);
	}

	while(!feof(modelo_carta)){														//ok!!!
		fscanf(modelo_carta,"%[^\n]s",str_aux);
		fscanf(modelo_carta,"\n");
		quant_linhas_carta++;
	}

	while(!feof(modelo_mesa)){														//ok!!!
		fscanf(modelo_mesa,"%[^\n]s",str_aux);
		fscanf(modelo_mesa,"\n");
		quant_linhas_mesa++;
	}
	
	rewind(modelo_mesa);
	rewind(modelo_carta);
	system("clear");

	//printf("HP : %i\nmao:\n",hp1);

/*
	for(i=0;i<SIZE_MAO;i++){
		printf(" %i-nome: %s \t ataque: %i   defesa: %i\n",i,(mao1+i)->nome,(mao1+i)->atk,(mao1+i)->def);
		printf("POS: %i\n",POS );
	}
	printf("\n\n");*/


	
	for(i=0;i<nlinhas-1;i++){
		for(c=0;c<ncolunas;c++){
			if(!i && (!c || c==ncolunas-1))
				printf(" ");
			else{

				if((c==0 || c==ncolunas-1) && i>0)
						printf("|");
				
				else{
					if(i==0 || i==nlinhas-2)
						printf("_");
					else{																							//ocorre aqui ( neste else exclusivamente ) a impressao das cartas/preenchimento de espaco em branco
						
							if(i==1 && c==(ncolunas-2)-strlen("HP:xxxx")){
								printf("HP:%-4i",hp2);
								c+=strlen("HP:xxxx");
							}
							if(i+1==(nlinhas-2)-quant_linhas_carta && c==1){
								printf("HP:%-4i",hp1 );
								c+=strlen("HP:xxxx");
							}

						
						if(i>=(nlinhas-2)-quant_linhas_carta){
						
							if(!feof(modelo_carta)){
								fscanf(modelo_carta,"%[^\n]s",str_aux);
								fscanf(modelo_carta,"\n");					
							}
							else
								strcpy(str_aux," ");
							
							//printf("%i",flag );
							flag++;
							if(flag==6)
								c+=imprime_linha_mao(((ncolunas-2) - 5*(strlen(str_aux)))/6,SIZE_MAO,str_aux,mao1,POS);
							else
								c+=imprime_linha_mao(((ncolunas-2) - 5*(strlen(str_aux)))/6,SIZE_MAO,str_aux,mao1,INIT_TELA_STECLA);


							for(c;c<ncolunas-1;c++)
								printf(" ");

							printf("|");	
						}
						else{											//ocorre aqui a impressao da mesa no meio da tela
							if(i>=(nlinhas-2-(quant_linhas_carta+quant_linhas_mesa))/2 +1){						//condicional para saber se ja esta na linha que permitira a mesa ficar no meio de tudo											

								if(!feof(modelo_mesa)){
									fscanf(modelo_mesa,"%[^\n]s",str_aux);
									fscanf(modelo_mesa,"\n");

									c+=imprime_linha_mesa(((ncolunas-2)-strlen(str_aux))/2,str_aux,mesa1,mesa2);
									for(c;c<ncolunas-1;c++)
										printf(" ");

									printf("|");	

								}
								else
								printf(" ");
							
							}
							else
								printf(" ");
							


						}					
					}
				}
			}
		}
	
	} 

	fclose(modelo_mesa);
	fclose(modelo_carta);
	
}


void mostrar_mesa(Carta *mesa1,int hp1,Carta *mesa2,int hp2,int carta_selecionada){
	int i,c;
	char beirada[2]={'\\','/'};
	char str_aux[50];
	int num_linhas_carta=0;

	FILE *modelo_carta=fopen("arquivos/modelo_carta","r");

	if(!modelo_carta){
		printf("ERRO AO ABRIR REFERENCIA/MODELO DE CARTA\n");
		strcpy(str_aux,"ERRO!");
		sleep(1);
	}

	while(!feof(modelo_carta)){
		fscanf(modelo_carta,"%[^\n]s",str_aux);
		fscanf(modelo_carta,"\n");
		num_linhas_carta++;
	}

	rewind(modelo_carta);


	for(i=0;i<nlinhas-2;i++){
		for(c=0;c<ncolunas;c++){
			if(!i){
				if(c<=2 || c>=ncolunas-3)
					printf(" ");
				else
					printf("_");
			}
			else{

				if(i==nlinhas-3){
					
					if(!c || c==ncolunas-1)
						printf(" ");
					else
						if(c==1)
							printf("\\");
						else
							if(c==2 || c==ncolunas-3)
								printf("|");
							else
								if(c==ncolunas-2)
									printf("/");
								else
									printf("_");
					
				}
				else{

					if(!c || c==ncolunas-1 || c==2 || c==ncolunas-3){
						if((i==1 || i==nlinhas-1) && (!c ||c==ncolunas-1 ) )
							printf(" ");
						else
							printf("|");
					}
					else{
						if(c==1 || c==ncolunas-2){
							if(c==1)
								printf("%c",beirada[i%2]);
							else
								printf("%c",beirada[(i+1)%2] );
						}
						else{
							//imprimindo mesa inimiga
							if(i>=((((nlinhas-4))-2*num_linhas_carta))/4 && i<((nlinhas-4)/2-((((nlinhas-4))-2*num_linhas_carta))/4 ) && !feof(modelo_carta)) {
								
									fscanf(modelo_carta,"%[^\n]s",str_aux);
									fscanf(modelo_carta,"\n");
									c+=imprime_linha_mao(((ncolunas-6)-SIZE_MESA*(strlen(str_aux)))/4,SIZE_MESA,str_aux,mesa2,-1);
								
								
								for(c;c<ncolunas-3;c++)
									printf(" ");
								printf("|%c|",beirada[(i+1)%2] );
								c+=3;
							}
							//imprimindo sua mesa
							else{
								if(i+num_linhas_carta ==(nlinhas-4)-((((nlinhas-4))-2*num_linhas_carta))/4 )
									rewind(modelo_carta);

								if(i+num_linhas_carta > (nlinhas-4)-((((nlinhas-4))-2*num_linhas_carta))/4  && i<nlinhas-4){
									if(!feof(modelo_carta)){

										fscanf(modelo_carta,"%[^\n]s",str_aux);
										fscanf(modelo_carta,"\n");
										c+=imprime_linha_mao(((ncolunas-6)-SIZE_MESA*(strlen(str_aux)))/4,SIZE_MESA,str_aux,mesa1,-1);

										for(c;c<ncolunas-3;c++)
											printf(" ");
										
										printf("|%c|",beirada[(i+1)%2] );
										c+=3;
									}
									else{
										if(carta_selecionada >= 0 && carta_selecionada < SIZE_MESA){
											for(c;c-3 < ( ((ncolunas-6)-SIZE_MESA*(strlen(str_aux)))/4 )*(carta_selecionada+1)  + (carta_selecionada)*strlen(str_aux) +strlen(str_aux)/2 ;c++)
												printf(" ");		

											printf("^");
											c++;

										}
										for(c;c<ncolunas-3;c++)
											printf(" ");
										
										printf("|%c|",beirada[(i+1)%2] );
										c+=3;

									}
								}
								else{
									if(i==1 && c==ncolunas-(strlen("HP:xxxx")+3)){
											printf("HP:%4i",hp2 );
											c+=strlen("HP:xxxx")-1;
									}
									else{
										if(i==nlinhas-4 && c==3){
											printf("HP:%4i",hp1 );
											c+=strlen("HP:xxxx")-1;
										}
										else{
											if(i==(nlinhas-3)/2)
												printf("=");
											else{
											printf(" ");
											}
										}
									}											
																								
								}
							}
						}
					}

				}

			}
		}
	}

	fclose(modelo_carta);
}





			/***************Funcoes realcionadas ao manuseio e controle das cartas e da batalha****************/

int gerar_mesa(int carta,char modo,Carta *mao,Carta *mesa){
	int c,retorno=0;

	if(!(carta<SIZE_MAO && carta>=0)){
		printf("NENHUMA CARTA SELCIONADA!\n");
		return 1;
	}

	if(mao[carta].modo==MODO_LIVRE){
		printf("NENHUMA CARTA SELCIONADA!\n");
		return 1;
	}
	
		for(c=0;c<SIZE_MESA;c++){
			if(mesa[c].modo==MODO_LIVRE){
				mesa[c]=mao[carta];
				if(modo=='d' || modo=='D')
					mesa[c].modo=MODO_DEF;
				else
					mesa[c].modo=MODO_ATK;

				return 1;			
			}
		}

	return 0;
	
		
}


void tela_vitoria(int jog){
	FILE *modelo_vitoria=fopen("arquivos/modelo_vitoria","r");

	int i,num_linhas=0,num_colunas;
	char aux[100];

	while(!feof(modelo_vitoria)){
		fscanf(modelo_vitoria,"%[^\n]s",aux);
		if(!num_linhas)
			num_colunas=strlen(aux);					//pega o tamanho da primeira string da tela de vitoria e a toma como universal

		fscanf(modelo_vitoria,"\n");
		num_linhas++;
	}


	setupterm(NULL, STDOUT_FILENO, NULL);
	putp(exit_attribute_mode);
  	putp(enter_bold_mode);


  	for(i=0;i<max_colors;i++){
  		system("clear");
		rewind(modelo_vitoria);

		ir_tela((nlinhas-num_linhas)/2,(ncolunas-num_colunas)/2);
		putp(tparm(set_a_foreground, i));

		while((aux[0]=fgetc(modelo_vitoria))!=EOF ){			
			printf("%c",aux[0]);

			if(aux[0]=='\n')
				ir_tela(0,(ncolunas-num_colunas)/2);
			
			
		}
		system("sleep 0.05");

	}

	system("sleep 1");

	putp(exit_attribute_mode);
	fclose(modelo_vitoria);
	
}


short sys_duelo(const short modo){																			//função 1 para batalha: escolher baralhos-----retornos: -1(erro ao abrir baralhos),-2(erro ao listar baralhos)
	
	char nomebaralho1[40],nomebaralho2[40];																	// 1(vitoria jog1), 2(vitoria jog2)
	int i;

	system("clear");
	printf("Lista de baralhos disponíveis:\n\n");
	if(!listar_baralhos()){
		printf("nao foi possivel listar os baralhos\n");
		return -2;
	}
	printf("aleatorio\n" );

	if(modo==1)
		sys_randdeck(nomebaralho2);

	
	switch(modo){
		case 1:
		printf("\nDigite o nome do baralho a ser usado\n");
		scanf(" %[^\n]s",nomebaralho1);
		if(strcmp(nomebaralho1,"aleatorio")==0)
			sys_randdeck(nomebaralho1);

		break;
		case 2:
		printf("Jogador 1 digite o nome do baralho que deseja usar:\t");
		scanf(" %[^\n]s",nomebaralho1);
		if(strcmp(nomebaralho1,"aleatorio")==0)
			sys_randdeck(nomebaralho1);

		printf("Jogador 2 digite o nome do baralho que deseja usar:\t");
		scanf(" %[^\n]s",nomebaralho2);

		if(strcmp(nomebaralho2,"aleatorio")==0)
			sys_randdeck(nomebaralho2);

		break;
	}
	system("clear");
	sleep(seg*1);



	switch(sys_duelo_start(nomebaralho1,nomebaralho2,modo)){												//inicia a batalha
		case 0:
		printf("erro ao abrir baralhos!!\n");
		sleep(1*seg);
		return -1;
		break;

		case 1:
		tela_vitoria(1);
		return 1;
		
		break;

		case 2:
		tela_vitoria(2);
		return 2;

	}

	getc(stdin);
	getc(stdin);

}

void gerar_mao(Carta *mao,Carta *deck,bool *cartas_usadas){		// FICA NUM LOOP INFINITO CASO TENHA USADO TODAS AS CARTAS DO DECK
	srand(time(0));
	int i,num;
	for(i=0;i<SIZE_MAO;i++){
		if((mao[i].modo) == MODO_LIVRE){																	
			do{
				num=rand();
				num%=SIZE_DECK;
				if(!cartas_usadas[num])
					mao[i]=deck[num];

			}while(cartas_usadas[num]);
			mao[i].modo= MODO_USADA;
			cartas_usadas[num]=true;
		}
			
	}
}

void finaliza_programa(void){
	printf("FINALIZANDO...\n");
	endwin();
	system("tput cnorm -- normal");
	system("stty echo");
	printf("PRONTO\n");
}
