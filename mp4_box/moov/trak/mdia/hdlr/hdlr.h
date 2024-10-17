#ifndef __HDLR_H__
#define __HDLR_H__

/* handler, declares the media (handler) type */

#include <stdio.h>
#include <stdint.h>

#include "mp4_metadata.h"


int write_hdlr_box(FILE *fp_mp4, char *p_buf, track_id_e track_id);


#endif /* __HDLR_H__ */

