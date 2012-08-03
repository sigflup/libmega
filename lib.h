#define	INTERRUPT	0
#define POLL		1

#define		int2fix(a) 	(((s16)(a))<<8)
#define		fix2int(a)	(((s8)((a)>>8)
#define		float2fix(a)	((s16)((a)*256.0f))

#define RND_A	30
#define RND_C	12
#define RND_M	128

#define DMA_LEN_LO	19
#define DMA_LEN_HI	20
#define DMA_SRC_LO	21
#define DMA_SRC_MI	22
#define DMA_SRC_HI	23

#define DMA_68K		0
#define DMA_FILL	2
#define DMA_COPY	3

#define DMA_DST_VRAM	1
#define DMA_DST_CRAM	3
#define DMA_DST_VSRAM	5

#define HEAP_START	0xff0000

#define CLAMP(Q,A,B)\
{\
 if((Q)>(B))\
  (Q) = (B);\
 if((Q)<(A))\
  (Q) = (A);\
}

#define SYNC \
{\
 while(go == 0);\
 go = 0;\
}

extern volatile int go;

typedef struct {
 int base;
 u16 index, x,y;
 u8 w,h;
 u16 link;
 u8 priority, palette;
 u8 v_flip, h_flip;
 int pattern_index;
} sprite_param_t;

typedef struct {
 u8 r,g,b;
} color_t;

extern volatile u16 *address;
extern volatile u32 *address32;

extern u8 message_buffer[0x20];
extern volatile u32 globl_counter;

extern u16 sprite_counter;

extern void *debug_text;

void blank_chips(void);

void wait_sync(short wait);

void *perm_malloc(int size);
void *malloc(int size);
void free_all(void);

void init_joy(int mode);
u8 poll_ctrl1(void);
u8 poll_ctrl2(void);

void seed(u16 in);
u16 random(void);
void do_dma(int type, int dst_ram, volatile void *src, int dst, int len);
void z80_reset(void);
void z80_write(int addr, const u8 *src, int len);
void vram_write(int addr, const u8 *src, int len);
void vram_write_rle(int addr, const u8 *src, int len);
void cram_write(int addr, const u8 *src, int len);
void clearscreen(void);
void ram_write_rle(u8 *dst, const u8 *src, int len);
void move_sprite(int base, int index, u16 x, u16 y);
void set_sprite(sprite_param_t *param);
void short_delay(void); 
