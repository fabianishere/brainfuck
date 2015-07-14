
2D Table reading routine for BrainFuck
        Made By: NYYRIKKI 2001

**************************************

If you need memory allocate it *HERE*

This address will be later called zero

>>>>>>>>>>>+>>
Z
**************************
* Write tables to memory *
**************************
(Replace with own tables)

Table 0
++++>>++++>>+++++>>+++

>>>>>>+>> (Table separator)
Table 1
++>>+++>>++++

>>>>>>+>>
Table 2
++++>>+++>>++

*****************
* End of tables *
*****************

[[<<]<<<<]<<<<<

****************************************
* Parameter table (Fill with own code) *
****************************************

(We are in address zero Put your own code here)

Read (Z=No NZ=Yes)
+

> Table number (0 to 255)
++

> Address in table (1 to 255)
++

> Value (1 to 255)

<<<
**************************
* End of Parameter table *
**************************


********
* Code *
********
[[-] Read Y/N

Copy table
>[->>>+>>+<< <<<]>>>[-<<<+>>>]
Copy Address
<<[->>+>> >>>>+<<<< <<<<]>>[-<<+>>]
>>

Goto table X
[-
>>>>
Move address number
[->[>>]>>>+<<< <<[<<]>]
Move table number
<<<<[-> >>>>[>>]<+<[<<]<<<]

>>>>>[>>]<
] End of goto

>>>>

Search address in current table

[-[->>+<<]+>>]

>
[-<+> >+<]

>[-<+>]<
<
-
<<<<
[->>>>

[-<<+>>]

<<<<<<
]
>>>>
[-<<+>>]<<

Copy value to parameter table
>-<
[-
<<<<<
[[<<]<<<<]<<
+
>> >>>>>>
[[>>]>>>>]
<
]
>+<
<<<<<[[<<]<<<<]<<<<<
] End of Read Y/N

***************
* End of Code *
***************

Read = 0
Value = readed value
other parameters are not changed

(We are in address zero Continue your code here)