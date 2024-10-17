#ifndef __STSD_H__
#define __STSD_H__

/* sample descriptions (codec types, initialization etc.) */

#include <stdio.h>
#include <stdint.h>

#include "mp4_metadata.h"


int write_stsd_box(FILE *fp_mp4, char *p_buf, metadata_t *metadata, track_id_e track_id);


#endif /* __STSD_H__ */

