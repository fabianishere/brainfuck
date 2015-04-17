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

----------------------------------------------------------------------------
 "THE BEER-WARE LICENSE" (Revision 42):
 <matslina (at) kth (dot) se> wrote this file. As long as you retain this
 notice you can do whatever you want with this stuff. If we meet some day,
 and you think this stuff is worth it, you can buy me a beer in return.
 Mats Linander 2004-06-15
----------------------------------------------------------------------------

]

  ++++++++++[->++++++++++>+++++++++++>+++<<<]>+.>.++++++.<.>--.>++.<---
  -.+++++++.--------.<---.+++.[-]>+++++.++[--<+>]<.[-]>>.[-]<<<+>>->,--
  --                                                                 --
  --  --[-->++++++[-<------>]<[->+>+<<]<+>+>>>>>,----------]<<<<<<[  <<
  <<  <]>-<<<[>>>>>>>[<<[->>[>>>>>]<+<<<<[<<<<<]>>>]>>[>>>>>]<<[-<<  <[
  <<  <<<]>>>+>>[>>>>>]<<]>[-<+>]<<<<-<<<<<[<<<<<]>>>>>[-]>>>>>]<<<  <[
  <<  <<<]<<+>>>>>    >>[<+>>>          >>>]<+[-      <<[->+>+<<]<[  ->
  +>  -<<]>[-<+>      ]>>[-<    <+>>]<    [[-]    <<<[<<<<<]<<[-]>>  >>
  >>  [>>>>>]<        ]<<<<]    <[-<->    ]<    [>>>>>>>>[<<+++++++  +[
  -<  ++++++>]<.>>    >>>>>>    ]+++++    ++    +[-<+++++<++++<++++  ++
  >>  >]<<<.>.>+++    .<.<.>>+            ++            ++[-<<->>]<  [-
  ]<  <<[<<<.>++++    ++++[-<------>]<    <<    ]+++++    +++++[-<+  ++
  <+  +++++>>]<++.    <+.[-]>.[-]>>>>>    >[    >>>>>]    +[<<<<<]>  >>
  >>  [<<<[-<+>]>+    <<[>>-<<[->+<]]>    [-    <+>]>[    >>-<<-]<+  ++
  ++  ++++[>>+<<-<    -[>>>-<<<[->>+    <<]]    >>[-<<    +>>]>[->[  -]
  <]  <<]<->>>            +>[-        <->>+<]+          <[-<<<+++++  ++
  ++  +>>>]>>>>>>]<<<<<[-]>[-<+>>+<]>[-<+>]<<<<<<<[<<<<<]>>>>>[>>>>  >]
  <<  <<<[>[->+>+>+<<<]>>[-<<+>>]++++++++[->++++++<]>.[-]<<<<<<<<<]  >[
  ->  +>+>+<<<]>>[-<<+>>]++++++++[->++++++<]>.[-]>[>>>>>]<<<<<[<+<<  <<
  ]<                                                                 <<
  +++++++++.--------->+<-]>[-<+>]<]++++++++++[->++++++++>+++++++++++>++
  ++++<<<]>.<++++[->++++<]>+.>--.---.+++++.<+++.>++++.---.--.<+.>>--.++
  [--<<<+>>>]<<<++.>>>+[>>>++++++++[-<++++++>]<.>>>]++++++++++.
