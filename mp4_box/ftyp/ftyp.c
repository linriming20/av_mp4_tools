
#include "mp4_box_header.h"

#include "ftyp.h"



int write_ftyp_box(FILE *fp_mp4, char *p_buf)
{
    uint32_t buf_pos = 0;

    /* 0~7 is [box header], generate last. */
    buf_pos += HEADER_SIZE_BOX;

    /* 8~ is [box data] */
    /* major brand */
    p_buf[buf_pos+0] = 'i';
    p_buf[buf_pos+1] = 's';
    p_buf[buf_pos+2] = 'o';
    p_buf[buf_pos+3] = 'm';
    buf_pos += 4;

    /* minor brand */
    p_buf[buf_pos+0] = 0x00;
    p_buf[buf_pos+1] = 0x00;
    p_buf[buf_pos+2] = 0x02;
    p_buf[buf_pos+3] = 0x00;
    buf_pos += 4;

    /* compatible brands */
    p_buf[buf_pos+0] = 'i';
    p_buf[buf_pos+1] = 's';
    p_buf[buf_pos+2] = 'o';
    p_buf[buf_pos+3] = 'm';
    buf_pos += 4;

    p_buf[buf_pos+0] = 'i';
    p_buf[buf_pos+1] = 's';
    p_buf[buf_pos+2] = 'o';
    p_buf[buf_pos+3] = '2';
    buf_pos += 4;

    p_buf[buf_pos+0] = 'm';
    p_buf[buf_pos+1] = 'p';
    p_buf[buf_pos+2] = '4';
    p_buf[buf_pos+3] = '1';
    buf_pos += 4;

    generate_box_header(buf_pos, "ftyp", p_buf);

    fwrite(p_buf, 1, buf_pos, fp_mp4);

    return buf_pos;
}


