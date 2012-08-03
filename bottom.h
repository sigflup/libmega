
extern u8 ctrl1, ctrl2;

extern volatile void (*v_hook)(void);
extern volatile void (*h_hook)(void);
extern volatile void (*ext_hook)(void);

void std_ctrl(void);
void no_routine(void);
