#Makefile to compile KiwiLexer and run it's test shell.

CC = gcc
INC = -I Headers
VPATH = Sourcefiles

TestShell: MemorySrc.o TokenizerSrc.o LexerSrc.o ParserSrc.o
	$(CC) -o TestShell MemorySrc.o TokenizerSrc.o LexerSrc.o ParserSrc.o $(VPATH)/TestShell.c $(INC)

MemorySrc.o: MemorySrc.c 
	$(CC) $(VPATH)/MemorySrc.c $(INC) -c

TokenizerSrc.o: TokenizerSrc.c 
	$(CC) $(VPATH)/TokenizerSrc.c $(INC) -c

LexerSrc.o: LexerSrc.c 
	$(CC) $(VPATH)/LexerSrc.c $(INC) -c

ParserSrc.o: ParserSrc.c 
	$(CC) $(VPATH)/ParserSrc.c $(INC) -c