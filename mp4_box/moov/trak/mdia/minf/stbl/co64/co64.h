#ifndef __CO64_H__
#define __CO64_H__

/* chunk offset, partial data-offset information */

#include <stdio.h>
#include <stdint.h>

#include "mp4_metadata.h"


int write_co64_box(FILE *fp_mp4, char *p_buf, metadata_t *metadata, track_id_e track_id);


#endif /* __CO64_H__ */

