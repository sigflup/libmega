#include "mega.h"
#include "lib.h"

u8 message_buffer[0x20];

u16 sprite_counter;

volatile u32 globl_counter;

u32 heap_perm, heap_pointer;

volatile int go;

void *debug_text;

volatile u8 dma_majic[10];

u16 rnd_x, rnd_y, rnd_z, rnd_v;

volatile u16 *address;
volatile u32 *address32;

void blank_chips(void) {
 int i;
 heap_pointer = 0;
 heap_perm = 0;
 GET_Z80BUS;
 for(i=0;i<0xff;i++) {
  FM1_WRITE(i,0);
  FM2_WRITE(i,0);
 }
 DROP_Z80BUS;
// do_dma(DMA_FILL, DMA_DST_VRAM, VDP_DATA, NULL, 0xffff);
 debug_text = (void *)0xff0000; 
}

/*
void wait_sync(short wait) {
 int x;
 x = vtimer+1+wait;
  while(x>vtimer);
} */

/* MALLOC on odd alignments, WHAT THE FUCK? */


/* must call all perm_mallocs before regular malloc */
void *perm_malloc(int size) {
 void *ret;
 ret = HEAP_START+heap_perm;
 heap_perm+=size;
 heap_perm |= 1;
 heap_pointer = heap_perm;
 return ret;
}

void *malloc(int size) {
 void *ret; 
 ret = HEAP_START + heap_pointer;
 heap_pointer+=size;
 heap_pointer |= 1;
 return ret;
}

/* free everything malloced with malloc */
void free_all(void) {
 heap_pointer = heap_perm;
}

void do_dma(int type, int dst_ram, volatile void *src, int dst, int len) {
 int len2;
 u32 command_long;
 int i;
 switch(type) {
  case DMA_68K:

   len2 = len>>(dst_ram == DMA_DST_VRAM ? 1 : 0);
   VDP_SET_REGISTER(AUTO_INCREMENT,(dst_ram == DMA_DST_VRAM ? 2 : 1));
   VDP_SET_REGISTER(DMA_LEN_LO, len2&0xff);
   VDP_SET_REGISTER(DMA_LEN_HI, (len2>>8)&0xff);

   VDP_SET_REGISTER(DMA_SRC_LO, ((u32)src>>1) & 0xff);
   VDP_SET_REGISTER(DMA_SRC_MI, ((u32)src>>9) &0xff);
   VDP_SET_REGISTER(DMA_SRC_HI, ((u32)src>>17)&0x7f);

  command_long = 
   ((dst_ram&3)<<30) | (dst&0x3fff)<<16 | 
   0x80|(dst_ram&4)<<2|(dst>>14)&3;

  address = (u16 *)VDP_CONTROL;
  *address = (command_long>>16)&0xffff;

  dma_majic[0] = 0x33;
  dma_majic[1] = 0xfc;
  dma_majic[2] = (command_long & 0xff00)>>8;
  dma_majic[3] = command_long &0xff;
  dma_majic[4] = 0x00;
  dma_majic[5] = 0xc0;
  dma_majic[6] = 0x00;
  dma_majic[7] = 0x04;
  dma_majic[8] = 0x4e;
  dma_majic[9] = 0x75;

  __asm__("jsr		dma_majic");

  break;
  case DMA_FILL:
   VDP_SET_REGISTER(AUTO_INCREMENT,1 );
   VDP_SET_REGISTER(DMA_LEN_LO, len&0xff);
   VDP_SET_REGISTER(DMA_LEN_HI, (len>>8)&0xff);

   VDP_SET_REGISTER(DMA_SRC_LO, 0);
   VDP_SET_REGISTER(DMA_SRC_MI, 0);
   VDP_SET_REGISTER(DMA_SRC_HI, 0x80);

   *(u32 *)VDP_CONTROL = 0x40000080 | (dst&0x3fff)<<16 | (dst>>14)&3;
   *(u16 *)VDP_DATA = 0;

   while((*(u16 *)VDP_CONTROL&2) == 2);
   break;
  case DMA_COPY:
   break;
 }
}

void seed(u16 in) {
 rnd_x = in * 8 + 3;
 rnd_y = in * 2 + 1;
 rnd_z = in | 1;
}

u16 get_random(void) {
 rnd_v = rnd_x * rnd_y;
 rnd_x = rnd_y;
 rnd_y = rnd_v;
 rnd_z = (rnd_z&(65535/2))*(30903/2) + (rnd_z >> 8);
 return rnd_y + rnd_z;
}

void z80_reset(void) {
 int i;
 RESET_Z80_ON
 for(i=0;i<0xffff;i++);
 RESET_Z80_OFF
}


void z80_write_rle(int addr, const u8 *src, int len) {
 int total, src_addr, dst_addr;
 u8 i,v,l; 
 u8 *ram; 
 ram = (u8 *)(addr+Z80_RAM);
 GET_Z80BUS;
 i = 0;
 src_addr = 0;
 dst_addr = 0;
 for(total=0;;) {
  v = src[src_addr++];
  l = src[src_addr++];
  for(i=0;i<l;i++) 
   ram[dst_addr++] = v;
  total+=l;
  if(total==len) break;
 }
}

