#ifndef __MOOV_H__
#define __MOOV_H__

/* container for all the metadata */

#include <stdio.h>
#include <stdint.h>

#include "mp4_metadata.h"


int write_moov_box(FILE *fp_mp4, char *p_buf, metadata_t *metadata);


#endif /* __MOOV_H__ */

