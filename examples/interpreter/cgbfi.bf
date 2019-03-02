Brainfuck Self Interpreter: by Clive Gifford
Version 1:  1 December 2006
The code to parse/load the input to the interpreter is almost exactly the same
as that in the interpreter described in the Nov 2003 paper by Oleg Mazonka and
Daniel Cristofani but the main interpreter loop is "new"
The main change in the loader code is to leave every second cell free for
working storage
The goal for this interpreter was to be more efficient with the main change in
approach being to only make one round trip between the emulated code and data
memory location for each instruction executed instead of multiple round trips
(according to the value of the internal coding scheme which for example is 8
for the "plus" instruction)
Other info:
An exclamation mark MUST be used to delimit the end of the input (code) to be
interpreted and any other "higher level" data ~ you can stack multiple copies
if you wish as long as they are all separated by exclamation marks also
Input is BF codes (with comments also if desired except for exclamation mark)
The underlying interpreter determines the possible range of data cell values
Does not use wrapping of data values
>>>>                      leave a little extra space before the program code
>+                        start in right hand cell of first program code pair
[
  ->>
  ++>+>+++++++            setup differences and read char as per dbfi
  [<++++>>++<-]++>>+>+>
  +++++[>++>++++++<<-]
  +>>>,<++
  [
    [>[->>]<[>>]<<-]      section 3
     <[<]<+>>[>]>
    [                     section 4
      <+>-
      [[<+>-]>]           section 5
      <                   section 6
      [
        [[-]<]            scan left and zero the differences
        ++<-              section 7
        [
          <+++++++++>[<->-]>>
        ]
        >>
      ]
    ]
    <<
  ]
 *** switch to handle three possibilities and spacing of instruction codes ***

  >[-]+<<                 set "done" flag and position to value
  [
    --
    [
      [-]
      >>- >+< <<
    ]
    >> [- <<<<[>+<-]>>>>> >+<] <<
  ]
  >> [- 0] <<
  >>>[<<<+>>>-]<<<

]
>>>+<<<<[<<]>>            go to first instruction
                          ********** MAIN INTERPRETER LOOPS STARTS HERER **********
[                         start on current instruction code
                          below is a big switch statement to decode instructions
  [<+>>+<-]               first need to move/copy instruction code and set flag so can
  +<-                     start with '(i less 1) (1) (i) where i is instruction code
  [
   -[
     -[
       -[
         -[
           -[
             -[
                -        can't be anything but 1 so bracketing not needed
                >- >> [>>] >> [>>]< + <[<<] << [<<] <    increment
              ]
              >[- >> [>>] >> [>>]< , <[<<] << [<<]] <    input
            ]
            >[- >> [>>] >> [>>]< - <[<<] << [<<]]<       decrement
          ]
          >[- >> [>>] >> [>>]< . <[<<] << [<<]]<         output
        ]
        >[- >> [>>] >> [>>] <<-<< [<<] << [<<]]<         move left
      ]
      >[- >> [>>] >> [>>] + [<<] << [<<]]<               move right
    ]
    >
    [-                      left hand bracket
      >> [>>] >> [>>]<      move to data cell
      [>+>>+<<<-]>          make double copy and move to first
      [<+>-]                restore original data cell value
      >>[<<+>>[-]]+         This and the following achieves
      <<[>>-<<-]            x = not x
      >>                    go to flag cell (0 or 1)
      Some tricky stuff here: set up (not flag) also so we can later choose
      appropriate instruction sequence to get back to code area in one pass
      In one case we set flags at the other end (data greater than 0) but
      for the other we just go back without setting any flags (data equals 0)
      [<<+>>>>+<<-]         make two copes of flag
      >>[<<+>>-]
      <<[>>+<<-]+           This and the following achieves
      >>[<<->>-]<<          x = not x
      <<                    so we now have (data) '(flag) (?) (not flag)
      [                     if flag set then
        -<< [<<] << [<<]<   clear and return to code section where we save
        << << ++            a 2 meaning we need (later) to match left bracket
        >>                  stop in zero cell for now
      ]
      >>                    if we executed code above then now at switch flag
                            else it will put us ready to return from data area
      [-<<<<<<[<<]<<[<<]<]  move back to switch flag without setting anything
      >
    ]
    <
  ]
  >
  [-                        right hand bracket
    >> [>>] >> [>>]<        move to data cell
    [>+>>+<<<-]>            make double copy and move to first
    [[<+>-]>>[-]+<<]        restore data from first then zero second and set flag
    >>                      go to flag cell (0 or 1)
    Some tricky stuff here: set up (not flag) also so we can later choose
    appropriate instruction sequence to get back to code area in one pass
    In one case we set flags at the other end (data greater than 0) but
    for the other we just go back without setting any flags (data equals 0)
    [<<+>>>>+<<-]           make two copes of flag
    >>[<<+>>-]
    <<[>>+<<-]+             This and the following achieves
    >>[<<->>-]<<            x = not x

    <<                      so we now have (data) '(flag) (?) (not flag)

    [                       if flag set then
      -<< [<<] << [<<]<     clear and return to code section where we save
      << << +               a 1 meaning we need (later) to match right bracket
      >>                    stop in zero cell for now
    ]
    >>                      if we executed code above then now at switch flag
                            else it will put us ready to return from data area

    [-<<<<<<[<<]<<[<<]<]    move back to switch flag without setting anything

    >
  ]
  >[<+>-]                   restore original instruction code
  *** We are positioned in the cell immediately to the right of the   ***
  *** instruction that has just been "executed" in the switch above   ***
  *** The following code is to handle finding matching brackets       ***
  *** because code above has only set a cell value to 1 or 2 to show  ***
  *** what kind of loop scanning is required (1=scan left 2 = right)  ***

  << << <<                  position to cell showing if matching required
  [                         if non zero we need to find a matching bracket
    >> +                    set up "done" flag for switch and
    << -                    decrement switch value so now is 0 or 1
    [                       if 1 we are looking for matching right bracket
      - >> - >> +           clear switch value & "done" & set level to 1
      [                     while level is more than 0
        >>>[-<+>>+<]        double copy
        +<-                 set flag and prepare for switch
        [
         -[
            [-]
            > [- other] <   do nothing except clear flag
          ]
          > [- 1 <<< + >>>] < increment level
        ]
        > [- 0 <<< - >>>]   decrement level
        >[-<+>]<<           restore instruction code
        <<                  go to level
        [>>+<<-]            if level then move right one instruction
        >>
      ]
      << << <<              go back to switch value cell
    ]
    >>                      go to switch done flag and if still set then
    [                       we must be looking for a matching left bracket
      - << +                clear switch value & "done" & set level to 1
      [                     repeat while level is more than 0
        >>>[-<+>>+<]        double copy
        +<-                 set flag and prepare for switch
        [
         -[
            [-]
            > [- other] <   do nothing except clear flag
          ]
          > [- 1 <<< - >>>] < decrement level
        ]
        > [- 0 <<< + >>>]   increment level
        >[-<+>]<<           restore instruction code
        <<                  go to level
        [<<+>>-]            if level then move left one instruction
        <<
      ]
    ]
  ]
  >> >> >>
  >                         move forward to next instruction
]
