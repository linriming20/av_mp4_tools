#include <string.h>

#include "mp4_box_header.h"

#include "tkhd.h"

int write_tkhd_box(FILE *fp_mp4, char *p_buf, metadata_t *metadata, track_id_e track_id)
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

    /* track id */
    p_buf[buf_pos+0] = (metadata->trackid[track_id] >> 24) & 0xFF;
    p_buf[buf_pos+1] = (metadata->trackid[track_id] >> 16) & 0xFF;
    p_buf[buf_pos+2] = (metadata->trackid[track_id] >> 8)  & 0xFF;
    p_buf[buf_pos+3] = (metadata->trackid[track_id] >> 0)  & 0xFF;
    buf_pos += 4;

    /* reserved */
    memset(p_buf + buf_pos, 0, 4);
    buf_pos += 4;

    /* duration */
    p_buf[buf_pos+0] = (metadata->duration[track_id] >> 24) & 0xFF;
    p_buf[buf_pos+1] = (metadata->duration[track_id] >> 16) & 0xFF;
    p_buf[buf_pos+2] = (metadata->duration[track_id] >> 8)  & 0xFF;
    p_buf[buf_pos+3] = (metadata->duration[track_id] >> 0)  & 0xFF;
    buf_pos += 4;

    /* reserved */
    memset(p_buf + buf_pos, 0, 8);
    buf_pos += 8;

    /* layer (default zero) */
    p_buf[buf_pos+0] = 0x00;
    p_buf[buf_pos+1] = 0x00;
    buf_pos += 2;

    /* alternet group */
    p_buf[buf_pos+0] = 0x00;
    p_buf[buf_pos+1] = 0x00;
    buf_pos += 2;

    /* volume if track is audio 0x0100 else 0 */
    if(track_id == TRACK_ID_AUDIO)
    {
        p_buf[buf_pos+0] = 0x01;
        p_buf[buf_pos+1] = 0x00;
        buf_pos += 2;
    }
    else
    {
        p_buf[buf_pos+0] = 0x00;
        p_buf[buf_pos+1] = 0x00;
        buf_pos += 2;
    }

    /* reserved */
    memset(p_buf + buf_pos, 0, 2);
    buf_pos += 2;

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

    /* width */
    p_buf[buf_pos+0] = (metadata->video_width >> 24) & 0xFF;
    p_buf[buf_pos+1] = (metadata->video_width >> 16) & 0xFF;
    p_buf[buf_pos+2] = (metadata->video_width >> 8)  & 0xFF;
    p_buf[buf_pos+3] = (metadata->video_width >> 0)  & 0xFF;
    buf_pos += 4;

    /* height */
    p_buf[buf_pos+0] = (metadata->video_height >> 24) & 0xFF;
    p_buf[buf_pos+1] = (metadata->video_height >> 16) & 0xFF;
    p_buf[buf_pos+2] = (metadata->video_height >> 8)  & 0xFF;
    p_buf[buf_pos+3] = (metadata->video_height >> 0)  & 0xFF;
    buf_pos += 4;

    generate_fullbox_header(buf_pos, "tkhd", 0x00, 0x07, p_buf);

    fwrite(p_buf, 1, buf_pos, fp_mp4);

    return buf_pos;
}


