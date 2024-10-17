#ifndef __MINF_H__
#define __MINF_H__

/* media information container */

#include <stdio.h>
#include <stdint.h>

#include "mp4_metadata.h"


int write_minf_box(FILE *fp_mp4, char *p_buf, metadata_t *metadata, track_id_e track_id);


#endif /* __MINF_H__ */

