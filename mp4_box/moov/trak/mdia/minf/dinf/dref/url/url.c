#include <string.h>

#include "mp4_box_header.h"

#include "url.h"

int write_url_box(FILE *fp_mp4, char *p_buf)
{
    uint32_t buf_pos = 0;

    /* 0~11 is [box header], generate last. */
    buf_pos += HEADER_SIZE_FULLBOX;

    /* 12~ is [box data] */
    /* nothing */

    generate_fullbox_header(buf_pos, "url ", 0x00, 0x01, p_buf);

    fwrite(p_buf, 1, buf_pos, fp_mp4);

    return buf_pos;
}

