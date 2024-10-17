#include <string.h>

#include "mp4_box_header.h"

#include "mvhd.h"



int write_mvhd_box(FILE *fp_mp4, char *p_buf, metadata_t *metadata)
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
    p_buf[buf_pos + 0] = (metadata->duration_mp4 >> 24) & 0xFF;
    p_buf[buf_pos + 1] = (metadata->duration_mp4 >> 16) & 0xFF;
    p_buf[buf_pos + 2] = (metadata->duration_mp4 >> 8)  & 0xFF;
    p_buf[buf_pos + 3] = (metadata->duration_mp4 >> 0)  & 0xFF;
    buf_pos += 4;

    /* rate */
    p_buf[buf_pos + 0] = 0x00;
    p_buf[buf_pos + 1] = 0x01;
    p_buf[buf_pos + 2] = 0x00;
    p_buf[buf_pos + 3] = 0x00;
    buf_pos += 4;

    /* volume */
    p_buf[buf_pos + 0] = 0x01;
    p_buf[buf_pos + 1] = 0x00;
    buf_pos += 2;

    /* reserved */
    memset(p_buf + buf_pos, 0, 10);
    buf_pos += 10;

    /* matrix { 0x00010000,0,0,0,0x00010000,0,0,0,0x40000000 } */
    p_buf[buf_pos + 0] = 0x00;
    p_buf[buf_pos + 1] = 0x01;
    p_buf[buf_pos + 2] = 0x00;
    p_buf[buf_pos + 2] = 0x00;
    buf_pos += 4;
    memset(p_buf + buf_pos, 0, 12);
    buf_pos += 12;
    p_buf[buf_pos + 0] = 0x00;
    p_buf[buf_pos + 1] = 0x01;
    p_buf[buf_pos + 2] = 0x00;
    p_buf[buf_pos + 2] = 0x00;
    buf_pos += 4;
    memset(p_buf + buf_pos, 0, 12);
    buf_pos += 12;
    p_buf[buf_pos + 0] = 0x40;
    p_buf[buf_pos + 1] = 0x00;
    p_buf[buf_pos + 2] = 0x00;
    p_buf[buf_pos + 2] = 0x00;
    buf_pos += 4;

    /* pre-defined */
    memset(p_buf + buf_pos, 0, 24);
    buf_pos += 24;

    /* next track id */
    p_buf[buf_pos + 0] = (metadata->trackid[TRACK_ID_VIDEO] >> 24) & 0xFF;
    p_buf[buf_pos + 1] = (metadata->trackid[TRACK_ID_VIDEO] >> 16) & 0xFF;
    p_buf[buf_pos + 2] = (metadata->trackid[TRACK_ID_VIDEO] >> 8) & 0xFF;
    p_buf[buf_pos + 3] = (metadata->trackid[TRACK_ID_VIDEO] >> 0) & 0xFF;
    buf_pos += 4;

    generate_fullbox_header(buf_pos, "mvhd", 0x0, 0x00, p_buf);

    fwrite(p_buf, 1, buf_pos, fp_mp4);

    return buf_pos;
}

