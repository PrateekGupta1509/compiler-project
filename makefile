# PRATEEK GUPTA
# 2014B4A70767P

all: driver.c lexer.o parser.o
	gcc -w driver.c lexer.o parser.o -o stage1exe 
	rm *.o

lexer.o: lexer.c lexer.h
	gcc -c -w lexer.c

parser.o: parser.c parser.h
	gcc -c -w parser.c

clean:
	rm -f stage1exe 