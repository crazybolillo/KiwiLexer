# KiwiLexer
Because reinventing the wheel is always fun...
This is a Lexer focused on simplicty. It has two goals:
* Simple, small and readable source code. Everyone should be able to just open a source file and be able to understand it without having to browse the whole repository looking for missing pieces. 
* Memory efficient. The less memory used the better. I will try to make this program use as little memory as possible. All code is tested
with valgrind to check for memory leaks too. 

## Capabilities
The Lexer is able to lex C code source files correctly. A text file with the C alphabet can be found on the source files and you can use the TestShell to try it out for yourself.  

## Limitations
The main limitation is it does not support regular expressions. Other programs like Flex support them and use them to declare what are considered valid identifiers or digits. This lexer avoids that by providing built in datatypes. The built in datatypes are the following:

* Strings. Anything that consists of just characters will be matched as a string. This means that Strings are valid identifiers. 
* String literals. Anything between two '"' will be matched as a string literal. 
* Numbers. Negative or with decimal point, if it finds a stream of numbers it will match them as their type, whether they are integers or real numbers.

You can add your own built in datatypes or make a parser that understands regex and make a pull request. The source code for the lexer is about 200 lines without comments so the application is very easy to modify to your own needs.
