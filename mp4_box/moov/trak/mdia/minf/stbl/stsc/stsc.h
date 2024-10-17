#ifndef __STSC_H__
#define __STSC_H__

/* sample-to-chunk, partial data-offset information */

#include <stdio.h>
#include <stdint.h>

#include "mp4_metadata.h"


int write_stsc_box(FILE *fp_mp4, char *p_buf, metadata_t *metadata, track_id_e track_id);


#endif /* __STSC_H__ */

