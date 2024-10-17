#ifndef __MDHD_H__
#define __MDHD_H__

/* media header, overall information about the media */

#include <stdio.h>
#include <stdint.h>

#include "mp4_metadata.h"


int write_mdhd_box(FILE *fp_mp4, char *p_buf, metadata_t *metadata, track_id_e track_id);


#endif /* __MDHD_H__ */

