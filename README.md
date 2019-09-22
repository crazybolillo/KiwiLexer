# KiwiLexer
If worse is better then UNESCO might as well declare this one of the seven wonders. This is a Lexer/Parser with two goals:
* Simple, small and readable source code. Everyone should be able to just open a source file and be able to understand it without having to browse the whole repository looking for missing pieces. 
* Memory efficient. The less memory used the better. 

## KiwiLexer in a nutshell.
This Lexer is able to both lex and parse files on Windows and Linux, it should work on Macs too but I am poor so I can't test it. The idea is for you to be able to either include the existing source code (as a library) in your code to create your own applications that analyze text or for you to be able to modify the source code and recompile it so it fits all your needs. 

*(Calculator implemented with the lexer and parser)*

![Alt text](Screenshot_1.png?raw=true "Calculator implemented with the lexer and parser")

## Current state
Both the Lexer and the Parser have been finished. I implemented a calculator which follows the order of operations (but does not understand parenthesis yet) to test it and so far it works great most of the time although there are some improvements to be made.

A makefile for Linux has been added which compiles all object files and the calculator itself. Technically it should also compile a static library without the calculator but since I do not know how to write makefiles you will have to force make to execute that specific rule. 
There is still a missing makefile for Windows that I will make one day. 

## Limitations
This Lexer does not support regex. Any regex matching needs to be hand written into the code. The Lexer comes with 3 "regex" expressions hand built into the code. You can check the source code to get an idea. (LexerSrc.c, three functions. isNumber, isString and IsLiteral.
* Digits. it will match any sequence of numbers as long as there is one or zero decimal points and one or zero minus signs.
* Strings. It will match any sequence of characters [Aa-Zz] and underscores. 
* Constant Strings. It will match any sequence of characters surrounded by non-escaped double or single quotes.

## Upcoming (someday...)
* Documentation on most of the source code and on how to easily hand write your own regex into the source code in under five minutes.
* I would like to improve the parser so it is not as rigid and allows more customizable productions.
* Some tweaks to try and use less memory, specially when lexing.
