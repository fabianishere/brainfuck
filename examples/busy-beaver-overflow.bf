>+>+>+[->[>]+[->[>]+[->[>]+[<]+<]+<]+<]
[ https://sites.google.com/site/res0001/surpassing-graham-s-number
  https://sites.google.com/site/res0001/f
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
Notes (RDB)
The code above is in brainfuck and appears to only need tape to the right.
The number of instructions is 39.
The number of cells used is the same ... 118842243771396506390315925502
Adding a ">+" on the start gives the "Y" value below.
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
Author:  r.e.s. (res0001@gmail.com)
Date:  2005-06-11
=====================================================
A family of F programs giving lower bounds on Sigma_F
=====================================================
The F programming language is described in detail in the appendix
of the article https://sites.google.com/site/res0001/F/Rado_Sigma.txt,
which presents a proof that Sigma_F is not a computable function.
(The webfolder https://sites.google.com/site/res0001/F contains other
F-related material as well.)
Sigma_F is a Busy Beaver function for F programs, defined as follows
in the spirit of Rado's original Busy Beaver TM model:
The memory model for F in this context assumes memory is a bitstring
that's finite but unbounded both to the right *and* to the left.
Sigma_F(n)(n = 1, 2, ...) is defined as the maximum number of 1's
that can be in final memory when an n-instruction F-program halts,
starting with all-0 memory.
Note that, like Rado's Sigma for TMs, Sigma_F is such that
(1) Memory (cf binary tape) is unbounded in both directions.
(2) Sigma_F counts all the 1's (not necessarily contiguous,
    and irrespective of which bit is current) in final memory.
