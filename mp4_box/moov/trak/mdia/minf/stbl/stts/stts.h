#ifndef __STTS_H__
#define __STTS_H__

/* (decoding) time-to-sample */

#include <stdio.h>
#include <stdint.h>

#include "mp4_metadata.h"


int write_stts_box(FILE *fp_mp4, char *p_buf, metadata_t *metadata, track_id_e track_id);


#endif /* __STTS_H__ */

