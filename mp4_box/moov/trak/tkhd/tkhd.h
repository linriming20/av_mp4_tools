#ifndef __TKHD_H__
#define __TKHD_H__

/* track header, overall information about the track */

#include <stdio.h>
#include <stdint.h>

#include "mp4_metadata.h"


int write_tkhd_box(FILE *fp_mp4, char *p_buf, metadata_t *metadata, track_id_e track_id);


#endif /* __TKHD_H__ */

