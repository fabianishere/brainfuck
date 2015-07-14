Although brainfuck is basically useless, it isn't quite so crashingly useless
as people usually say. Some small tasks can be done fairly concisely.
(Some of these are fairly UNIX-specific.)

Do nothing, terminate successfully.
(Also called "true".)
(This is the shortest brainfuck quine as well.)

+[>.+<]
Real random byte generator.
(Send output to file, stop program after a while, and take the last byte.)

+++++++.
Beep.
(Small binary data files are easy to make with brainfuck and a pipe, too.)

,[.[-],]
Copy input to output. With a pipe, a no-frills way to make text files.
(Unlike most programs assuming EOF->0, this needed modification before it would
work with EOF->"no change" as well.)
(remove the . to get an input-devourer.)

>,[>,]<[.<]
Reverse input.

unmatched [.
A Kimian quine, for my implementation.

,[---------[-[++++++++++.[-]]],]
Strip tabs and linefeeds.

++++[>++++++++<-],[[>+.-<-]>.<,]
Show ASCII values of input in unary, separated by spaces.
(Useful for checking your implementation's newline behavior on input.)

++++++++++[>++++++++++>+<<-]>[>.<-]
Clear the screen.

+++++++++++[>++++++>+<<-]>+.>-[.[-],]
Play "Prisoner's Dilemma" using the robust "Tit for Tat" strategy.

+++++++++++[>++++++>+<<-]>++.>-.>,[,[-]<<.>.>,]
...and using the not-so-robust "All D" strategy.

+++++[>+++++++++<-],[[>--.++>+<<-]>+.->[<.>-]<<,]
Translate text to brainfuck that prints it.

>>,[>>,]<<[[-<+<]>[>[>>]<[.[-]<[[>>+<<-]<]>>]>]<<]
Sort bytes in ascending order.
(These last two are solutions from Brainfuck Golf.)

>,[>>>++++++++[<[<++>-]<+[>+<-]<-[-[-<]>]>[-<]<,>>>-]<.[-]<<]
Translate "binary" to ASCII, e.g. "0110100001101001" to "hi".

+++++[>+++++<-]>[>++>++++<<-]>-->-<[[>[[>>+<<-]<]>>>-]>-[>+>+<<-]>]
A novel way of moving to the 9999th cell.

>++++[>++++++<-]>-[[<+++++>>+<-]>-]<<[<]>>>>-
-.<<<-.>>>-.<.<.>---.<<+++.>>>++.<<---.[>]<<.
Print "brainfuck" with a linefeed.

++++++++[>++++[>++>+++>+++>+<<<<-]>+>->+>>+[<]<-]>>.>
>---.+++++++..+++.>.<<-.>.+++.------.--------.>+.>++.
Print "Hello World!" with a linefeed.

>++++++++++>>+<+[[+++++[>++++++++<-]>.<++++++[>--------<-]+<<]>.>[->[<++>-]<[>+
<-[>+<-[>+<-[>+<-[>+<-[>+<-[>+<-[>+<-[>+<-[>[-]>[-]++<<-[>+<-]]]]]]]]]]]+>>]<<]
Output powers of two, in decimal. (Doesn't terminate.)

Daniel B Cristofani (cristofdathevanetdotcom)
http://www.hevanet.com/cristofd/brainfuck/
