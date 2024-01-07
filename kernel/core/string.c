#include <string.h>
#include <math.h>

/*
** Size is number of bytes
*/
void* memset(void *dst, uint8_t c, size_t size)
{
   size_t cnt, rm;
   loc_t  dloc;

   dloc.addr = dst;

   if(!size)
      return dst;

   __divrm(size, sizeof(ulong_t), cnt, rm);

   if(cnt)
   {
      ulong_t lc = __replicate_byte_on_long(c);
      _memset32(dloc.linear, lc, cnt);
      dloc.linear += cnt*sizeof(ulong_t);
   }

   if(rm)
      _memset8(dloc.linear, c, rm);

   return dst;
}

void* memcpy(void *dst, void *src, size_t size)
{
   size_t cnt, rm;
   loc_t  dloc, sloc;

   dloc.addr = dst;
   sloc.addr = src;

   if(!size)
      return dst;

   __divrm(size, sizeof(ulong_t), cnt, rm);

   if(cnt)
   {
      _memcpy32(dloc.linear, sloc.linear, cnt);
      dloc.linear += cnt*sizeof(ulong_t);
      sloc.linear += cnt*sizeof(ulong_t);
   }

   if(rm)
      _memcpy8(dloc.linear, sloc.linear, rm);

   return dst;
}

char* strchr(char *str, size_t len, char c)
{
   loc_t d, s;

   s.str = str;
   _memchr8(d.linear, s.linear, c, len);

   if(!len)
      return (char*)0;

   return (d.str-1);
}

size_t strlen(char *str)
{
   loc_t  d, s;
   size_t len = -1UL;

   s.str = str;
   _memchr8(d.linear, s.linear, 0, len);
   return ((d.linear-1) - s.linear);
}

int strncmp (const char *s1, const char *s2, size_t n) {
    unsigned char c1 = '\0';
    unsigned char c2 = '\0';
    if (n >= 4) {
        size_t n4 = n >> 2;
        do {
            c1 = (unsigned char) *s1++;
            c2 = (unsigned char) *s2++;
            if (c1 == '\0' || c1 != c2)
            return c1 - c2;
            c1 = (unsigned char) *s1++;
            c2 = (unsigned char) *s2++;
            if (c1 == '\0' || c1 != c2)
            return c1 - c2;
            c1 = (unsigned char) *s1++;
            c2 = (unsigned char) *s2++;
            if (c1 == '\0' || c1 != c2)
            return c1 - c2;
            c1 = (unsigned char) *s1++;
            c2 = (unsigned char) *s2++;
            if (c1 == '\0' || c1 != c2)
            return c1 - c2;
        } while (--n4 > 0);
        n &= 3;
    }
    
    while (n > 0) {
        c1 = (unsigned char) *s1++;
        c2 = (unsigned char) *s2++;
        if (c1 == '\0' || c1 != c2) {
            return c1 - c2;
        }
        n--;
    }
    return c1 - c2;
}