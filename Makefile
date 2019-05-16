#compile all files and main and generate the executable, then erase the binaries (.o) files
default: main.o headers.o libs.o link_all clean



#compile all files and main and generate the executable, keep the binaries
all: main.o headers.o libs.o link_all
	
	
#uses all the binaries files to generate the executable
link_all:
	gcc main.o libs/sys_duelo_start.o libs/imprime_linha_mao.o headers/tecla_detect.o -o atpd.exe -lncurses
	
#generates the main binaries
main.o: main.c
	gcc -c main.c -o main.o

headers.o:
	gcc -c headers/tecla_detect.c -o headers/tecla_detect.o

libs.o:
	gcc -c libs/sys_duelo_start.c -o libs/sys_duelo_start.o
	gcc -c libs/imprime_linha_mao.c -o libs/imprime_linha_mao.o

atpd_deck:
	gcc atpd_deck.c -o deck_editor

#clean all .o files
clean:
	find -name '*.o' -delete
	
erase:
	find -name '*.o' -delete
	find ! -type d -executable -delete
	
