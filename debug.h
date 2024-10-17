#ifndef __DEBUG_H__
#define __DEBUG_H__

/* just for debug print */

#include <stdio.h>


#define RED         "\e[31m"
#define GREEN       "\e[32m"
#define COLOR_END   "\e[0m"

// control by Makefile
#ifdef ENABLE_DEBUG
    #define DEBUG(fmt, args...)     printf(fmt, ##args)
#else
    #define DEBUG(fmt, args...)
#endif



#endif /* __DEBUG_H__ */


