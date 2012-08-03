	.text
	.proc
	.globl	__divsi3
__divsi3:
	movel	%d2, %sp@-

	moveq	#1, %d2	/* sign of result stored in d2 (=1 or =-1) */
	movel	%sp@(12), %d1	/* d1 = divisor */
	jpl	L1
	negl	%d1
	negl	%d2		/* change sign because divisor <0  */
L1:	movel	%sp@(8), %d0	/* d0 = dividend */
	jpl	L2
	negl	%d0
	negl	%d2

L2:	movel	%d1, %sp@-
	movel	%d0, %sp@-
	jbsr	__udivsi3	/* divide abs(dividend) by abs(divisor) */
	addql	#8, %sp

	tstb	%d2
	jpl	L3
	negl	%d0

L3:	movel	%sp@+, %d2
	rts

