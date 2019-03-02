This is version 0_1 of bfcl

bfcl is a BrainFuck compiler for Linux written itself in BrainFuck
It reads the input from stdin and outputs a Linux ELF binary on stdout
Currently no optimization at all is done (which is another reason why
this thing is so sloooooooow on my system :) but that is planned for
version 0_2

Conventions assumed in this program:
fields are one byte long and decreasing zero is possible

Conventions in the binaries compiled with bfcl:
a) fields are one byte long
b) there are 30 000 fields
c) moving the pointer outside this area will lead to your computer 
   catching fire;
   nothing is done to prevent you from doing that however
d) when end of file is encountered the program stores whatever
   the Linux syscall returns (I believe it's zero but I'm too lazy to
   check)
e) No checks are made on matching parentheses; maybe for version 0_3 :)

And yes; I know the code is far from pretty; far from optimized; and not 
very well documented; but I'm sending it out anyway because the longer I 
stare at it the more my head hurts

Final word of thanks: many ideas are shamelessly stolen from Brian 
Raiter's 171 byte BF compiler available from www_muppetlabs_com/~breadbox/

For questions and comments you can reach me at 
vissers@theochem dot kun dot nl
You will forgive me for not typing the dots :)

Ge Vissers
17 april 2003

**************************************************************************

>>>>>>>                               reserve some extra space
                                      so we can shift the program later

Read the program

Reading a character is a bit of a nuisance since different compilers
use different strategies:
a) leave byte unchanged
b) set byte to zero
c) set byte to 0xff
I *believe* the following code snippets catches all three possibilities above
so that the program ends on either a null or a 0xff byte 

>-                                    set character to 0xff
,                                     read a character
[<+>->+<]                             copy byte to previous and next field
>[+<]>                                if byte is not zero
                                        add one to it
[                                     if it is still not zero
  [-]<                                clear the copy
  +++++[<-------->-]<---              subtract plus from input
  [                                   if char is not plus
    -                                 subtract 1 from char
    [                                 if char is not comma
      -                               subtract 1 from char
      [                               if char is not minus
        -                             subtract 1 from char
        [                             if char is not dot
          --------------              subtract 14 from char
          [                           if char is not left angle
            --                        subtract 2 from char
            [                         if char is not right angle
              >+++[<---------->-]<+   subtract 29 from char
              [                       if char is not left bracket
                --                    subtract 2 from char
                [                     if char is not right bracket
                  <-------->          set opcode to minus 8
                  [-]                 clear character
                ]
                <+>                   increase opcode
              ]                       end if (char is not left bracket)
              <+>                     increase opcode
            ]                         end if (char is not right angle)
            <+>                       increase opcode
          ]                           end if (char is not left angle)
          <+>                         increase opcode
        ]                             end if (char is not dot)
        <+>                           increase opcode 
      ]                               end if (char is not minus)
      <+>                             increase opcode
    ]                                 end if (char is not comma)
    <+>                               increase opcode
  ]                                   end if (char is not plus)
  <+                                  increase opcode 
  [                                   if opcode is not zero
    >                                 move to next field
  ]                                   end if (opcode is not zero)
  >>-,                                read in a new character
  [<+>->+<]                           copy to previous and next field
  >[+<]>                              if not null check if it's 0xff
]                                     end while not EOF

<<[+]                                 clear possible 0xff

>>++++++++[<++++++++++>-]<++++<       84 bytes for ELF header
>++++++++++++<                        12 bytes to initialize program
>++++++<                              6 bytes to end program

Calculate file size

