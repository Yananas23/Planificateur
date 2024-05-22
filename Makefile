opt = -c -Wall -Wextra

main:	planificateur.o reader.o log.o
		gcc -o planificateur planificateur.o reader.o log.o

planificateur.o: planificateur.c
		gcc ${opt} planificateur.c

reader.o: reader.c
		gcc ${opt} reader.c

log.o: log.c
		gcc ${opt} log.c

all: main clean

clean:
		rm *.o