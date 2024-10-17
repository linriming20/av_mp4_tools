#ifndef __STSZ_H__
#define __STSZ_H__

/* sample size box */

#include <stdio.h>
#include <stdint.h>

#include "mp4_metadata.h"


int write_stsz_box(FILE *fp_mp4, char *p_buf, metadata_t *metadata, track_id_e track_id);


#endif /* __STSZ_H__ */

