[A universal Turing machine from Yurii Rogozhin's article "Small universal
Turing machines", in Theoretical Computer Science, 168(2):215-240, 20 November
1996. Thus, a very direct proof that brainfuck is Turing-complete. For i/o
formats and so on, read below; for fuller detail, dig up the article.

If you just want a quick and complete test case, the input b1b1bbb1c1c11111d
should produce the output 1c11111.

Daniel B Cristofani (cristofdathevanetdotcom)
http://www.hevanet.com/cristofd/brainfuck/



This Turing machine achieves Turing-completeness not by simulating other
Turing machines directly, but by simulating a Turing-complete class of
tag-systems (a computational model invented by Emil Post and named after the
children's game "tag"). A tag-system transforms strings over an alphabet A =
{a[1], a[2], ... a[n], a[n+1]} as follows: a positive integer m is chosen, and
so is a function P that maps each a[i] for 1<=i<=n to a string P(a[i]) over
the alphabet A. Now:

1. if the string being transformed has fewer than m elements, the whole
process stops now.
2. m elements are removed from the beginning of the string
3. call the first element removed a[k]; if k=n+1 the whole process stops now.
4. P(a[k]) is appended to the string.
5. steps 1-5 are repeated.

The particular class of tag-systems this Turing machine simulates is the class
where m=2, the initial string has length at least 2, and all P(a[i]) where
1<=i<=n are of the form a[n]a[n]B[i] where B[i] is some string over the
alphabet A (B[i] is the empty string if and only if i=n).

The input for this brainfuck program is mildly complex, and there is no error
checking. The complexity comes from the encoding of tag-systems in terms of
Turing machine tape configurations. Note that the set of initial tape
configurations that represent tag-systems from the above class is a small,
though Turing-complete, subset of the set of possible initial tape
configurations for this Turing machine; and the following brainfuck program is
only designed to accept inputs from that subset.

-The representation of a symbol a[i] from the alphabet A is a string of 1s
which is one element longer than twice the combined length of all P(a[j])
where 1<=j<i.

-a value like P(a[i]) = a[n]a[n]a[w]a[x]...a[y]a[z] is represented as follows:
b 1
b 111...(as many as required to represent a[z] as described above) b
b 111...(to represent a[y] as described above) b
.
.
.
b 111...(to represent a[x]) b
b 111...(to represent a[w]) b
b 111...(to represent a[n]) b
b 111...(as many as for a[n] as described above, MINUS the number of 1s that
represent a[i]; and no final b)

-The function P is represented by listing all its outputs in the order
P(a[n]), P(a[n-1]), ... P(a[2]), P(a[1]). The representation of P(a[n+1])=STOP
is done for you by the brainfuck program.

-The initial string a[q]a[r]...a[s]a[t] to be transformed by the tag-system is
represented as
111...(as many as required to represent a[q] as above) c
111...(to represent a[r]) c
.
.
.
111...(to represent a[s]) c
111...(to represent a[t]; note that there is no final c.)

-The input to this program is a function P as described above, then a single
b, then the initial string to be transformed. Run all the 1s, bs, and cs
together in one line with nothing between, followed by a linefeed, or a period
if a linefeed is problematic for your implementation.

-The output format, if the program terminates, is the same as the input format
for the initial string, and represents the final state of the transformed
string, with a final a[n+1] appended due to a peculiarity of the Turing
machine's algorithm.

-An example.
A tag-system over the alphabet A = {a[1], a[2], a[3], a[4]}, where m = 2 and:
P(a[1]) = a[3]a[3]a[2]a[1]a[4]
P(a[2]) = a[3]a[3]a[1]
P(a[3]) = a[3]a[3]
P(a[4]) = STOP

meets the criteria above; and when applied to the initial string a[2]a[1]a[1]
it gives:
a[2]a[1]a[1]
        a[1]a[3]a[3]a[1]
                a[3]a[1]a[3]a[3]a[2]a[1]a[4]
                        a[3]a[3]a[2]a[1]a[4]a[3]a[3]
                                a[2]a[1]a[4]a[3]a[3]a[3]a[3]
                                        a[4]a[3]a[3]a[3]a[3]a[3]a[3]a[1]
                                                a[3]a[3]a[3]a[3]a[3]a[1]
and then it's done.

Now, the encoding:

a[1] is 1
a[2] is 11111111111
a[3] is 11111111111111111
a[4] is 111111111111111111111

P(a[1]) is b1 b111111111111111111111b b1b b11111111111b b11111111111111111b
b1111111111111111
P(a[2]) is b1 b1b b11111111111111111b b111111
P(a[3]) is b1 b11111111111111111b b

the initial string is 11111111111c1c1

and so the whole input is

b1 b11111111111111111b b
b1 b1b b11111111111111111b b111111
b1 b111111111111111111111b b1b b11111111111b b11111111111111111b
b1111111111111111
b
11111111111c1c1

which when run together for input to the program becomes

b1b11111111111111111bbb1b1bb11111111111111111bb111111b1b111111111111111111111bb1bb11111111111bb11111111111111111bb1111111111111111b11111111111c1c1

The output should be
11111111111111111 c
11111111111111111 c
11111111111111111 c
11111111111111111 c
11111111111111111 c
1 c
111111111111111111111

--that is,
11111111111111111c11111111111111111c11111111111111111c11111111111111111c11111111111111111c1c111111111111111111111


For those interested, the state table of the Turing machine itself is
10<L1    201L2    30cR1    40cL2
11<L1    210R2    311R3    410R4
1b>R1    2b>L3    3b<R4    4bcL2
1<0R1    2<>L2    3< H     4<   
1>bL1    2><R2    3>bR3    4><R4
1c0R4    2cbR2    3c1R1    4cbR4
where the initial state is 1, the blank symbol is "0", tape cells are set as
per the input but with the termination code P(a[n+1])=STOP represented as a
"< b" at the left end, and the head is initially at the first "1" in the code
for the initial string. If and when the machine halts, the head is at the
leftmost cell, a "<"; the representations of the rules are intact, in a form
isomorphic to their original form (each "b" replaced with ">" and each "1"
unchanged); they are followed by a series of "1" cells, then a "c" (the
leftmost one at that time), then the cells representing the final state of the
transformed string, then a "c" and a sequence of "1" cells representing a[n+1]
as mentioned. 

The minimal test case b1b1bbb1c1c11111 represents the tag-system where P(a[1])
= a[1]a[1] and P(a[2]) = STOP, applied to the string a[1]a[1]a[2]. This runs
for 518 steps of the Turing machine, exercising all 23 Turing machine
instructions, before halting with the output string a[1].


Here is the brainfuck program that implements this Turing machine. The basic
memory layout is as follows.
Each Turing machine cell is represented by a brainfuck cell, with the symbols
"0 1 b < > c" represented by 0, 1, 2, 3, 4, 5 respectively. The brainfuck
cells representing the Turing machine cells are laid out contiguously from the
beginning of the tape, except that the head of the Turing machine is
represented by a gap of three brainfuck cells, just to the left of the
brainfuck cell that represents the current Turing machine cell. At the start
of each cycle, the rightmost of these three cells holds the Turing machine
state, where states 1-4 are represented by 1-4 and "halt" (here treated as a
separate state) is represented by 0. The other two cells hold zeroes.

Now to walk through the code:

+++>++>>>+[

Set up 3 2 0 0 1, representing "< b" and the Turing machine head, in the
initial state 1; we can put this at the left end of the brainfuck array
because the Turing machine will never go left from the "<".
Next, start the main input-processing loop. Each time through this loop, we
begin at the rightmost tape cell that we have filled so far, or at the state
cell of the Turing machine head if it is to the right of all tape cells (as it
is initially). Each time through, we read a character; if it is "1" or "c", we
add the appropriate code to the right end of the tape; if it is a "b", we not
only add the code to the end of the tape but also move the head to the right
of it, since the head must follow the rightmost "b" when the Turing machine
starts; if the input character is a linefeed or other terminator, we add
nothing to the tape but position the brainfuck pointer at the zero that
follows the last filled tape cell, thus ending this loop.

>>,[>+++++<[[->]<<]<[>]>]

Read input, producing
... x 0 'i 0 ...
where "x" is a nonzero cell and "i" is the input.
While i is nonzero, run this loop:
-set up
... x 0 'i 5 0 ... 
-If the input was six or greater, the [[->]<<] part will five times decrement
both i and 5; the sixth time, it will only decrement i, and move to the cell
left of i, producing 
... x '0 i 0 0 ...
following which the code <[>]> will restore the pointer to i:
... x 0 'i 0 0 ...
In short, while i is at least six, the net effect of each iteration of the
loop [>+++++<[[->]<<]<[>]>] is to reduce i by 6; so repeated iterations will
change i to i mod 6; call this j. Then the loop will be run once more.
Now legitimate input characters give the values 1, 2, 3, 4 when reduced mod 6;
"1" gives 1, "b" gives 2, "c" gives 3, and linefeed and "." and "d" all give
4. On the last run through the loop [>+++++<[[->]<<]<[>]>] , the [[->]<<] part
will decrement both j and 5 repeatedly until j is zeroed, i.e. it will zero j
while reducing 5 by j, leaving
... x 0 '0 r 0 ...
where r is 5-j. The code <[>]> leaves this configuration unchanged, and the
loop exits.

>-[<<+++++>>-[<<---->>-[->]<]]

If r was 1, i mod 6 was 4, meaning a terminator. So we don't fill any tape
cell but leave
... x 0 0 '0 ...
If r was 2, i mod 6 was 3, meaning "c". So we set up
... x 5 0 '0 ...
If r was 3, i mod 6 was 2, meaning "b". In this case we set up 
... x 1 0 '0 ...
and skip the innermost [->] loop, then step left leaving
... x 1 '0 0 ...
(note the pointer position; only in this case is the pointer immediately to
the right of a nonzero cell.)
If r was 4, i mod 6 was 1, meaning "1". In this case we set up
... x 1 0 '1 0 ...
and enter the inner [->] loop, resulting in
... x 1 0 0 '0 ...
after which we step left, producing
... x 1 0 '0 ...

<[<-<[<]+<+[>]<<+>->>>]

Now we step left. If and only if i was "b", we will enter this loop which will
move the Turing machine head. Now the situation, including the head which is
somewhere off to the left, is
... 2 0 0 1 y ... y y y y '1 0 0 0 ...
where each y is a 1 (since that is the only symbol that occurs between one b
and the next b, in input strings that represent tag-systems).
Now we zero one y and scan left:
... 2 0 '0 1 y ... y y y 0 1 0 0 0 ...
set two more 1's, and scan right (these two, and the 1 that was formerly the
state cell of the head, now serve as tape symbols, so we label them y);
... 2 y y y y ... y y y '0 1 0 0 0 ...
next, we set up the head at its new position, and we set the cell to the left
of it to b, and move the pointer just right of the 1 which is the state:
... 2 y y y y ... y 2 0 0 1 '0 0 0 ...
and now we're to the right of the rightmost nonzero cell, as we should be, so
we end the head-moving loop.

(In the degenerate case where we received two b's in a row, the process goes:
...2 0 0 1 '1 0 0 0 ...
...2 0 '0 0 1 0 0 0 ...
...2 y y '0 1 0 0 0 ...
...2 2 0 0 1 '0 0 0 ...
as it should.)

After either performing or skipping that loop, there are four possibilities
corresponding to the four possible inputs.
... x x x x 1 '0 ... if the input was a "1"
... x 2 0 0 1 '0 ... if the input was a "b"
... x x x x 5 '0 ... if the input was a "c"
... x x x x 0 '0 ... if the input was a terminator.

<]<[<]>[

Now we move left and close the loop. If the input was anything but a
terminator, this puts us at the rightmost nonzero cell and we repeat the
input-processing loop. If the input was a terminator, this puts us at the zero
after the rightmost nonzero cell, and the input is already finished; then we
scan left to the gap that represents the Turing machine head, and position the
pointer at the state cell. Then we begin the main loop, which will be executed
once for each step of the Turing machine's operation, stopping when the state
cell holds 0 (representing the halt state).

-[>++++++<-]>[<+>-]

At the beginning of each iteration of the main processing loop, the
configuration is
... w x 0 0 's y z ...
where w, x, y, z are tape cells, with y being the current tape cell, and s is
the state (1-4). First we combine the current state with the current symbol;
we add (6*(s-1)) onto y, then move the result back into the former location of
s. Call the combination g; its values range from 0 to 23, one for each
state-symbol combination.
... w x 0 0 '0 g z ...
... w x 0 0 g '0 z ...

Now we have to use g to select a new symbol, a new state, and a direction to
move the head. We will provisionally move the head right one cell, and set a
direction flag; if that flag is nonzero, we will move the head left two cells,
resulting in a total movement of one cell to the left. That is, we want to use
g in
... w x 0 0 g 0 z ...
to construct an appropriate
... w x y d 0 s z ...
where d is the direction flag, s is the new state, and y is the new symbol;
then if d is nonzero ("move left after all") we want to shift this to produce
... w 0 0 s x y z ...

The way we use g to pick new values for s, y, and d is a very general scheme
for mathematical functions of one variable, and this UTM is one place we need
the generality. (See my rot13.b for a place where I didn't need the
generality, and used this method anyway, leading to comically inconcise but
fast code.) The basic idea is like
>set f(0)<[>set f(1)<-[>set f(2)<-[>set f(3)<-[...&c...]]]]

The argument (in this case, g) is gradually decremented; if it was (say) five,
the program will enter the five outermost loops of this nest but will skip the
sixth and those inside it, since by that point the argument will have been
zeroed. So we just make sure the output cells have the right values for f(5)
at that point, by setting them inside the fifth loop but outside the sixth.


We get the outputs from the state table, naturally, reading down the columns.
Recall that "01b<>c" map to 0 1 2 3 4 5, and that "d=1" means "left".

+<<<+++>+>
g>=0; set s=1, y=3, d=1

[-
g>=1; same values. There's some continuity in the table; only changes from one
combination to the next will be commented hereafter.

[<<+>->-
g>=2; y=4, d=0

[<<[-]>>-
g>=3; y=0

[<<++>+>-
g>=4; y=2, d=1

[<<-->->>+++<-
g>=5; y=0, d=0, s=4 

[<<+>+>>--<-
g>=6; y=1, d=1, s=2

[<<->->-
g>=7; y=0, d=0

[<<++++>+>>+<-
g>=8; y=4, d=1, s=3

[>-<-
g>=9; s=2

[<<->->-
g>=10; y=3, d=0

[<<->>-
g>=11; y=2

[<<+++>>>-<-
g>=12; y=5, s=1

[<<---->>>++<-
g>=13; y=1, s=3

[<<++>>>+<-
g>=14; y=3, s=4

[>[-]<-
g>=15; s=0 (this is the halt condition; having it produce d=0 is useful, since
moving left would take us outside the brainfuck array, and the capability of
actually not moving the head has been omitted as unnecessary, given that we're
only going to output the part of the tape that holds the final state of the
transformed string.)

[<<->>>+++<-
g>=16; y=2, s=3

[<<->>>--<-
g>=17; y=1, s=1

[<<++++>+>>+<-
g>=18; y=5, d=1, s=2

[<<[-]>->>++<-
g>=19; y=0, d=0, s=4

[<<+++++>+>>--<-
g>=20; y=5, d=1, s=2

[<->>++<[<<->>-]]
Here's a tricky part. g==21 is never produced by the Turing machine from
correct input, so the remaining states to consider are g==22 and g==23, which
should give (y=3, d=0, s=4) and (y=2, d=0, s=4) respectively. So we set up
d=0, s=4 which are common to both, then we take the 2 or 3 that remains in g
and subtract it from y to produce the right result for y.

]]]]]]]]]]]]]]]]]]]]

We arrive here when g has been zeroed. Again, the layout now is
... w x y d '0 s z ...
and y, d, and s have the correct values.

<[->>[<<+>>-]<<<[>>>+<<<-]<[>>>+<<<-]]

If d==1 we do:
... w x y '0 0 s z ...
... w x y s 0 '0 z ...
... w x '0 s 0 y z ...
... w '0 0 s x y z ...
to move the head left and leave the pointer at the leftmost cell of the head,
where it would be if d had been 0 also.

>>]

Go to the state cell, and if it is nonzero (not the halt state) go through the
main Turing machine loop again.

>[-[---[-<]]>]

Now the Turing machine has halted. The situation is 
4 0 0 '0 x x ... x x 0 0 0 ...
where each x is either 1, 4, or 5, and the leftmost 5 ("c") marks the start of
the transformed string. So we scan through the tape looking for that 5, and
incidentally clearing the tape as we go. When we find a 1, we decrement it,
skip the loop [---[-<]], move right, and start fresh. When we find a 4, we
decrement it down to 0, skip the loop [-<], move right, and start fresh. When
we find a 5, we decrement it down to 0, move left, move right to the space the
5 occupied (now 0), and end this loop.

>[+++[<+++++>--]>]

Now we want to output the part of the tape that represents the transformed
string; the situation is 
... 0 '0 x x x ... x x x 0 0 0 ...
where each x is either 1, representing "1", or 5, representing "c". To
transform each to the right ASCII value we first add 3, producing 4 for "1"
and 8 for "c", then multiply the resulting value by 5/2 while moving it left;
this is safe because we know the value is even.  We scan through the string
this way, after which it consists of 10s (for "1") and 20s (for "c"), and the
whole string has been shifted one cell left.

+<++[[>+++++<-]<]

Now we set two more cells to make the linefeed, producing
... 0 x x x ... x x x '2 1 0 0 ...
and multiply each cell by five while moving it right, producing
... '0 0 x x x ... x x x 11 0 0 ...
where each x is either 50 (for "1") or 100 (for "c").

>>[-.>]

Now we scan right, reducing each cell by one and outputting it; the values are
49 (ASCII for "1"), 99 (ASCII for "c"), and 10 (ASCII for the final linefeed).
After this loop there are no more commands, so the program terminates.]


The entire program again without comments:

+++>++>>>+[>>,[>+++++<[[->]<<]<[>]>]>-[<<+++++>>-[<<---->>-[->]<]]
<[<-<[<]+<+[>]<<+>->>>]<]<[<]>[-[>++++++<-]>[<+>-]+<<<+++>+>
  [-
    [<<+>->-
      [<<[-]>>-
        [<<++>+>-
          [<<-->->>+++<-
            [<<+>+>>--<-
              [<<->->-
                [<<++++>+>>+<-
                  [>-<-
                    [<<->->-
                      [<<->>-
                        [<<+++>>>-<-
                          [<<---->>>++<-
                            [<<++>>>+<-
                              [>[-]<-
                                [<<->>>+++<-
                                  [<<->>>--<-
                                    [<<++++>+>>+<-
                                      [<<[-]>->>++<-
                                        [<<+++++>+>>--<-
                                          [<->>++<
                                            [<<->>-
]]]]]]]]]]]]]]]]]]]]]]<[->>[<<+>>-]<<<[>>>+<<<-]<[>>>+<<<-]]>>]
>[-[---[-<]]>]>[+++[<+++++>--]>]+<++[[>+++++<-]<]>>[-.>]


