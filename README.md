# KiwiLexer
If worse is better then UNESCO might as well declare this one of the seven wonders. This is a Lexer with two goals:
* Simple, small and readable source code. Everyone should be able to just open a source file and be able to understand it without having to browse the whole repository looking for missing pieces. 
* Memory efficient. The less memory used the better. 

## KiwiLexer in a nutshell.
I have been testing this Lexer by lexing C source files. So far the lexer has correctly lexed everything. The Lexer combines a set of strings (I will call it alphabet) and built in types (cool word for regex) to lex input. You can find an example of the C language alphabet on the source files folder. As for types, the Lexer comes with some built in types so it is able to match digits, strings and literal strings (text quoted by double or single quotes) out of the box. The Lexer does not support regex so any regex must be hardwritten into the code. I know that sounds scary but it is not, read about limitations to read about this great design decision.

Currently loading the C alphabet into memory consumes around 1300 bytes. Lexing 6108 bytes of C code takes 0.013764 seconds on an Intel Core 2 DUO CPU @2.20GHz (its a crappy Thinkpad). (Fun fact, those 6108 bytes correspond to the file LexerSrc.c without comments. Can be found under sourcefiles).

This Lexer can be used both in Windows or Linux. I have not tested it on a Mac yet because I'm too poor.

## Current state
As far as I am concerned the Lexer is finished. I will now work on implementing a parser to go with it. If you are interested in development I wrote a very simple command line I have been using to test the Lexer. The command line is able to read alphabets, use them to lex text and print the tokens found. I will probably add parsing capabilities to it once I have a semi-working parser.

To compile the command line testing utility just do:
```
$(YOUR_COMPILER) -o TestingShell TokenizerSrc.c LexerSrc.c TestShell.c -I $(PATH_TO_HEADERS_FOLDER)
```
I will soon elaborate real makefiles both for Windows and Linux.

## Limitations
This Lexer does not support regex. Any regex matching needs to be hand written into the code. The Lexer comes with 3 "regex" expressions hand built into the code. You can check the source code to get an idea. (LexerSrc.c, three functions. isNumber, isString and IsLiteral.
* Digits. it will match any sequence of numbers as long as there is one or zero decimal points and one or zero minus signs.
* Strings. It will match any sequence of characters [Aa-Zz] and underscores. 
* Constant Strings. It will match any sequence of characters surrounded by non-escaped double or single quotes.

## Upcoming
* Documentation on most of the source code and on how to easily hand write your own regex into the source code in under five minutes.
* A parser.
* Some tweaks to try and use less memory, specially when lexing.