void vram_write_rle(int addr, const u8 *src, int len) {
 int total,src_addr;
 u8 hi,i,v,l,k;
 i = 0;
 k = 0;
 src_addr = 0;
 VDP_SET_REGISTER(AUTO_INCREMENT,2);
 SET_VRAM_ADDRESS(addr);
 __asm__("/* fuckings */");
 for(total=0;;) {
  v = src[src_addr++];
  l = src[src_addr++];
  for(i=0;i<l;i++) {
   if((k&1) == 0)
    hi = v;
   else  {
    address = (u16 *)VDP_DATA;
    *address = v | (hi<<8);  
   }
   k++;
  }
  total+=l;
  if(total == len) break; 
 }
 __asm__("/* end fuckings */");
}

void ram_write_rle(u8 *dst, const u8 *src, int len) {
 int total, src_addr, dst_addr;
 u8 i,v,l;
 i=0;
 src_addr = 0;
 dst_addr = 0;
 for(total=0;;) {
  v = src[src_addr++];
  l = src[src_addr++];
  for(i=0;i<l;i++)
   dst[dst_addr++] = v;
  total+=l;
  if(total==len) break;
 }
}

void cram_write(int addr, const u8 *src, int len) {
 int i;
 VDP_SET_REGISTER(AUTO_INCREMENT,2);
 SET_CRAM_ADDRESS(addr);
 for(i=0;i<len;i+=2)
  *(u16 *)VDP_DATA = src[i] | (src[i+1] << 8);
}

void clearscreen(void) {
 int i;
 do_dma(DMA_FILL, 0, 0, 0, 0xffff);

 for(i=0;i<64;i++) 
  VDP_SET_PAL(i, 0,0,0);

 for(i=0;i<0x80;i++) {
  SET_VSRAM_ADDRESS(i);
  *(u16 *)VDP_DATA = 0x0000;
 }
}

void vram_write(int addr, const u8 *src, int len) {
 int i;
 int j;
 VDP_SET_REGISTER(AUTO_INCREMENT,2);
 SET_VRAM_ADDRESS(addr);
 j = 0x00ff;
 address = (u16 *)VDP_DATA;
 for(i=0;i<len;i+=2) 
  *address = src[i+1] | (src[i]<<8) ;
 
}

void z80_write(int addr, const u8 *src, int len) {
 int i;
 volatile u8 *ram;
 ram = (u8 *)(addr+Z80_RAM);
 GET_Z80BUS
 for(i=0;i<len;i++) 
  ram[i] = src[i];
 DROP_Z80BUS
}

void move_sprite(int base, int index, u16 x, u16 y) {
 VDP_SET_REGISTER(AUTO_INCREMENT, 2);
 SET_VRAM_ADDRESS( base + (index * 8));
 address = (u16 *)VDP_DATA;
 *address = y&0x7ff;
 SET_VRAM_ADDRESS( base + (index * 8)+6);
 address = (u16 *)VDP_DATA;
 *address = x&0x7ff;
}

void set_sprite(sprite_param_t *param) {
 VDP_SET_REGISTER(AUTO_INCREMENT,2);
 SET_VRAM_ADDRESS( param->base + (param->index * 8));

 address = (u16 *)VDP_DATA;
 *address = param->y&0x7ff;
 *address = ((param->h&3)<<8) | ((param->w&3)<<10) | (param->link&0x7f);
 *address = ((param->priority&1)<<15) | ((param->palette&3)<<13) |
                    ((param->v_flip&1) << 12)|((param->h_flip&1) << 11)|(param->pattern_index&0x7ff);
 *address = param->x&0x7fff;

}

void short_delay(void) {
 int i;
 for(i=0;i<0xffffff;i++);
}

void init_joy(int mode) {
 if(mode == POLL) {
  *(u8 *)CTRL_1 = 0x40;
  *(u8 *)CTRL_2 = 0x40;
  *(u8 *)CTRL_3 = 0x40;
 } else {
  *(u8 *)CTRL_1 = 0xa0;
  *(u8 *)CTRL_2 = 0xa0;
  *(u8 *)CTRL_3	= 0xa0;
 }
}

u8 poll_ctrl1(void) {
 u8 ret; // -1(%fp)
 __asm__(
  "	move.b	#0x40, 0xa10003			\n"
  "	nop					\n"
  "	nop					\n"
  "	move.b	0xa10003, %d1			\n"
  "	andi.b	#0x3f, %d1			\n"
  "	move.b	#0, 0xa10003			\n"
  "	nop					\n"
  "	nop					\n"
  "	move.b 	0xa10003, %d0			\n"
  "	andi.b	#0x30, %d0			\n"
  "	lsl.b	#2, %d0				\n"
  "	or.b	%d1,%d0				\n"
  "	not.b	%d0				\n"
  "	move.b	%d0, -1(%fp)			\n"
 );
 return ret;
}

u8 poll_ctrl2(void) {
 u8 ret; // -1(%fp)
 __asm__(
  "	move.b	#0x40, 0xa10005			\n"
  "	nop					\n"
  "	nop					\n"
  "	move.b	0xa10005, %d1			\n"
  "	andi.b	#0x3f, %d1			\n"
  "	move.b	#0, 0xa10003			\n"
  "	nop					\n"
  "	nop					\n"
  "	move.b 	0xa10005, %d0			\n"
  "	andi.b	#0x30, %d0			\n"
  "	lsl.b	#2, %d0				\n"
  "	or.b	%d1,%d0				\n"
  "	not.b	%d0				\n"
  "	move.b	%d0, -1(%fp)			\n"
 );
 return ret;

}
