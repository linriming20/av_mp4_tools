#ifndef __MVHD_H__
#define __MVHD_H__

/* movie header, overall declarations */

#include <stdio.h>
#include <stdint.h>

#include "mp4_metadata.h"


int write_mvhd_box(FILE *fp_mp4, char *p_buf, metadata_t *metadata);


#endif /* __MVHD_H__ */

