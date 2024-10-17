#ifndef __ESDS_H__
#define __ESDS_H__

#include <stdio.h>
#include <stdint.h>

#include "mp4_metadata.h"


int write_esds_box(FILE *fp_mp4, char *p_buf, metadata_t *metadata);


#endif /* __ESDS_H__ */

