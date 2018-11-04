all: project2 
project2: main.c init.c assign.c parsing.c traversal.c update.c declarations.h
	gcc -o project2 main.c init.c assign.c parsing.c traversal.c update.c declarations.h -lm -g
clean:
	-rm -f project2