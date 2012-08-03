	.text
	.proc
	.globl	__mulsi3
__mulsi3:
	movew	%sp@(4), %d0	/* x0 -> d0 */
	muluw	%sp@(10), %d0	/* x0*y1 */
	movew	%sp@(6), %d1	/* x1 -> d1 */
	muluw	%sp@(8), %d1	/* x1*y0 */
	addl	%d1, %d0
	swap	%d0
	clrw	%d0
	movew	%sp@(6), %d1	/* x1 -> d1 */
	muluw	%sp@(10), %d1	/* x1*y1 */
	addl	%d1, %d0

	rts

