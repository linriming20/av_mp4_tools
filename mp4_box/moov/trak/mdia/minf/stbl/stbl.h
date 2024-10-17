#ifndef __STBL_H__
#define __STBL_H__

/* sample table box, container for the time/space map */

#include <stdio.h>
#include <stdint.h>

#include "mp4_metadata.h"


int write_stbl_box(FILE *fp_mp4, char *p_buf, metadata_t *metadata, track_id_e track_id);


#endif /* __STBL_H__ */

