# KiwiLexer
Because reinventing the wheel is always fun...
This is a Lexer focused on simplicty. It has two goals:
* Simple, small and readable source code. Everyone should be able to just open a source file and be able to understand it without having to browse the whole repository looking for missing pieces. 
* Memory efficient. The less memory used the better. I will try to make this program use as little memory as possible. All code is tested
with valgrind to check for memory leaks too. 

## Capabilities
The Lexer is able to lex C code source files correctly. A text file with the C alphabet can be found on the source files and you can use the TestShell to try it out for yourself.  

The source code is also minimal and easy to modify. The whole lexer file is probably around 200 lines without comments. There is no obfuscation at all and all files follow a naming convention that makes it very easy to find the .c file that implements the headers. This means you can just open a file and understand it without having to find typedefs or other definitions deep down a 1000 file repository.

## Limitations
The main limitation is it does not support regular expressions. Other programs like Flex support them and use them to declare what are considered valid identifiers or digits. This lexer avoids that by providing built in datatypes. The built in datatypes are the following:

* Strings. Anything that consists of just characters will be matched as a string. This means that Strings are valid identifiers. 
* String literals. Anything between two '"' will be matched as a string literal. 
* Numbers. Negative or with decimal point, if it finds a stream of numbers it will match them as their type, whether they are integers or real numbers.
