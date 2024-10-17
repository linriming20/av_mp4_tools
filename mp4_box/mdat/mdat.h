#ifndef __MDAT_H__
#define __MDAT_H__

/* media data container */

#include <stdio.h>
#include <stdint.h>

#include "mp4_metadata.h"


int write_mdat_box(FILE *fp_mp4, FILE *fp_h264, FILE *fp_aac, char *p_buf, metadata_t *metadata);


#endif /* __MDAT_H__ */

