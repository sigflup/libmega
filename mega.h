/* sega.doc is bonk-ass confusing jonk... JONK! */

#define NULL	0

#define HALT	for(;;);

/* VDP Registers */
#define	MODE_SET_1		0
#define MODE_SET_2		1
#define NAME_TABLE_SCROLLA_BASE	2
#define NAME_TABLE_WINDOW_BASE 	3
#define NAME_TABLE_SCROLLB_BASE	4
#define SPRITE_ATTRIBUTE_BASE	5
#define BACKGROUND_COLOR	7
#define H_COUNTER		10
#define MODE_SET_3		11
#define MODE_SET_4		12
#define HSCROLL_BASE		13
#define AUTO_INCREMENT		15
#define SCROLL_SIZE		16
#define	WINDOW_X		17
#define WINDOW_Y		18

/* mode bits */
#define MODE_5			0x4
#define GEN_DISPLAY		0x80
#define ENABLE_H_INT		0x10
#define HV_COUNTER_STOP		0x2
#define VDP_ON			0x1
#define ENABLE_PALETTE		0x4

#define ENABLE_DISPLAY		0x40
#define ENABLE_V_INT		0x20
#define ENABLE_DMA		0x10

#define	VERTICAL_2CELL		0x4
#define HORIZONTAL_CELL		0x2
#define HORIZONTAL_LINE		0x3

#define CELLW_40		0x81

#define SCROLL_WIDTH_64		1
#define SCROLL_WIDTH_128	3
#define SCROLL_HEIGHT_64	(1<<4)
#define SCROLL_HEIGHT_128	(3<<4)

#define	ACCESS_VRAM_W	1
#define ACCESS_CRAM_W	3
#define ACCESS_VRAM_R	0
#define ACCESS_CRAM_R	8

typedef signed char    s8;
typedef signed short   s16;
typedef signed int     s32;
typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;


#define VDP_CONTROL	0xc00004
#define VDP_DATA	0xc00000
#define Z80_RAM		0xa00000
#define Z80_BUSRQ	0xa11100
#define Z80_RESET	0xa11200


#define HSCROLL_ADDR(x)\
 ((x&0xfc00)>>10)
#define SCROLL_A_ADDR(x)\
 (((x&0xe000)>>13)<<3)
#define SCROLL_B_ADDR(x)\
 ((x&0xe000)>>13)

#define SPRITE_ADDR(x)\
 ((x&0xfe00)>>9)

#define VDP_STATUS\
 (*(u16 *)VDP_CONTROL)

#define SET_CRAM_ADDRESS(addr)\
 address32 = (u32 *)VDP_CONTROL; \
 *address32 = 0xc0002000 | ((addr&0x7f) << 16);

#define CRAM_WRITE(addr, x)\
 SET_CRAM_ADDRESS(addr)\
 address = (u16 *)VDP_DATA; \
 *address = x;

#define SET_VSRAM_ADDRESS(addr)\
 address32 = (u32 *)VDP_CONTROL;\
 *address32 = 0x40000010 | ((addr&0x7e)<<16);

#define VSRAM_WRITE(addr, x)\
 SET_VSRAM_ADDRESS(addr);\
 address = (u16 *)VDP_DATA; \
 *address = (x);

#define SET_VRAM_ADDRESS(addr)\
 address = (u16 *)VDP_CONTROL; \
 *address = 0x4000 | ((addr &0x3fff) &~1);\
 *address = (addr&0xc000)>>14;
// *(u32 *)VDP_CONTROL = 0x40000000 | (((addr&0x3fff)&~1)<<16) | ((addr&0xc000)>>14);

#define VRAM_WRITE(addr,x)\
{ \
 SET_VRAM_ADDRESS(addr);\
 address = (u16 *)VDP_DATA; \
 *address = x; \
}

#define GET_Z80BUS	\
 address = (u16 *)Z80_BUSRQ; \
 *address = 0x100; \
 for(;;) \
  if( ((*address >> 7)&1) == 0) break;

#define DROP_Z80BUS \
 address = (u16 *)Z80_BUSRQ; \
 *address = 0;

#define RESET_Z80_ON\
 address = (u16 *)Z80_RESET; \
 *address = 0;

#define RESET_Z80_OFF\
 address = (u16 *)Z80_RESET; \
 *address = 0x100;

#define VDP_WRITE_ADDR(x) (((0x4000 + (x & 0x3fff)) << 16)+(x>>14))

#define _WRT1(cd, x)\
 (u16)( (x&0x3fff) | ((cd&3)<<14));


#define _WRT2(cd,x) \
 (u16)( ((x&0xc000)>>14) | ((cd&0x3c)<<4))

#define VDP_WRITE(cd,addr,value) \
 *(u16 *)VDP_CONTROL = _WRT1(cd,addr); \
 *(u16 *)VDP_CONTROL = _WRT2(cd,addr); \
 *(u16 *)VDP_DATA = value

#define VDP_SET_REGISTER(x,y) \
{ \
 address = (u16 *)VDP_CONTROL; \
 *address = (u16)(0x8000 | (x<<8)) | y; \
}

#define VDP_CRAM_COLOR(R,G,B) \
 (u16)(((R<<1)&0xe) | ((((B<<1)&0xe)<<8)) | (((G<<1)&0xe)<<4) )

#define VDP_SET_PAL(I,R,G,B) 	{\
 CRAM_WRITE((I<<1),VDP_CRAM_COLOR(R,G,B))\
}

#define FM1_WRITE(A,D) \
 while((*(u8 *)(Z80_RAM+0x4000)&0x80)==0x80) { }\
 *(u8 *)(Z80_RAM+0x4000) = A; \
 while((*(u8 *)(Z80_RAM+0x4000)&0x80)==0x80) { }\
 *(u8 *)(Z80_RAM+0x4001) = D;

#define FM2_WRITE(A,D) \
 while((*(u8 *)(Z80_RAM+0x4002)&0x80)==0x80) { }\
 *(u8 *)(Z80_RAM+0x4002) = A; \
 while((*(u8 *)(Z80_RAM+0x4002)&0x80)==0x80) { }\
 *(u8 *)(Z80_RAM+0x4003) = D;


/* SERIAL/PAR PORTS */

#define UP		0x01
#define DOWN		0x02
#define LEFT		0x04
#define RIGHT		0x08
#define B		0x10
#define	C		0x20
#define A		0x40
#define START		0x80

#define DATA_CTRL1	0xa10003
#define DATA_CTRL2	0xa10005
#define DATA_EXP	0xa10007
#define CTRL_1		0xa10009
#define CTRL_2		0xa1000b
#define CTRL_3		0xa1000d
#define TX_CTRL1	0xa1000f
#define RX_CTRL1	0xa10011
#define SCTRL_1		0xa10013
#define TX_CTRL2	0xa10015
#define RX_CTRL2	0xa10017
#define SCTRL_2		0xa10019
#define TX_CTRL3	0xa1001b
#define RX_CTRL3	0xa1001d
#define SCTRL_3		0xa1001f