<<[<]>                                move to first opcode
[                                     while opcode exists
  [<<+<+>>>-]<<                       copy to two previous fields
  -                                   decrease
  [                                   if opcode is not plus
    -                                 decrease opcode
    [                                 if opcode is not comma
      -                               decrease opcode
      [                               if opcode is not minus
        -                             decrease opcode
        [                             if opcode is not dot
          -                           decrease opcode
          [                           if opcode is not left angle
            -                         decrease opcode
            [                         if opcode is not right angle
              - 	              decrease opcode
              [                       if opcode is not left bracket
                >>>[>]>+++++          indicate 5 bytes should be added
                <<[<]-<<              set indicator to minus one
                -
              ]                       end if (opcode is not left bracket)
              >>[<-<->>+]<[>-<+]<+    copy indicator and increase
              [                       else (opcode is left bracket)
                >>>[>]>++++++++       indicate 8 bytes should be added
                <<[<]-<<              set indicator to minus one
                -
              ]                       end else (opcode is left bracket)
            ]                         end if (opcode is not right angle)
            >>[<-<->>+]<[>-<+]<+      copy indicator and increase
            [                         else (opcode is right angle)
              >>>[>]>+                indicate 1 byte should be added
              <<[<]-<<                set indicator to minus 1
              -
            ]                         end else (opcode is right angle)
          ]                           end if (opcode is not left angle)
          >>[<-<->>+]<[>-<+]<+        copy indicator and increase
          [                           else (opcode is left angle)
            >>>[>]>+                  indicate 1 byte should be added
            <<[<]-<<                  set indicator to minus 1
            -
          ]                           end else (opcode is left angle)
        ]                             end if (opcode is not dot)
        >>[<-<->>+]<[>-<+]<+          copy indicator and increase
        [                             else (opcode is dot)
          >>>[>]>++++++               indicate 6 bytes should be added
          <<[<]-<<                    set indicator to minus 1
          -
        ]                             end else (opcode is dot)
      ]                               end if (opcode is not minus)
      >>[<-<->>+]<[>-<+]<+            copy indicator and increase
      [                               else (opcode is minus)
        >>>[>]>++                     indicate 2 bytes should be added
        <<[<]-<<                      set indicator to minus 1
        -
      ]                               end else (opcode is minus)
    ]                                 end if (opcode is not comma)
    >>[<-<->>+]<[>-<+]<+              copy indicator and increase
    [                                 else (opcode is comma)
      >>>[>]>++++++                   indicate 6 bytes should be added
      <<[<]-<<                        set indicator to minus 1
      -
    ]                                 end else (opcode is comma)
  ]                                   end if (opcode is not plus)
  >>[<-<->>+]<[>-<+]<+                copy indicator and increase
  [                                   else (opcode is plus)
    >>>[>]>++                         indicate 2 bytes should be added
    <<[<]-<<                          set indicator to minus 1
    -
  ]                                   end else (opcode is plus)

  >>+>[>]>                            move to increment
  [>+                                 increase byte 1
   [>>>>+>+<<<<<-]>>>>>[<<<<<+>>>>>-] copy byte 1
   <[>-<[-]]                          if no overflow set field to minus 1
   >+[-                               if overflow
      <<<<+                           increase byte 2
      [>>>+>+<<<<-]>>>>[<<<<+>>>>-]   copy byte 2
      <[>-<[-]]                       if no overflow set field to minus 1
      >+[-                            if overflow
         <<<+                         increase byte 3
         [>>+>+<<<-]>>>[<<<+>>>-]     copy byte 3
         <[>-<[-]]                    if no overflow set field to minus 1
         >+[-                         if overflow
            <<+                       increase byte 4
            >>
           ]                          end if
        ]                             end if
     ]                                end if
   <<<<<<-                            decrease increment
  ]

  <<[<]>                              move to next opcode
]

>>>>>>                                move behind file size

>++++++++[<++++++++++++++++>-]<-.     output ELF magic bytes
>+++++++[<-------->-]<--.
+++++++.------.

[-]+...-.........++.--.+++.---.+.-... print rest of ELF header
>++++++++[<++++++++++>-]<++++.
>++++++[<+++++++>-]<++.[-]++++.++++.
>++++++[<+++++++>-]<++.[-]...........
>+++++++[<+++++++>-]<+++.>.++++
[<----->-]<.>.+.-.<++++++++.[-].....
+.-........>
++++++++[<++++++++++++++++>-]<.>++++.
++++.>.<<.>----.++++.
<<<<<.>.>.>.                          this is file size

Copy the file size since we need it to initialize ecx
>[-]>[-]<<                            clear the fields 
<<<[>>>>+>+<<<<<-]>>>>>[<<<<<+>>>>>-] copy the bytes
<<<<[>>>>+>+<<<<<-]>>>>>[<<<<<+>>>>>-]
<<<<[>>>>+>+<<<<<-]>>>>>[<<<<<+>>>>>-]
<<<<[>>>>+>+<<<<<-]>>>>>[<<<<<+>>>>>-]

We have to add 30 000 = 0x75 30 to the file size
Start with 0x30

>>>++++++[<++++++++>-]<               set to 0x30
[                                     while increment is not 0
  <<<<<<+                             increase byte 1
  [>>>>+>+<<<<<-]>>>>>[<<<<<+>>>>>-]  copy byte 1
  <[>-<[-]]                           if no overflow set field to minus 1
  >+[-
     <<<<+                            if overflow increase byte 2
     [>>>+>+<<<<-]>>>>[<<<<+>>>>-]    copy byte 2
     <[>-<[-]]                        if no overflow set field to minus 1
     >+[-
        <<<+                          if overflow increase byte 3
        [>>+>+<<<-]>>>[<<<+>>>-]      copy byte 3
        <[>-<[-]]                     if no overflow set field to minus 1
        >+[<<+>>-]                    if overflow increase byte 4
       ]
    ]
  >-                                  decrease increment
]
<<<<<<.                               print first byte
     
