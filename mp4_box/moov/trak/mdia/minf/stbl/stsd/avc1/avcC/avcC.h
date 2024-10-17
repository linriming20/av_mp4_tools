#ifndef __AVCC_H__
#define __AVCC_H__

#include <stdio.h>
#include <stdint.h>

#include "mp4_metadata.h"


int write_avcC_box(FILE *fp_mp4, char *p_buf, metadata_t *metadata);


#endif /* __AVCC_H__ */

