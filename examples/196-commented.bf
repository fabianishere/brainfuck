[
 The 196-algorithm implemented in brainfuck by Mats Linander.

 This program reads a number in the form of a string of decimal digits 
 terminated by a unix style newline (0x10) and tries to determine if the
 entered number is a lychrel number.

 A lychrel number is a number which never yields a palindrome when iteratively
 added with its own reversal. The process of iteratively reversing and adding
 until a palindromic number is obtained, is often called the 196-algorithm.
 The smallest number believed to be a lychrel is 196. Hence the name of the
 algorithm.

 This program will keep reversing and adding until a palindromic numbers is
 obtained or it runs out of memory. Given x bytes of memory, an approximately
 x/5 digits long number can be calculated. 

 Rows starting with a percent sign ('%') show what the memory is supposed
 to look like at that point of execution. The string ":::" means "...".

----------------------------------------------------------------------------
 "THE BEER-WARE LICENSE" (Revision 42):
 <matslina (at) kth (dot) se> wrote this file. As long as you retain this
 notice you can do whatever you want with this stuff. If we meet some day,
 and you think this stuff is worth it, you can buy me a beer in return.
 Mats Linander 2004-06-15
----------------------------------------------------------------------------

]

// Request input; print the string "enter number: "
++++++++++[->++++++++++>+++++++++++>+++<<<]
>+.>.++++++.<.>--.>++.<----.+++++++.-------
-.<---.+++.[-]>+++++.++[--<+>]<.[-]>>.[-]<<
<

// Read a string of numbers and setup memory
// Let 'A' denote the first number read; 'B' the second; 'Y' the second last
// and 'Z' the last
// Note that the string may be of any length so it is possible that A=Y
// or Z=A and there can be any number of numbers between 'B' and 'Y'
+>>->
,----------[-->++++++[-<------>]<[->+>+<<]<+>+>>>>>,----------]
<<<<<<[<<<<<]>-
% 1 0 0 0 A A 0 1 1 B B 0 1 1 ::: 0 1 1 Y Y 0 1 1 Z Z 0 0 0 :::

// Main loop
// Loop while cell (0) is 1
<<<[

// Move some numbers around
>>>>>>>[<< [->>[>>>>>]<+<<<<[<<<<<]>>>]>>[>>>>>]<<[-<<<[<<<<<]>>>+>>[>>>>>]<<]
 >[-<+>]<<<<-<<<<<[<<<<<]>>>>>[-]>>>>>]
% 1 0 0 0 A Z 0 0 1 B Y 0 0 1 ::: 0 0 1 Y B 0 0 1 Z A 0 0 0 :::

// Set some flags
<<<<[<<<<<]<<+>>>>>>>[<+>>>>>>]
% 1 1 0 0 A Z 0 1 1 B Y 0 1 1 ::: 0 1 1 Y B 0 1 1 Z A 0 0 0 :::

// for all pairs (AZ and BY etc) if they are not equal (A!=Z or B!=Y etc) clear
// flag in cell (1)
<+[-<<
[->+>+<<]<[->+>-<<]>[-<+>]>>[-<<+>>]<[[-]<<<[<<<<<]<<[-]>>>>>>[>>>>>]<]
<<<<]
% 1 0/1 0 0 A Z 0 0 1 B Y 0 0 1 ::: 0 0 1 Y B 0 0 1 Z A 0 0 0 :::

// The flag in cell (1) is set if and only if the number is palindromic
// If it is we clear the flag in cell (0) and the main loop will end
<[-<->]<
% 1/0 0 0 0 A Z 0 0 1 B Y 0 0 1 ::: 0 0 1 Y B 0 0 1 Z A 0 0 0 :::

// If number is not palindromic we will do some addition
[

// First output some information on where we are
// Print the string "AB:::YZ (plus) ZY:::BA = "
>>>>>>>>[<<++++++++[-<++++++>]<.>>>>>>>>]++++++++[-<+++++<++++<++++++>>>]<<<.>
.>+++.<.<.>>+++++[-<<->>]<[-]<<<[<<<.>++++++++[-<------>]<<<]++++++++++
[-<+++<++++++>>]<++.<+.[-]>.[-]

// Go through all pairs (including the last one)
// Let 'G' and 'H' represent the pair of numbers we are currently working
// with in the loop below
>>>>>>[>>>>>]+[<<<<<]>>>>>[
% ::: G H 0 0 1 :::

// Add them and check if the sum is larger than 9
<<<[-<+>]
>+<<[>>-<<[->+<]]>[-<+>]>[>>-<<-]<+++++++++
[>>+<<-<-[>>>-<<<[->>+<<]]>>[-<<+>>]>[->[-]<]<<]
% ::: G(plus)H(minus)9 0 0 0 1/0 :::

// The rightmost cell in the list above is 1 iff the sum was larger than 9
// If it was we add 1 to the next pair of numbers
// If it was not we restore the sum
<->>>+>[-<->>+<]+<[-<<<++++++++++>>>]
% ::: G(plus)H 0 0 0 1 :::
// Do the next pair
>>>>>>]

// After adding we prepare the sum for next iteration in the main loop
// and print the sum followed by newline
<<<<<[-]>[-<+>>+<]>[-<+>]<<<<<<<[<<<<<]>>>>>[>>>>>]<<<<<
[>[->+>+>+<<<]>>[-<<+>>]++++++++[->++++++<]>.[-]<<<<<<<<<]
>[->+>+>+<<<]>>[-<<+>>]++++++++[->++++++<]>.[-]
>[>>>>>]<<<<<[<+<<<<]<<<
+++++++++.---------
% 1 0 0 0 Z' Z' 0 1 1 Y' Y' 0 1 1 ::: 0 1 1 B' B' 0 1 1 A' A' 0 0 0 :::
// Where the A'B' ::: Y'Z' = AB ::: YZ (plus) ZY ::: BA

// Go back to the main loop
>+<-]
>[-<+>]<

]

// We've got a palindrome and have left the main loop
// Print "Palindrome: A'B':::Y'Z'"
% 1/0 0 0 0 A Z 0 0 1 B Y 0 0 1 ::: 0 0 1 Y B 0 0 1 Z A 0 0 0 :::
++++++++++[->++++++++>+++++++++++>++++++<<<]>.
<++++[->++++<]>+.>--.---.+++++.<+++.>++++.---.
--.<+.>>--.++[--<<<+>>>]<<<++.>>>+[>>>++++++++
[-<++++++>]<.>>>]++++++++++.
