#ifndef __FREE_H__
#define __FREE_H__

/* It can nothing add to free box, or use to reserve some space, or use to align 4/8 foe whole file size. */

#include <stdio.h>
#include <stdint.h>


int write_free_box(FILE *fp_mp4, char *p_buf);


#endif /* __FREE_H__ */

