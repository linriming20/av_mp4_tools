#include <string.h>

#include "mp4_box_header.h"

#include "smhd.h"


int write_smhd_box(FILE *fp_mp4, char *p_buf)
{
    uint32_t buf_pos = 0;

    /* 0~11 is [box header], generate last. */
    buf_pos += HEADER_SIZE_FULLBOX;

    /* 12~ is [box data] */
    /* balance (ignore, skip) */
    p_buf[buf_pos+0] = 0x00;
    p_buf[buf_pos+1] = 0x00;
    buf_pos += 2;

    /* reserved */
    p_buf[buf_pos+0] = 0x00;
    p_buf[buf_pos+1] = 0x00;
    buf_pos += 2;

    generate_fullbox_header(buf_pos, "smhd", 0x0, 0x00, p_buf);

    fwrite(p_buf, 1, buf_pos, fp_mp4);

    return buf_pos;
}


