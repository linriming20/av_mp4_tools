#ifndef __MP4_H__
#define __MP4_H__

/* MP4 Operation Utils */

#include <stdio.h>
#include <stdint.h>

#include "mp4_metadata.h"

#define MP4_BOX_BUF_MAX_SIZE    (512*1024)


int mp4_mux_h26x_aac(char *h26x_filename, uint32_t width, uint32_t height, uint32_t video_fps, char *aac_filename, char *mp4_filename);
int mp4_tree(char *mp4_filename, int enable_print, metadata_t *metadata);
int mp4_demux(char *mp4_filename);


#endif /* __MP4_H__ */

