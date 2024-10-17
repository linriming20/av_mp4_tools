#ifndef __STSS_H__
#define __STSS_H__

/* sync sample box */

#include <stdio.h>
#include <stdint.h>

#include "mp4_metadata.h"


int write_stss_box(FILE *fp_mp4, char *p_buf, metadata_t *metadata);


#endif /* __STSS_H__ */

