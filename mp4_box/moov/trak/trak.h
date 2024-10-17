#ifndef __TRAK_H__
#define __TRAK_H__

/* container for an individual track or stream */

#include <stdio.h>
#include <stdint.h>

#include "mp4_metadata.h"


int write_trak_box(FILE *fp_mp4, char *p_buf, metadata_t *metadata, track_id_e track_id);


#endif /* __TRAK_H__ */

