#ifndef __EXTEND_USERLAND_H__
#define __EXTEND_USERLAND_H__

void __attribute__((section(".usr"),aligned(4))) userland(void);

#endif
