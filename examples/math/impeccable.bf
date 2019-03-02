[impeccable.b -- compute impeccable numbers
(c) 2016 Daniel B. Cristofani
http://brainfuck.org/]

>>>->+[
    [
        [<<+>+>-]++++++[<<++++++++>>-]<<-.[-]<
    ]++++++++++.[-]>>>++<[
        [-[[>>>]<<<-[+>>>]<<<[<<<]+>]<-[+>++++++++++>>]>]>>>
        [[>+<-]>>>]<<[-[<++>-[<++>-[<++>-[<++>-[<[-]>-[<++>-]>>[<+<]>[->]<++<<]]]]]<+<<]
	>
    ]>[>>>]<<<
]

This program outputs sequence (http://oeis.org/A014221). Although this sequence
is technically nonterminating, computing its eighth term would require more
storage than can exist in this universe, so you may as well kill this program
after the seventh term.
