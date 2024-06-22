exe: main.o level.o joueur.o menu.o gameover.o
	gcc main.o level.o joueur.o menu.o gameover.o -o exe -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2

main.o: main.c header.h
	gcc -Wall -c main.c -o main.o

level.o: level.c header.h
	gcc -Wall -c level.c -o level.o

joueur.o: joueur.c header.h
	gcc -Wall -c joueur.c -o joueur.o

menu.o: menu.c header.h
	gcc -Wall -c menu.c -o menu.o
	
gameover.o: gameover.c header.h
	gcc -Wall -c gameover.c -o gameover.o
