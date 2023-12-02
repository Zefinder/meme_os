/* GPLv2 (c) Airbus */
#ifndef __STRING_H__
#define __STRING_H__

#include <types.h>
#include <gpr.h>
#include <math.h>

/*
** Size is number of store operations to repeat
*/
#define __fix_str_dir(__op__,a,b,c,l)           \
   ({                                           \
      eflags_reg_t save;                        \
      save_flags(save);                         \
      asm volatile ("cld");                     \
      __op__(a,b,c,l);                          \
      load_flags(save);                         \
   })

#define  __rep_8_16(b) ((((uint16_t)b)<<8)|((uint16_t)b))
#define  __rep_8_32(b) ((((uint32_t)__rep_8_16(b))<<16)|((uint32_t)__rep_8_16(b)))

#define __replicate_byte_on_word(b)  __rep_8_16(b)
#define __replicate_byte_on_dword(b) __rep_8_32(b)

#define __replicate_byte_on_long(b) __rep_8_32(b)

#define __memset8(d,v,t,l)  asm volatile ("rep stosb"::"D"(d),"a"(v),"c"(t))
#define __memcpy8(d,s,t,l)  asm volatile ("rep movsb"::"D"(d),"S"(s),"c"(t))
#define __memchr8(d,s,v,l)  asm volatile ("repnz scasb":"=D"(d), "=c"(l):"D"(s),"a"(v),"c"(l))

#define _memset8(d,v,t)     __fix_str_dir(__memset8,d,v,t,0)
#define _memcpy8(d,s,t)     __fix_str_dir(__memcpy8,d,s,t,0)
#define _memchr8(d,s,v,l)   __fix_str_dir(__memchr8,d,s,v,l)

#define __memset32(d,v,t,l)  asm volatile ("rep stosl"::"D"(d),"a"(v),"c"(t))
#define __memcpy32(d,s,t,l)  asm volatile ("rep movsl"::"D"(d),"S"(s),"c"(t))
#define __memchr32(d,s,v,l)  asm volatile ("repnz scasl":"=D"(d), "=c"(l):"D"(s),"a"(v),"c"(l))

#define _memset32(d,v,t)     __fix_str_dir(__memset32,d,v,t,0)
#define _memcpy32(d,s,t)     __fix_str_dir(__memcpy32,d,s,t,0)
#define _memchr32(d,s,v,l)   __fix_str_dir(__memchr32,d,s,v,l)

static inline void __buf_add(buffer_t *buf, size_t len, char c)
{
   if(buf->sz < len)
      buf->data.str[buf->sz++] = c;
}

#define BAD_NIBBLE ((uint8_t)-1)

/*
** Prototypes string.c
*/
void* memset(void*, uint8_t, size_t);
void* memcpy(void*, void*, size_t);
char* strchr(char*, size_t, char);
size_t strlen(char*);

/*
** Prototypes print.c
*/
int    dec_to_uint64(uint8_t*, size_t, uint64_t*);
int    hex_to_uint64(uint8_t*, size_t, uint64_t*);
int    __hex_to_uint8(uint8_t*, uint8_t*);

size_t uint64_to_hex(buffer_t*, size_t, uint64_t, size_t);
void   __uint8_to_hex(uint8_t*, uint8_t);

#endif
