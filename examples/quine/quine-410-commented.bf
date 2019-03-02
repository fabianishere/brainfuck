Written by Erik Bosman

a 410 byte quine with comments

DATA SECTION

: The data section contains the data needed for reproducing the code section
: The format in which the data is stored is as follows:
:
: every character is stored into memory as two separate bytes (B1; B2)
: the character that is represented by these bytes has the ascii value
: B1 plus B2*16 plus 43
:
: I chose this format because it makes it possible to write a very compact
: code section which has its effect on the size of the data section too
:
: The data is inserted in reversed order and the first minus sign causes the
: code to stop at the beginning of the stream

-
>++>+++ >+>+ >++> >+>+ >+++> >+>+ >++>+++ >+++>+ >> >> >> >> >> >> >> >> >> >>
>> >> >> >> >> >> >+>+ >++> >>+++ >> >> >+++>+ >> >> >> >> >> >> >> >> >> >>
>>+++ >> >> >> >++>+++ >+++>+ >>+++ >> >+++>+ >+++>+ >++>+++ >> >+>+ >+>+
>++>+++ >+>+ >>+++ >> >> >> >+>+ >> >+>+ >++>+++ >+++>+ >>+++ >+++>+ >+++>+
>++>+++ >++> >+>+ >++>+++ >+>+ >>+++ >> >+++>+ >> >++>+++ >+++>+ >>+++ >>
>+++>+ >+++>+ >>+++ >>+++ >>


CODE SECTION

: The data section contains information about how to print the code section
: The following code examines this data and uses it to add the code needed
: for the insertion of the data into the memory (the data section)
: ( this data is in the same format as the code section and also in
:   reversed order ) 
:
: In the process all bytes are increased by one
: therefore the ascii value of a character can now be calculated by
: B1 plus B2*16 plus 26

WHILE THERE ARE BYTES LEFT: +[
	: move the data two positions to the right
	: and add N plus signs at the end where N
	: is the value of the current byte

	[
		>>+[>]+>+[<]<-
	]

	: note that this actually is one too many because the byte 
	: already has been increased once
	:
	: go to the end of the stream once again to replace the last plus sign
	: by a greater than sign and return to the orginal position

	>>[>]<+<+++[<]<

	: on to the next byte
	<
+]


: Go to the end and add the data to print a minus sign there
:
: The third character of the code below is a plus sign instead of a
: greater than sign because it uses less space in the data section
: and it works just as well; the value of this byte is irrelevant
: in the code that is to follow

>>+[>]+++


: note that B1 is 3 and B2 is 0 and the ascii character of the minus sign
: is 45 and not 29 as the formula as described above would return
: This is because in the following loop B2 will be decreased by one for
: all characters but the first one
:
: The actual formula applied will therefore be:
: (B1 plus 10) plus (B2 plus 2)*16

WHILE THERE IS A CHARACTER LEFT TO PRINT: [
	++++++++++>++
	[-<++++++++++++++++>]
	<.<-<
]