Now do 0x75 00
>>>>>>>
+++++++[<++++++++++++++++>-]<+++++    set increment
[                                     while increment is not 0
  <<<<<+                              increase byte 2
  [>>>+>+<<<<-]>>>>[<<<<+>>>>-]       copy byte 2
  <[>-<[-]]                           if no overflow set field to minus 1
  >+[-
     <<<+                             if overflow increase byte 3
     [>>+>+<<<-]>>>[<<<+>>>-]         copy byte 3
     <[>-<[-]]                        if no overflow set field to minus 1
     >+[<<+>>-]                       if overflow increase byte 4
    ]
  >-                                  decrease increment
]
<<<<<.>.>.                            print other 3 bytes

[-]<[-]<[-]<[-]                       clear up
++++++.------....++++++++++++++++.    print rest of header
[-]..

add 0x80 00 to file size
>++++++++[<++++++++++++++++>-]<       set counter to 0x80
[<<<+                                 increase byte 2
 [>>>>+>+<<<<<-]>>>>>[<<<<<+>>>>>-]   copy byte 2
 <[>-<[-]]                            if no overflow set indicator to minus 1
 >+[-                                 if overflow
    <<<<+                             increase byte 3
    [>>>+>+<<<<]>>>>[<<<<+>>>>-]      copy byte 3
    <[>-<[-]]                         if no overflow set indicator to minus 1
    >+[<<<+>>>-]                      if overflow increase byte 4
   ]
 <<-                                  decrease counter
]                                     loop until counter is zero

add 0x04 00 00 to file size
++++                                  set counter to 0x04
[<<+                                  increase byte 3
 [>>>+>+<<<<-]>>>>[<<<<+>>>>-]        copy byte 3
 <[>-<[-]]                            if no overflow set indicator to minus 1
 >+[<<<+>>>-]                         if overflow increase byte 4
 <<-                                  decrease counter
]                                     loop until counter is zero

add 0x08 00 00 00 to file size
<++++++++>

Initialize registers
>>+++++++[<+++++++>-]<.               xor eax eax
>>++++++++++++[<++++++++++++++++>-]<.

<.>>+++[<+++++++++>-]<.               xor ebx ebx

>++++[<-------->-]<--.<<<<<<.>.>.>.   mov ecx filesize

>>.>>+++++[<+++++>-]<.                xor edx edx

>++++[<<++++>>-]<<+.                  inc edx

Now start compiling

>[-]<[-]<[-]<[-]<[-]<[-]<[-]          clean up
<<<<<<[<]>                            move to first instruction

