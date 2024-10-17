#include "mp4_box_header.h"

#include "free.h"



int write_free_box(FILE *fp_mp4, char *p_buf)
{
    uint32_t buf_pos = 0;

    /* 0~7 is [box header], generate last. */
    buf_pos += HEADER_SIZE_BOX;

    /* 8~ is [box data] */
    /* It can nothing add to free box, or use to reserve some space, or use to align 4/8 foe whole file size. */

    generate_box_header(buf_pos, "free", p_buf);

    fwrite(p_buf, 1, buf_pos, fp_mp4);

    return buf_pos;
}

