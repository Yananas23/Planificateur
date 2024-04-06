opt = -c -Wall -Wextra

main:	planificateur.o
		gcc -o planificateur planificateur.o

planificateur.o: planificateur.c
		gcc ${opt} planificateur.c

all: main clean

clean:
		rm *.o