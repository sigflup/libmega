#include "mega.h"
#include "lib.h"
#include "bottom.h"

u8 ctrl1, ctrl2;

volatile void (*v_hook)(void);
volatile void (*h_hook)(void);
volatile void (*ext_hook)(void);


void std_ctrl(void) {
 ctrl1 = poll_ctrl1();
 ctrl2 = poll_ctrl2();
}

void no_routine(void) {
}

