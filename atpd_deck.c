/*************************************************CRIADOR DE BARALHOS PARA ATPD-CARD v-1.2********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE_DECK 30


typedef struct{
	char nome[25];
	int atk;
	int def;
	short modo;
}Carta;


char aux_end[]="baralhos/";	

int listar_baralhos();

void make_ende(char *str);

void rest_ende();

int deletar(char *);

int deletar_todos(int);

int apagar_da_lista(char *str){
	char nome[50],aux[50];
	int i,c;

	FILE *arq=fopen("baralhos/lista_baralhos","r");
	FILE *arq_aux=fopen("baralhos/lista_baralhos_aux","w+");

	if(!arq || !arq_aux){
		printf("erro ao abrir arquivos(func apagar_da_lista\n");
		return 0;
	}

	while(!feof(arq)){
		fscanf(arq,"%[^\n]s",aux);
		if(strcmp(nome,aux)!=0){
			fprintf(arq_aux, "%s\n",aux);
			printf("na linha %i de nome %s\n",i,nome );
		}
		fscanf(arq,"\n");
		i++;
	}
	fclose(arq);
	rewind(arq_aux);
	arq=fopen("baralhos/lista_baralhos","w");
	if(!arq_aux){
		printf("erro ao abrir arquivos(func apagar_da_lista\n");
		return 0;
	}
	for(c=0;c<i;c++){
		fscanf(arq_aux,"%[^\n]s",aux);
		fprintf(arq,"%s\n",aux);
		fscanf(arq_aux,"\n");
	}
	fclose(arq_aux);
	fclose(arq);
	return 1;
}


int main(){

	FILE *deck;
	FILE *arq_lista=fopen("baralhos/lista_baralhos","a"),*info=fopen("baralhos/info","rb");

	if(!(info && arq_lista)){
		printf("nao foi possivel abrir a lista de baralhos\n");
		exit(1);
	}

	Carta carta;
	int c;
	int menu,menu2,i,num_baralhos;
	char nome_deck[40];

	
	fread(&num_baralhos,sizeof(int),1,info);
	fclose(info);
	info=fopen("baralhos/info","wb");



	do{
		system("clear");
		printf("Bem vindo ao gerenciador de baralhos A.T.P.D-C.A.R.D v-1.2\n\n");
		printf("1-criar novo baralho\n2-deletar um baralho\n3-deletar todos os baralhos\n4-Outros\n0-sair\n");
		scanf(" %i",&menu);

		switch(menu){
			case 1:
			printf("digite o nome do baralho/deck:\n");
			scanf(" %[^\n]s",nome_deck);
			make_ende(nome_deck);
			deck=fopen(aux_end,"wb");
			rest_ende();
			if(!deck){
				printf("erro ao gerar arquivo do baralho/deck\n");
				exit(1);
			}
			
			for(i=0;i<SIZE_DECK;i++){
				system("clear");
				printf("\tCarta nº%i\n",i+1);
				printf("NOME:\t");
				scanf(" %[^\n]s",carta.nome);
				c=0;
				while(1){
					if(carta.nome[c]!='\0')
						c++;
					else{
						while(carta.nome[c-1]==' '){
							carta.nome[c-1]='\0';
							c--;
						}
						break;
					}
				}
				printf("ATRIBUTO ATAQUE:\t");
				scanf(" %i", &carta.atk);
				printf("ATRIBUTO DEFESA:\t");
				scanf(" %i",&carta.def);
				fwrite(&carta,sizeof(Carta),1,deck);
			}
			num_baralhos++;
			fclose(deck);
			fprintf(arq_lista,"%s\n",nome_deck);
			break;

			case 2:
			system("clear");
			listar_baralhos();
			printf("digite o nome do baralho a ser removido\n");
			scanf(" %[^\n]s",nome_deck);

			if(deletar(nome_deck) && apagar_da_lista(nome_deck)){
				printf("baralho deletado com sucesso!\n");
				num_baralhos--;
			}
			else
				printf("erro ao deletar baralho!\n");
			system("sleep 1");
			
			break;

			case 3:
			if(deletar_todos(num_baralhos)){
				printf("todos os baralhos foram deletados com sucesso!!\n");
				num_baralhos=0;
			}
			system("sleep 1");

			break;

			case 4:
			do{
				system("clear");
				printf("1-resetar/refefinir infos dos baralhos manualmente (altera o dado resposavel pela quantidade de baralhos. \n\nobs1:usar em caso de emergencia\nobs2: recomenda-se apagar manualmente os baralhos que ficarao fora da lista para evitar conflitos\n\n0-voltar\n");
				printf("numero atual de baralhos:%i\n",num_baralhos);

				scanf(" %i",&menu2);
				switch(menu2){
					case 1:
					info=fopen("baralhos/info","wb");
					if(!info){
						printf("erro ao resetar info!\n");
						break;
					}
					printf("digite o novo valor da quantidade de baralhos\n");
					scanf(" %i",&num_baralhos);
					fclose(arq_lista);
					arq_lista=fopen("baralhos/lista_baralhos","w");
					if(!arq_lista){
						printf("erro ao limpar lista!\n");
						break;
					}
					printf("Redefinição feita com sucesso!\n");
					system("sleep 1");

				}

			}while(menu2);
					
		}

		printf("numero total de baralhos:%i\n",num_baralhos);
		system("sleep 1");


	}while(menu);

	fwrite(&num_baralhos,sizeof(int),1,info);

	
	fclose(info);

	fclose(arq_lista);
}




/**********************************FUNCOES********************************************/



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
		printf("AVISO!! (quantidade de baralhos existentes!=quantidade informada, ou baralhos inexistentes)\n");
		printf("quant informada: %i\n",quant );
	}

	fclose(arq);
	fclose(info);
	return 1;
}

void make_ende(char *str){
	strcat(aux_end,str);
}

void rest_ende(){
	aux_end[9]='\0';
}


int deletar(char *str){
	make_ende(str);
	if(remove(aux_end)==0){
		rest_ende();
		return 1;
	}
	else
		return 0;
	
}

int deletar_todos(int quant){
	int i;
	char nome_deck[40];
	FILE *arq_lista=fopen("baralhos/lista_baralhos","r");
	if(!arq_lista)
		return 0;
	
	for(i=0;i<quant && !feof(arq_lista);i++){
		fscanf(arq_lista,"%[^\n]s",nome_deck);
		deletar(nome_deck);
		fscanf(arq_lista,"\n");
	}
	fclose(arq_lista);
	arq_lista=fopen("baralhos/lista_baralhos","w");

	if(!arq_lista)
		return 0;

	fclose(arq_lista);
	return 1;

}