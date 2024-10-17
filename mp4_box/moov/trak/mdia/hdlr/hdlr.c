#include <string.h>

#include "mp4_box_header.h"

#include "hdlr.h"

int write_hdlr_box(FILE *fp_mp4, char *p_buf, track_id_e track_id)
{
    uint32_t buf_pos = 0;

    /* 0~11 is [box header], generate last. */
    buf_pos += HEADER_SIZE_FULLBOX;

    /* 12~ is [box data] */
    /* component type */
    p_buf[buf_pos+0] = 0x00;
    p_buf[buf_pos+1] = 0x00;
    p_buf[buf_pos+2] = 0x00;
    p_buf[buf_pos+3] = 0x00;
    buf_pos += 4;

    /* component subtype */
    switch(track_id)
    {
        case TRACK_ID_AUDIO:
            p_buf[buf_pos+0] = 's';
            p_buf[buf_pos+1] = 'o';
            p_buf[buf_pos+2] = 'u';
            p_buf[buf_pos+3] = 'n';
            buf_pos += 4;
            break;
        case TRACK_ID_VIDEO:
            p_buf[buf_pos+0] = 'v';
            p_buf[buf_pos+1] = 'i';
            p_buf[buf_pos+2] = 'd';
            p_buf[buf_pos+3] = 'e';
            buf_pos += 4;
            break;
    }

    /* component manufacturer */
    p_buf[buf_pos+0] = 0x00;
    p_buf[buf_pos+1] = 0x00;
    p_buf[buf_pos+2] = 0x00;
    p_buf[buf_pos+3] = 0x00;
    buf_pos += 4;

    /* component flags */
    p_buf[buf_pos+0] = 0x00;
    p_buf[buf_pos+1] = 0x00;
    p_buf[buf_pos+2] = 0x00;
    p_buf[buf_pos+3] = 0x00;
    buf_pos += 4;

    /* component flags mask */
    p_buf[buf_pos+0] = 0x00;
    p_buf[buf_pos+1] = 0x00;
    p_buf[buf_pos+2] = 0x00;
    p_buf[buf_pos+3] = 0x00;
    buf_pos += 4;

    /* component name */
    p_buf[buf_pos + 0] = 0x00;
    buf_pos += 1;

    generate_fullbox_header(buf_pos, "hdlr", 0x0, 0x00, p_buf);

    fwrite(p_buf, 1, buf_pos, fp_mp4);

    return buf_pos;
}

