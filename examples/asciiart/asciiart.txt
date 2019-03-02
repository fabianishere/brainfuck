From: Clifford Wolf <clifford@clifford.at>
To: friends-of-brainfuck@koeln.ccc.de

Hi,

some people told me that I should send the following to this list ..

I wrote a simple brainf*ck program which reads an ascii art only
consisting of the characters / \ _ | SPACE and NEWLINE from the input and
creates an obfuscated c-program which generates the ascii art as output.
there are some limitations:

	* the byte-lenght of the ascii-art must be a multiple of 2

	* I assume that the cell size of the virtual machine is pretty
	  small (like 8 bit). I'm using the construct [-] to set the
	  current memory cell to zero - and that would take some time on
	  32-bit memory cells when the cell has a value such as -1 (0xff..)

	* I assume that the interpreter has an end-of-file handling which
	  does result in (a) not modifying the current cell, (b) writing 0
	  to the current cell or (c) writing -1 to the current cell.

Here comes the code: <in asciiart.b>

clifford@deejay:~/geekfun$ cat demo.txt
  ____ _                     _             __  __   _ _   _ _   ___ _____
 / ___| |__   __ _  ___  ___| |_ _ __ ___ / _|/ _| / / | / / | / _ \___ /
| |   |  _ \ / _| |/ _ \/ __| __| |__/ _ \ |_| |_  | | | | | || | | ||_ \
| |___| | | | |_| | |_| \__ \ |_| | |  __/  _|  _| | | |_| | || |_| |__| |
 \____|_| |_|\__|_|\___/|___/\__|_|  \___|_| |_|   |_|_|_|_|_|_\___/____/

clifford@deejay:~/geekfun$ ./bf fun.bf - < demo.txt
int main(){char*a="3CCA3333333333A333333A5A53553553C5CC524CQOC3A55AC3CQO55CAC:A>A744744745DC:,7737AH:A7>AH4C7C7E<AHOQO5OOOOOO777S56NOCQOOOOQOOQHCHOQOO3C4A7A777E77SOQOC70HCCE7ELCELC<ECICE7HCQQOQ3OQQQQQJC<CC4+",*b="?\n /_\\|";while(*a){putchar(b[(*a-'#')%7]);putchar(b[(*(a++)-35)/7]);}return 0;}

clifford@deejay:~/geekfun$ ./bf fun.bf - < demo.txt > demo.c
clifford@deejay:~/geekfun$ gcc -o demo demo.c
clifford@deejay:~/geekfun$ ./demo
  ____ _                     _             __  __   _ _   _ _   ___ _____
 / ___| |__   __ _  ___  ___| |_ _ __ ___ / _|/ _| / / | / / | / _ \___ /
| |   |  _ \ / _| |/ _ \/ __| __| |__/ _ \ |_| |_  | | | | | || | | ||_ \
| |___| | | | |_| | |_| \__ \ |_| | |  __/  _|  _| | | |_| | || |_| |__| |
 \____|_| |_|\__|_|\___/|___/\__|_|  \___|_| |_|   |_|_|_|_|_|_\___/____/

It is primary tested on the Brainf*ck CPU that I will present at this
years Chaos Communication Congress.

yours,
 - clifford

-- 
| Clifford Wolf /-----=[ www.clifford.at ]==[ Tel: +43-699-10063494 ]=-\
|--------------/ diestartseite.at vocat.cc =[ Fax: +43-2235-42788-4 ]=-|
|-=[ EDEN Creations -- www.edenevents.at ]==[ IRC: www.freenode.net ]=-|
\==[ www.rocklinux.org ]===[ www.rocklinux.net ]===[ www.linbit.com ]==/

2B OR (NOT 2B) That is the question. The answer is FF.

