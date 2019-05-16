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

#include "../headers/tecla_detect.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


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



extern struct termios old,new;

typedef struct{
	char nome[25];
	int atk;
	int def;
	short modo;														//por padrao é -1, para indicar mão descupada ( ver função gerar mão)
}Carta;


extern struct termios old,new;
extern char aux_end[];
void make_ende(char *str);
void rest_ende();

extern int nlinhas, ncolunas;
extern const Carta CARTA_VAZIA;

void obter_tela(int *,int *);
void ir_tela(const int,const int);



void IA_selecionacarta(Carta *,int *,char *);
void IA_escolhecarta(Carta *,Carta *,int *,int *,char *);



void gerar_mao(Carta *,Carta *,bool *);


void mostrar_tela_do_jogo(Carta *,Carta *,Carta *,int ,int, int );
int gerar_mesa(int ,char ,Carta *,Carta *);

void mostrar_mesa(Carta *,int,Carta *,int,int);



short sys_duelo_start(char *nomebaralho1,char *nomebaralho2,short quant_jogadores){
	srand(time(0));

	make_ende(nomebaralho1);														//transforma o endereço para o desejado do baralho escolhido
	FILE *arq_d1=fopen(aux_end,"rb");
	rest_ende();

	make_ende(nomebaralho2);														//transforma o endereço para o desejado do baralho escolhido
	FILE *arq_d2=fopen(aux_end,"rb");
	rest_ende();

	int i,c,jog,rodada=0,num_jogadas;
	short vitoria=0,jogadas_max=0;

	printf("baralhos selecionados:\njogador 1: %s\njogador 2: %s\n",nomebaralho1,nomebaralho2);
	sleep(2*seg);

	if(!(arq_d1 && arq_d2))
		return 0;

	sleep(2*seg);
	system("clear");


/**** começa aqui o inicio da preparação para a batalha ( declaração das variaveis usadas para tal, iniciar baralho, iniciar mao, etc )***/

	int temp_atributo[2],temp_DiferecaDeHP;
	int TECLA,pos,flag_frames;

	int HP[2]={SIZE_HP,SIZE_HP};
	int carta_selecionada[2];
	int carta_atacada;
	char modo_da_carta[2];														//modo da carta (atk ou def)
																				//carregando as cartas de ambos baralhos
	Carta **deck;														//dois vetores de estruturas que vão armazenar os baralhos
	deck=(Carta**)malloc(2*sizeof(Carta*));
	if(!deck){
		printf("ERRO AO ALOCAR MEMORIA\n");
		exit(1);
	}
	for(i=0;i<2;i++){
		*(deck+i)=(Carta*)calloc(SIZE_DECK,sizeof(Carta));
		if(!*(deck+i)){
			printf("ERRO AO ALOCAR MEMORIA\n");
			exit(1);
		}
		
	}												
	bool cartas_usadas[2][SIZE_DECK]={};									//vetores para mostrar de as cartas estão disponiveis no baralho ou não

	fread(*(deck),sizeof(Carta),SIZE_DECK,arq_d1);
	fread(*(deck+1),sizeof(Carta),SIZE_DECK,arq_d2);
	fclose(arq_d1);
	fclose(arq_d2);															//fim de tal carregamento

	
	Carta **mesa;														//dois vetores de estruturas que vão armazenar as cartas na mesa, iniciados com 0 (isto é, vazios)
	mesa=(Carta**)malloc(2*sizeof(Carta*));
	if(!mesa){
		printf("ERRO AO ALOCAR MEMORIA\n");
		exit(1);
	}
	for(i=0;i<2;i++){
		*(mesa+i)=(Carta*)calloc(SIZE_MESA,sizeof(Carta));
		if(!*(mesa+i)){
			printf("ERRO AO ALOCAR MEMORIA\n");
			exit(1);
		}
		
	}

	Carta **mao;														//dois vetores de estruturas que vão armazenar as cartas na mão
	
	mao=(Carta**)malloc(2*sizeof(Carta*));													//alocando memória para as cartas da mão de forma que iniciem com o modo valor 0 ==MODO_LIVRE
	if(!mao){
		printf("ERRO AO ALOCAR MEMORIA\n");
		exit(1);
	}

	for(i=0;i<2;i++){
		*(mao+i)=(Carta*)calloc(SIZE_MAO,sizeof(Carta));
		if(!*(mao+i)){
			printf("ERRO AO ALOCAR MEMORIA\n");
			exit(1);
		}
	}


			getc(stdin);

/************************************************inicio da batalha*******************************************************************/


	do{
		if(rodada<=SIZE_DECK-5){
			gerar_mao(mao[0],deck[0],cartas_usadas[0]);
			gerar_mao(mao[1],deck[1],cartas_usadas[1]);																}

		for(jog=0;jog < 2;jog++){
			

			if(quant_jogadores==2 || jog==0){		//se for a vez de um player fisico/real
				flag_frames=true;
				modo_da_carta[jog]=' ';
				pos=2;
				mostrar_tela_do_jogo(*(mao+jog),mesa[jog],mesa[(jog+1)%2],HP[jog],HP[(jog+1)%2],INIT_TELA_STECLA);							//Inicia a tela do jogo sem uma posição selecionada pela tecla
				do{
					switch(ler_tecla()){
						case DIR:
						if(pos<5)
							pos++;
						break;

						case ESQ:
						if(pos>0)
							pos--;
						break;

						case CIMA:
						modo_da_carta[jog]='a';
						break;

						case BAIXO:
						modo_da_carta[jog]='d';
						break;

						case DEL_KEY:
						(*(mao+jog)+pos)->modo = MODO_LIVRE;
						break;

						case 'x':
						modo_da_carta[jog]='x';
						break;

						case ESC_KEY:
						flag_frames=false;
						break;

						case '\n':
						flag_frames=false;
						if(modo_da_carta[jog]=='x'){
							(*(mao+jog)+pos)->modo = MODO_LIVRE;
							flag_frames=true;
						}
					}
					mostrar_tela_do_jogo(*(mao+jog),mesa[jog],mesa[(jog+1)%2],HP[jog],HP[(jog+1)%2],pos);							
					printf("\nJogador %i,\tselecione a carta e seu modo:\t\tCARTA: %i\t\tMODO: %c\t\t",jog+1,pos+1,modo_da_carta[jog]);
				
					*(carta_selecionada+jog)=pos;
				
				}while(flag_frames);
			}
			else{
				IA_selecionacarta(*(mao+jog),carta_selecionada+jog,modo_da_carta+jog);
			}

			system("clear");
			ir_tela(nlinhas/2,ncolunas/2-strlen("carta selecionada por jogador %i:"));
			printf("carta selecionada por jogador %i: %s\t\t%i/%i\n",jog+1,mao[jog][*(carta_selecionada+jog)].nome,mao[jog][*(carta_selecionada+jog)].atk,mao[jog][*(carta_selecionada+jog)].def);
			ir_tela(0,ncolunas/2-strlen("modo da carta:%c"));
			printf("modo da carta:%c\n",*(modo_da_carta+jog) );
			getc(stdin);
			system("clear");

			
			
				if(gerar_mesa(carta_selecionada[jog],modo_da_carta[jog],mao[jog],mesa[jog])){				//gera/atualiza a nova mesa de acordo com a carta escolhida da mão
					(*(mao+jog)+carta_selecionada[jog])->modo = MODO_LIVRE;				//altera o valor da mão da carta selecionada para livre					
				}
				else{
					printf("MESA CHEIA!\n");
					getc(stdin);
				}
			


			/****************aplicando jogadas - inicio da rodada*********************/

			if(rodada){
				num_jogadas=0;																						//condicional para primeira rodada ( nao é permitido atacar logo na primeira rodada);
				for(i=0;i<SIZE_MESA;i++){
					if(mesa[jog][i].modo == MODO_LIVRE)
						break;
				}

				jogadas_max=i;

				while(num_jogadas < jogadas_max){
					modo_da_carta[jog]=0;

					
					printf("\n");
					if(quant_jogadores==2 || !jog){
						pos=2;
						flag_frames=true;
						modo_da_carta[jog]=' ';
						mostrar_mesa(mesa[jog],HP[0],mesa[(jog+1)%2],HP[1],-1);
						printf("numero de jogadas: %i\n",jogadas_max-num_jogadas );
						do{
							switch(ler_tecla()){
								case DIR:
								if(pos < SIZE_MESA-1)
									pos++;
								break;

								case ESQ:
								if(pos > 0)
									pos--;
								break;

								case CIMA:
								modo_da_carta[jog]='a';
								mesa[jog][*carta_selecionada].modo=MODO_ATK;
								break;

								case BAIXO:
								modo_da_carta[jog]='d';
								mesa[jog][*carta_selecionada].modo=MODO_DEF;
								break;

								case ESC_KEY:
								modo_da_carta[jog]='-';
								flag_frames=false;
								break;

								case DEL_KEY:	//tecla del
								//num_jogadas++;
								mesa[jog][pos]=CARTA_VAZIA;
								break;

								case 'x':
								modo_da_carta[jog]='x';
								break;

								case '\n':
								flag_frames=false;
								if(modo_da_carta[jog]=='x'){
									flag_frames=true;
									//num_jogadas++;
									mesa[jog][pos]=CARTA_VAZIA;
								}
								break;
							}
							system("clear");
							mostrar_mesa(mesa[jog],HP[0],mesa[(jog+1)%2],HP[1],pos);							
							printf("numero de jogadas: %i",jogadas_max-num_jogadas );
							printf("\nJogador %i,\tselecione a carta e seu modo:\t\tCARTA: %i\t\tMODO: %c\t",jog+1,pos+1,modo_da_carta[jog]);
							fflush(stdout);
							*(carta_selecionada)=pos;
			
						}while(flag_frames);
						system("clear");
						//printf("\njogador %i, escolha qual carta quer usar\t(0 ou menos para finalizar a jogada):  ",jog+1 );

					}			
					else{
						IA_escolhecarta(mesa[1],mesa[0],carta_selecionada,&carta_atacada,modo_da_carta+jog);
					}
						
						
				


					/*if(modo_da_carta[jog]=='a')
						mesa[jog][*carta_selecionada].modo=MODO_ATK;

					if(modo_da_carta[jog]=='d' )
						mesa[jog][*carta_selecionada].modo=MODO_DEF;
					
					if(modo_da_carta[jog]=='x')
						mesa[jog][*carta_selecionada]=CARTA_VAZIA;
					*/

					if(*carta_selecionada >= 0 && modo_da_carta[jog]=='a'){

						do{
							system("clear");
							mostrar_mesa(mesa[jog],HP[0],mesa[(jog+1)%2],HP[1], *carta_selecionada );
							printf("Escolha o numero de qual carta quer atacar");
							if(quant_jogadores==2 || jog==0){
								scanf(" %i",&carta_atacada);
								carta_atacada--;
							}

							if(carta_atacada>2 || carta_atacada<0)
								printf("Escolha uma carta de 1 a 3!\t\t");

						}while(carta_atacada>2 || carta_atacada<0);
					

						temp_atributo[jog]= (*(mesa+jog)+*carta_selecionada) ->atk;

						if( ((*(mesa+((jog+1)%2)))+carta_atacada)->modo == MODO_ATK){									//caso a carta atacada esteja em modo ataque
							temp_atributo[(jog+1)%2]= (*(mesa+((jog+1)%2))+carta_atacada)->atk;

							temp_DiferecaDeHP= temp_atributo[(jog+1)%2] - temp_atributo[jog];						//armazena a diferença de atributo desta forma: carta atacada-carta que atacou

							if(temp_DiferecaDeHP<0){									//carta sua vence inimiga
								HP[(jog+1)%2]+=temp_DiferecaDeHP;
								mesa[(jog+1)%2][carta_atacada] = CARTA_VAZIA;
								system("clear");
								ir_tela(nlinhas/2,ncolunas/2 -strlen("A CARTA DO JOGADOR x FOI DESTRUÍDA!")/2);
								printf("A CARTA DO JOGADOR %i FOI DESTRUÍDA!\n",((jog+1)%2)+1);
								system("sleep 2");
							}
							else{
								if(temp_DiferecaDeHP>0){								//carta inimiga vence a sua
									HP[jog]-=temp_DiferecaDeHP;
									mesa[jog][*carta_selecionada] = CARTA_VAZIA;
									system("clear");
									ir_tela(nlinhas/2,ncolunas/2 -strlen("A CARTA DO JOGADOR x FOI DESTRUÍDA!")/2);
									printf("A CARTA DO JOGADOR %i FOI DESTRUÍDA!\n",jog+1);
									system("sleep 2");
								}
								else{ 												//empate e ambas sao destruidas
									mesa[jog][*carta_selecionada] = CARTA_VAZIA;
									mesa[(jog+1)%2][carta_atacada] = CARTA_VAZIA;
									system("clear");
									ir_tela(nlinhas/2,ncolunas/2 -strlen("AMBAS CARTAS FORAM DESTRUÍDAS!")/2);
									printf("AMBAS CARTAS FORAM DESTRUÍDAS!\n");
									system("sleep 2");

								}
							}


						}
						else{
							if( ((*(mesa+((jog+1)%2)))+carta_atacada)->modo == MODO_DEF){																					//caso a carta atacada esteja em modo defesa
								temp_atributo[(jog+1)%2]= (*(mesa+((jog+1)%2))+carta_atacada)->def;

								temp_DiferecaDeHP= temp_atributo[(jog+1)%2] - temp_atributo[jog];

								if(temp_DiferecaDeHP<0){									//Carta inimiga derrotada
									mesa[(jog+1)%2][carta_atacada] = CARTA_VAZIA;
									system("clear");
									ir_tela(nlinhas/2,ncolunas/2 -strlen("A CARTA DO JOGADOR x FOI DESTRUÍDA!")/2);
									printf("A CARTA DO JOGADOR %i FOI DESTRUÍDA!\n",((jog+1)%2)+1);
									system("sleep 2");
								}
								else{
									if(temp_DiferecaDeHP>0){							//sua carta derrotada
										HP[jog]-=temp_DiferecaDeHP;
										mesa[jog][*carta_selecionada] = CARTA_VAZIA;
										system("clear");
										ir_tela(nlinhas/2,ncolunas/2 -strlen("A CARTA DO JOGADOR x FOI DESTRUÍDA!")/2);
										printf("A CARTA DO JOGADOR %i FOI DESTRUÍDA!\n",jog+1);
										system("sleep 2");
									}
								}
							}
							else{
								for(i=0;i<SIZE_MESA;i++){
									if( ((*(mesa+((jog+1)%2)))+i)->modo != MODO_LIVRE )
										break;

								}
								if(i==3)
									HP[(jog+1)%2]-=temp_atributo[jog];
							}

						}
						
					}
					if(modo_da_carta[jog]=='-'){
						num_jogadas=jogadas_max;							//como optou por não fazer mais nenhuma jogada, finaliza aqui o direito de jogar mais de uma vez
					}

					if(HP[(jog+1)%2]<=0)						//checa se alguem ja venceu
						break;
					
					num_jogadas++;
				}
			}
			if(jog==0 || quant_jogadores==2){						//mostra a tela ao final da rodada apenas para jogadores humanos
				printf("\n");
				mostrar_mesa(mesa[jog],HP[0],mesa[(jog+1)%2],HP[1],-1);
				printf("\n");
				system("sleep 1.7");
			}
			
			rodada++;

			if(HP[(jog+1)%2]<=0){						//checa vitoria do jogador atual
				vitoria=jog+1;
				break;
			}
			else{
				if(HP[jog]<=0){
					vitoria=((jog+1)%2)+1;
					break;
				}
			}

		}

		/************************fim da rodada***************************/

	}while(!vitoria);


	for(i=0;i<2;i++){
		free(*(mesa+i));
		free(*(mao+i));
	}
	free(mesa);
	free(mao);
	
	return vitoria;

}