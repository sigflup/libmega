	.text
	.proc
	.globl	__modsi3
__modsi3:
	movel	%sp@(8), %d1	/* d1 = divisor */
	movel	%sp@(4), %d0	/* d0 = dividend */
	movel	%d1, %sp@-
	movel	%d0, %sp@-
	jbsr	__divsi3
	addql	#8, %sp
	movel	%sp@(8), %d1	/* d1 = divisor */
	movel	%d1, %sp@-
	movel	%d0, %sp@-
	jbsr	__mulsi3	/* d0 = (a/b)*b */
	addql	#8, %sp
	movel	%sp@(4), %d1	/* d1 = dividend */
	subl	%d0, %d1		/* d1 = a - (a/b)*b */
	movel	%d1, %d0
	rts

