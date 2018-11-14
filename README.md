# KiwiLexer
Because reinventing the wheel is always fun...
This is a Lexer focused on simplicty. It has three goals (hopefully it  will have two one day):
* Simple, small and readable source code. Everyone should be able to just open a source file and be able to understand it without having to browse the whole repository looking for missing pieces. 
* Memory efficient. The less memory used the better. I will try to make this program use as little memory as possible. All code is tested
with valgrind to check for memory leaks too. 
* Incorportate a parser. In my ideal world you would be able to parse C code with this. At the very least this program will include a simple parser that can be used for command line applications.

## Basics
The Lexer supports this syntax:
```
~ANIMALS,dog,cow,lion,tiger~`
```

Where '~' delimits the definition for a token and its lexemes. Commas ',' separate the different symbols
and the '`' signals the end of the alphabet. The first item of each token declaration is the token ID.
Currently the three characters mentioned can not be used inside token IDs or values. The tokenizer also supports ASCII
only. I said it was simple didnt I? 

So for:

```
~ANIMALS,dog,cow,lion,tiger~`
```

"ANIMALS" is the token, and everything else is its lexems. If the lexer reads "dog" or "cow" etc... it will mark it
as an "ANIMALS" token.

You can declare as many tokens as you want, just do not forget to terminate the file with '`' or the program will take a 
trip down memory lane and read places it should not read. Here is an example where several tokens are declared:
```
~ANIMALS,dog,cow,lion,tiger~
~CLIMATE,sunny,windy,rainy~
~FRUITS,apple,banana,mango,kiwi~`
```
