#ifndef __AVC1_H__
#define __AVC1_H__

#include <stdio.h>
#include <stdint.h>

#include "mp4_metadata.h"


int write_avc1_box(FILE *fp_mp4, char *p_buf, metadata_t *metadata);


#endif /* __AVC1_H__ */

