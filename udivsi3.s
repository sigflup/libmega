	.text
	.proc
	.globl	__udivsi3
__udivsi3:
	link	%a6,#-12
	moveml	%d2-%d4,%sp@
	movel	%a6@(8),%d0
	movel	%a6@(12),%d1
	clrl	%d2		| clear p
	moveq	#31,%d4
L1:	addl	%d0,%d0		| shift reg pair (p,a) one bit left
	addxl	%d2,%d2
	movl	%d2,%d3		| subtract b from p, store in tmp.
	subl	%d1,%d3
	jcs	L2		| if no carry,
	bset	#0,%d0	| set the low order bit of a to 1,
	movl	%d3,%d2		| and store tmp in p.
L2:	subql	#1,%d4
	jcc	L1
	moveml	%sp@,%d2-%d4	| restore data registers
	unlk	%a6		| and return
	rts
