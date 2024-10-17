#include <string.h>

#include "mp4_box_header.h"

#include "mdhd.h"


int write_mdhd_box(FILE *fp_mp4, char *p_buf, metadata_t *metadata, track_id_e track_id)
{
    uint32_t buf_pos = 0;

    /* 0~11 is [box header], generate last. */
    buf_pos += HEADER_SIZE_FULLBOX;

    /* 12~ is [box data] */
    /* creation time */
    p_buf[buf_pos+0] = (metadata->creation_time >> 24) & 0xFF;
    p_buf[buf_pos+1] = (metadata->creation_time >> 16) & 0xFF;
    p_buf[buf_pos+2] = (metadata->creation_time >> 8)  & 0xFF;
    p_buf[buf_pos+3] = (metadata->creation_time >> 0)  & 0xFF;
    buf_pos += 4;

    /* modification time */
    p_buf[buf_pos+0] = (metadata->modification_time >> 24) & 0xFF;
    p_buf[buf_pos+1] = (metadata->modification_time >> 16) & 0xFF;
    p_buf[buf_pos+2] = (metadata->modification_time >> 8)  & 0xFF;
    p_buf[buf_pos+3] = (metadata->modification_time >> 0)  & 0xFF;
    buf_pos += 4;

    /* timescale */
    p_buf[buf_pos + 0] = (metadata->timescale >> 24) & 0xFF;
    p_buf[buf_pos + 1] = (metadata->timescale >> 16) & 0xFF;
    p_buf[buf_pos + 2] = (metadata->timescale >> 8)  & 0xFF;
    p_buf[buf_pos + 3] = (metadata->timescale >> 0)  & 0xFF;
    buf_pos += 4;

    /* duration */
    p_buf[buf_pos + 0] = (metadata->duration[track_id] >> 24) & 0xFF;
    p_buf[buf_pos + 1] = (metadata->duration[track_id] >> 16) & 0xFF;
    p_buf[buf_pos + 2] = (metadata->duration[track_id] >> 8)  & 0xFF;
    p_buf[buf_pos + 3] = (metadata->duration[track_id] >> 0)  & 0xFF;
    buf_pos += 4;

    /* lanuage */
    p_buf[buf_pos + 0] = 0x15;
    p_buf[buf_pos + 1] = 0xC7;
    buf_pos += 2;

    /* pre-defined */
    p_buf[buf_pos + 0] = 0x00;
    p_buf[buf_pos + 1] = 0x00;
    buf_pos += 2;

    generate_fullbox_header(buf_pos, "mdhd", 0x0, 0x00, p_buf);

    fwrite(p_buf, 1, buf_pos, fp_mp4);

    return buf_pos;
}

