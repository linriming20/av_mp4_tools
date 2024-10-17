#ifndef __MDIA_H__
#define __MDIA_H__

/* container for the media information in a track */

#include <stdio.h>
#include <stdint.h>

#include "mp4_metadata.h"


int write_mdia_box(FILE *fp_mp4, char *p_buf, metadata_t *metadata, track_id_e track_id);


#endif /* __MDIA_H__ */