Recursion for the family of programs A(i)
-----------------------------------------
Define programs A(i)(i = 1, 2, 3, ...) recursively as follows:
N(i) := '+>+>...+>+' (with n(i) +'s, and n(i) a positive integer)
A(1) := N(1) '[<]'
A(i) := N(i) '[+>[>]' A(i-1) '+<]'   (i > 1)
where concatenation is indicated by the spaces between strings.
Thus A(i) is a function of the i numbers n(i),n(i-1),...,n(1), say
A(i)[n(i),n(i-1),...,n(1)].  Note that i is the bracket-nesting
"depth" in A(i).
Recursion for the output of the programs A(i)
---------------------------------------------
Let [n(i),...,n(1)]p denote the number of 1's produced by program
A(i)[n(i),...,n(1)] when applied to memory 1^p (0);  that is, when
the current bit is a 0 preceded by a block of p 1's.  The effect
on memory is then seen to be
A(i)[n(i),...,n(1)]:    1^p (0)  |-->  (0) 1^[n(i),...,n(1)]p
and inspection of the program recursion shows that for all i > 1,
[n(i), ..., n(1)]0 = 1 + [n(i-1), ..., n(1)]([n(i)-1, ..., n(1)]0)
[1, n(i-1), ..., n(1)]0 = 1 + [n(i-1), ..., n(1)]0
[n(i), ..., n(1)]p = [n(i)+p, ..., n(1)]0
[n(1)]p = n(1) + p.
These relations recursively define [n(i),...,n(1)]p  for all values
of n(i),...,n(1),p, and lead as follows to some explicit formulas:
[n]0 = n
[m,n]0
= 1 + [n][m-1,n]0
= 1 + n + [m-1,n]0
= (1+n)*(m-1) + [1,n]0
= (1+n )*(m-1) + 1 + [n]0
= m*(n+1)                                <-- note * is multiplication
[k,m,n]0
= 1 + [m,n][k-1,m,n]0
= 1 + [m,n](1 + [m,n][k-2,m,n]0)
= 1 + [m,n](1 + [m,n](1 + ... + [m,n](1+[m,n][1,m,n]0)...)))
= 1 + [m,n](1 + [m,n](1 + ... + [m,n](1+[m,n](1+[m,n]0)...)))
= 1 + [m,n](1 + [m,n](1 + ... + [m,n](1+[m,n](1+m*(n+1))...)))
= 1 + [m,n](1 + [m,n](1 + ... + [m,n](1+[m+1+m*(n+1))*(n+1),n]0)...)))
= (m*(n+1) + 1)*((n+1)^(k-1) + (n+1)^(k-2) + ... + 1)
= (m*(n+1) + 1)*((n+1)^k - 1) / n
> (n+1)^k*m                              <-- note ^ is exponentiation
[j,k,m,n]0
= 1 + [k,m,n][j-1,k,m,n]0
= 1 + [k + [j-1,k,m,n]0, m, n]0
= 1 + (m*(n+1) + 1)*((n+1)^(k'-1) + ...)
where k' = k + [j-1,k,m,n]0
         = k + 1 + (m*(n+1) + 1)*((n+1)^(k''-1) + ...)
      k''= k + [j-2,k,m,n]0
etc., giving
[j,k,m,n]0
>  ((n+1)^^j)^Q*Q                        <-- note ^^ is tetration
where Q = (n+1)^k*m.
[i,j,k,m,n]0
= 1 + [j,k,m,n][i-1,j,k,m,n]0
= 1 + [j + [i-1,j,k,m,n]0, k, m, n]0
> ((n+1)^^^i)^^Q^Q*Q                     <-- etc. for ^^^, ^^^^, ...
where Q = (n+1)^^j^k*m.
Recursion for the length of the programs A(i)
---------------------------------------------
Letting L(i) = L(i)[n(i),n(i-1),...,n(1)] be the length of A(i),
the recursion for A(i) leads immediately to
L(1) = 2*(n(1)+1)
L(i) = 2*(n(i)+4) + L(i-1) (i>1)
giving the explicit formula
L(i) = 2*( n(i) + n(i-1) + ... + n(1) + 4*i - 3 )   (i >= 1).
Examples
--------
The program of depth=3 with arguments n(3)=4, n(2)=1, n(1)=2, is
A(3)[4,1,2] = '+>+>+>+  [+>[>]  +  [+>[>]  +>+   [<]+<]  +<]'
having length
L(3)[4,1,2] = 2*(4+1+2 + 4*3 - 3) = 32
and producing a number of 1's equal to
[4,1,2]0 = (3*1+1)*(3^4-1)/2 = 160.  Hence Sigma_F(32) >= 160.
The programs A(i)(i>1) provide the lower bounds on Sigma_F in the
following table (each program of depth i > 1 is an A(i), or is an
A(i) with a single '+' appended):
length  depth  #1's output   program
------  -----  -----------   ------------------------------------------
1       0      1             +
2       0      1             +>
3       0      2             +>+
4       0      2             +>+>
5       0      3             +>+>+
6       0      3             +>+>+>
...
19      0      10            +>+>+>+>+>+>+>+>+>+
20      0      10            +>+>+>+>+>+>+>+>+>+>
21      0      11            +>+>+>+>+>+>+>+>+>+>+
22      2      12            +>+>+>+[+>[>]+>+[<]+<]
23      2      13            +>+>+>+[+>[>]+>+[<]+<]+
24      2      16            +>+>+>+[+>[>]+>+>+[<]+<]
25      2      17            +>+>+>+[+>[>]+>+>+[<]+<]+
26      2      20            +>+>+>+[+>[>]+>+>+>+[<]+<]
27      2      21            +>+>+>+[+>[>]+>+>+>+[<]+<]+
28      2      25            +>+>+>+>+[+>[>]+>+>+>+[<]+<]
29      2      26            +>+>+>+>+[+>[>]+>+>+>+[<]+<]+
30      3      52            +>+>+[+>[>]+[+>[>]+>+[<]+<]+<]
31      3      53            +>+>+[+>[>]+[+>[>]+>+[<]+<]+<]+
32      3      160           +>+>+>+[+>[>]+[+>[>]+>+[<]+<]+<]
33      3      161           +>+>+>+[+>[>]+[+>[>]+>+[<]+<]+<]+
34      3      484           +>+>+>+>+[+>[>]+[+>[>]+>+[<]+<]+<]
35      3      485           +>+>+>+>+[+>[>]+[+>[>]+>+[<]+<]+<]+
36      3      1705          +>+>+>+>+[+>[>]+[+>[>]+>+>+[<]+<]+<]
37      3      1706          +>+>+>+>+[+>[>]+[+>[>]+>+>+[<]+<]+<]+
38      4      X             +>+>+[+>[>]+[+>[>]+[+>[>]+[<]+<]+<]+<]
39      4      X+1           +>+>+[+>[>]+[+>[>]+[+>[>]+[<]+<]+<]+<]+
40      4      Y             +>+>+>+[+>[>]+[+>[>]+[+>[>]+[<]+<]+<]+<]
...
        where  X = 3*(2^(3*(2^5-1)+2)-1)+1
                 = 3*(2^95-1)+1 = 118842243771396506390315925502
                 > 10^29
               X+1 = 118842243771396506390315925503
               Y = 3*(2^(X+1)-1)+1
                 = 3*(2^118842243771396506390315925503-1)+1
                 > 10^10^28
Open questions (among many):
(1) What is the shortest Busy Beaver of depth > 1 that is neither an
    A(i) nor an A(i) '+' ?
(2) What is the shortest Busy Beaver of odd length not ending in a '+'?
]
