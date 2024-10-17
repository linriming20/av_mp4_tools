#ifndef __HVCC_H__
#define __HVCC_H__

#include <stdio.h>
#include <stdint.h>

#include "mp4_metadata.h"


int write_hvcC_box(FILE *fp_mp4, char *p_buf, metadata_t *metadata);


#endif /* __HVCC_H__ */

