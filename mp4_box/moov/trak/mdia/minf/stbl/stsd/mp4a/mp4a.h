#ifndef __MP4A_H__
#define __MP4A_H__

#include <stdio.h>
#include <stdint.h>

#include "mp4_metadata.h"


int write_mp4a_box(FILE *fp_mp4, char *p_buf, metadata_t *metadata);


#endif /* __MP4A_H__ */