[                                     while opcode exists
  -                                   decrease opcode
  [                                   if opcode is not plus
    -                                 decrease opcode
    [                                 if opcode is not comma
      -                               decrease opcode
      [                               if opcode is not minus
        -                             decrease opcode
        [                             if opcode is not dot
          -                           decrease opcode
          [                           if opcode is not left angle
            -                         decrease opcode
            [                         if opcode is not right angle
              -                       decrease opcode
              [                       if opcode is not left bracket
                <++++[>------<-]>.    output e9
                [-]                   clear this field
                >[>]>>>>>>>>[>>>>>>]  move to end of loop size stack
                -------->->->-<<<     initialize increment
                <<<<<                 move to byte 1 of size
                [                     while byte 1 is not zero
                  >>>>>-              decrease byte 1
                  [>>>>+>+<<<<<-]     copy byte 1
                  >>>>>[<<<<<+>>>>>-]
                  <[>-<[-]]           if no underflow set field to minus 1
                  >+[-                if underflow
                     <<<<-            decrease byte 2
                     [>>>+>+<<<<-]    copy byte 2
                     >>>>[<<<<+>>>>-]
                     <[>-<[-]]        if no underflow set field to minus 1
                     >+[-             if underflow
                        <<<-          decrease byte 3
                        [>>+>+<<<-]   copy byte 3
                        >>>[<<<+>>>-]
                        <[>-<[-]]     if no underflow set field to minus 1
                        >+[-<<->>]    if underflow decrease byte 4
                       ]              end if
                    ]                 end if
                  <<<<<<<<<<-         decrease byte 1 of size
                ]                     end while
                >                     move to byte 2 of size
                [                     while byte 2 is not zero
                  >>>>>-              decrease byte 2
                  [>>>+>+<<<<-]       copy byte two
                  >>>>[<<<<+>>>>-]
                  <[>-<[-]]           if no underflow set field to minus 1
                  >+[-                if underflow
                     <<<-             decrease byte 3
                     [>>+>+<<<-]      copy byte 3
                     >>>[<<<+>>>-]
                     <[>-<[-]]        if no underflow set field to minus 1
                     >+[-<<->>]       if underflow decrease byte 4
                    ]                 end if
                  <<<<<<<<<-          decrease byte 2 of size
                ]                     end while
                >                     move to byte 3 of size
                [                     while byte 3 is not zero
                  >>>>>-              decrease byte 3
                  [>>+>+<<<-]         copy byte 3
                  >>>[<<<+>>>-]
                  <[>-<[-]]           if no underflow set field to minus 1
                  >+[-<<->>]          if underflow decrease byte 4
                  <<<<<<<<-           decrease byte 3 of size
                ]
                >                     move to byte 4 of size
                [                     while byte 4 is not zero
                  >>>>>-<<<<<-        decrease byte 4
                ]
                >->.>.>.>.            print increment
                [+]<[+]<[+]<[+]       clear increment
                <<<<<<-               remove size from stack
                <[<<<<<<]<<<<<<<<[<]  move back to opcode
                <->                   set indicator to minus 1
              ]                       end if (opcode is not left bracket)
              <+                      increase indicator
              [                       else (opcode is left bracket)
                ++++++[>++++++++<-]   set to 38
                >++.---------.        output 3a 31
                <+++++++++++++++.     output 0f
                +[>+++++<-]>+++.      output 84
                [-]                   clear this byte
                                      clear the byte counter
                +                     set nesting level to one

                [                     while nesting greater than 0
                  >[<<<+<+>>>>-]      copy opcode before nesting level
                  <<<-------          subtract 7 from opcode
                  [                   if opcode is not left bracket
                    -                 decrease opcode
                    [                 if opcode is not right bracket
                      [+]             clear field
                      >-<             set indicator to minus 1
                    ]                 end if opcode is not right braket
                    >+                increase indicator
                    [                 else (opcode is right bracket)
                      >-<-            decrease nesting level
                    ]                 end else (opcode is right bracket)
                    -<                set indicator to minus 1
                  ]                   end if (opcode is not left bracket)
                  >+                  increase indicator
                  [                   else (opcode is left bracket)
                    >+<-              increase nesting level
                  ]                   end else (opcode is left bracket)
                  >[>+<-]>            copy nesting to next field
                ]                     end while (nesting greater than 0)
                <<<<                  move to last opcode in loop
                [                     while there are opcodes
                  [>>>+>+<<<<-]       copy the opcode twice
                  >>>                 move to first copy
                  -                   decrease opcode
                  [                   if opcode is not plus
                    -                 decrease opcode
                    [                 if opcode is not comma
                      -               decrease opcode
                      [               if opcode is not minus
                        -             decrease opcode
                        [             if opcode is not dot
                          -           decrease opcode
                          [           if opcode is not left angle
                            -         decrease opcode
                            [         if opcode is not right angle
                              -       decrease opcode
                              [       if opcode is not left bracket
                                -     then it must be right bracket
                                      set increment to five bytes
                                <<+++++>->
                              ]       end if (opcode is not left bracket)
                              <+      increase indicator
                              [       else (opcode is left bracket)
                                      set increment to 8 bytes
                                <++++++++>-
                              ]       end else (opcode is left bracket)
                              ->      set indicator to minus 1
                            ]         end if (opcode is not right angle)
                            <+        increase indicator
                            [         else (opcode is right angle)
                              <+>-    set increment to 1 byte
                            ]         end else (opcode is right angle)
                            ->        set indicator to minus 1
                          ]           end if (opcode is not left angle)
                          <+          increase indicator
                          [           else (opcode is left angle)
                            <+>-      set increment to 1 byte
                          ]           end else (opcode is left angle)
                          ->          set indicator to minus 1
                        ]             end else (opcode is not dot)
                        <+            increase indicator
                        [             else (opcode is dot)
                          <++++++>-   set increment to 6 bytes
                        ]             end else (opcode is dot)
                        ->            set indicator to minus 1
                      ]               end if (opcode is not minus)
                      <+              increase indicator
                      [               else (opcode is minus)
                        <++>-         set increment to two bytes
                      ]               end else (opcode is minus)
                      ->              set indicator to minus 1
                    ]                 end if (opcode is not comma)
                    <+                increase indicator
                    [                 else (opcode is comma)
                      <++++++>-       set increment to 6 bytes
                    ]                 end else (opcode is comma)
                    ->                set indicator to minus 1
                  ]                   end if (opcode is not plus)
                  <+                  increase indicator
                  [                   else (opcode is plus)
                    <++>-             set increment to 2 bytes
                  ]                   end else (opcode is plus)
                  <[>>>[>]>+<<[<]<<-] copy increment behind program
                  >>>[>]>             move to increment
                  [                   while increment is not zero
                    >+                increase byte 1
                    [>>>>+>+<<<<<-]   copy byte 1
                    >>>>>[<<<<<+>>>>>-]
                    <[>-<[-]]         if no overflow set field to minus 1
                    >+[-              if overflow
                       <<<<+          increase byte 2
                       [>>>+>+<<<<-]  copy byte 2
                       >>>>[<<<<+>>>>-]
                       <[>-<[-]]      if no overflow set field to minus 1
                       >+[-           if overflow
                          <<<+        increase byte 3
                          [>>+>+<<<-] copy byte 3
                          >>>[<<<+>>>-]
                          <[>-<[-]]   if no overflow set field to minus 1
                          >+[-        if overflow
                             <<+>>    increase byte 4
                            ]
                         ]            end if
                      ]               end if
                    <<<<<<-           decrease increment
                  ]                   end while
                  <<[<]<<<<           move to next opcode
                ]                     end while opcode exists
                >>>>>[>]>>.>.>.>.     output the loop increment
                <<<                   move to byte 1
                                      copy byte 1 on stack
                [>>>>>>[>>>>>>]>+<<[<<<<<<]<<<<<-]
                >                     move to byte 2
                                      copy byte 2 on stack
                [>>>>>[>>>>>>]>>+<<<[<<<<<<]<<<<-]
                >                     move to byte 3
                                      copy byte 3 on stack
                [>>>>[>>>>>>]>>>+<<<<[<<<<<<]<<<-]
                >                     move to byte 4
                                      copy byte 4 on stack
                [>>>[>>>>>>]>>>>+<<<<<[<<<<<<]<<-]
                                      set surrounding 1 bytes
                >>>[>>>>>>]+>>>>>+<<<<<<[<<<<<<]<<
                <<<<<<[<]             move back to start of loop
                <                     move to indicator field
              ]                       end else (opcode is left bracket)
              ->                      set indicator to minus 1
            ]                         end if (opcode is not right angle)
            <+                        increase indicator
            [                         else (opcode is right angle)
              +++++++[>++++++++<-]>+  set to 41
              .[-]<                   output 41 and clear up
            ]                         end else (opcode is right angle)
            ->                        set indicator to minus 1
          ]                           end if (opcode is not left angle)
          <+                          increase indicator
          [                           else (opcode is left angle)
            +++++++[>+++++++++<-]>+.  output 49
            [-]<                      clear up
          ]                           end else (opcode is left angle)
          ->                          set indicator to minus 1
        ]                             end if (opcode is not dot)
        <+                            increase indicator
        [                             else (opcode is dot)
          ++++++++++                  set to b0
          [>++++++++++++++++<-]>.     output b0
          <++++.>+++.<---.            output 04 b3 01
          +[>+++++++++++++<-]>.       output cd
          <+++++++[>-----------<-]>.  output 80
          [-]<                        clear up
        ]                             end else (opcode is dot)
        ->                            set indicator to minus 1
      ]                               end if (opcode is not minus)
      <+                              increase indicator
      [                               else (opcode is minus)
        >--.++<++++++++.---------     output fe 09
      ]                               end else (opcode is minus)
      ->                              set indicator to minus 1
    ]                                 end if (opcode is not comma)
    <+                                increase indicator
    [                                 else (opcode is comma)
      ++++++++++[>++++++++++++++++<-] set to b0
      >.<+++.>+++.<---.               output b0 03 b3 00
      ++[>+++++++++++++<-]>.<         output cd
      +++++++[>-----------<-]>.       output 80
      [-]<                            clear up
    ]                                 end else (opcode is comma)
    ->                                set indicator to minus one
  ]                                   end if (opcode is not plus)
  <+                                  increase indicator
  [                                   else (opcode is plus)
    ---.+++.-                         output fe 01
  ]                                   end else (opcode is plus)
  >>                                  move to next opcode
]

[>]>

Clean up

>+++++++++++[<++++++++++++++++>-]<.   mov al 1
>+.

<+++.>-.                              mov bl 0

++++[<++++++>-]<++.                   int 0x80
>>++++++++[<++++++++++++++++>-]<.