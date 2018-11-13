# KiwiLexer
Because reinventing the wheel is always fun...
This is a simple lexer that returns a stream of tokens based on an alphabet. The alphabets need to be 
written down on a simple text file. The tokenizer currently supports this syntax:

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
