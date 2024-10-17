#ifndef __SMHD_H__
#define __SMHD_H__

/* audio media header, overall information (audio track only) */

#include <stdio.h>
#include <stdint.h>


int write_smhd_box(FILE *fp_mp4, char *p_buf);


#endif /* __SMHD_H__ */

