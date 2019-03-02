=====MUL10=======
[>+<-]>         ; move num one right ie num2=num
>>++++++++++    ; load 10 into fourth element
[
 <<[<+>>+<-]    ; add num2 to first and third element
 >[<+>-]>       ; num2=third element
 -              ; loop ten times
]
<<[-]<          ; clear num2
#
