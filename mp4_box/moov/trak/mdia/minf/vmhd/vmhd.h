#ifndef __VMHD_H__
#define __VMHD_H__

/* video media header, overall information (video track only) */

#include <stdio.h>
#include <stdint.h>


int write_vmhd_box(FILE *fp_mp4, char *p_buf);


#endif /* __VMHD_H__ */

