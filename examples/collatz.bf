>,[
    [
        ----------[
            >>>[>>>>]+[[-]+<[->>>>++>>>>+[>>>>]++[->+<<<<<]]<<<]
            ++++++[>------<-]>--[>>[->>>>]+>+[<<<<]>-],<
        ]>
    ]>>>++>+>>[
        <<[>>>>[-]+++++++++<[>-<-]+++++++++>[-[<->-]+[<<<<]]<[>+<-]>]
        >[>[>>>>]+[[-]<[+[->>>>]>+<]>[<+>[<<<<]]+<<<<]>>>[->>>>]+>+[<<<<]]
        >[[>+>>[<<<<+>>>>-]>]<<<<[-]>[-<<<<]]>>>>>>>
    ]>>+[[-]++++++>>>>]<<<<[[<++++++++>-]<.[-]<[-]<[-]<]<,
]

[The Collatz problem or 3n+1 problem is as follows. Take a natural number n.
If it's even, halve it; if odd, triple it and add one. Repeat the process with
the resulting number, and continue indefinitely. If n is 0, the resulting
sequence is 0, 0, 0, 0... It is conjectured but not proven that for any
positive integer n, the resulting sequence will end in 1, 4, 2, 1...
See also http://www.research.att.com/projects/OEIS?Anum=A006577

This program takes a series of decimal numbers, followed by linefeeds (10).
The entire series is terminated by an EOF (0 or "no change"). For each number
input, the program outputs, in decimal, the number of steps from that number
to zero or one, when following the rule above. It's quite fast; on a Sun
machine, it took three seconds for a random 640-digit number.

One more note. This program was originally written for Tristan Parker's
Brainfuck Texas Holdem contest, and won by default (it was the only entry);
the version I submitted before the contest deadline is at
http://www.hevanet.com/cristofd/brainfuck/oldcollatz.b

Daniel B Cristofani (cristofdathevanetdotcom)
http://www.hevanet.com/cristofd/brainfuck/]
