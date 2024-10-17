#include <string.h>

#include "mp4_box_header.h"

#include "mp4a.h"

#include "esds.h"


int write_mp4a_box(FILE *fp_mp4, char *p_buf, metadata_t *metadata)
{
    uint32_t buf_pos = 0;
    uint64_t file_tail_pos = 0;

    /* 0~7 is [box header], generate last. */
    uint64_t box_header_pos = ftell(fp_mp4);
    buf_pos += HEADER_SIZE_BOX;
    fseek(fp_mp4, HEADER_SIZE_BOX, SEEK_CUR);

    /* 8~ is [box data] */
    /* reserved[6] */
    memset(p_buf + buf_pos, 0, 6);
    buf_pos += 6;

    /* data reference index */
    p_buf[buf_pos+0] = 0x00;
    p_buf[buf_pos+1] = 0x01;
    buf_pos += 2;

    /* reserved[8] */
    memset(p_buf + buf_pos, 0, 8);
    buf_pos += 8;

    /* channel cnt */
    p_buf[buf_pos+0] = (metadata->audio_channel_cnt >> 8) & 0xFF;
    p_buf[buf_pos+1] = (metadata->audio_channel_cnt >> 0) & 0xFF;
    buf_pos += 2;

    /* sample size: set to 16 */
    p_buf[buf_pos+0] = 0x00;
    p_buf[buf_pos+1] = 0x10;
    buf_pos += 2;

    /* reserved[4] */
    memset(p_buf + buf_pos, 0, 4);
    buf_pos += 4;

    /* samplerate = {timescale of media}<<16; */
    p_buf[buf_pos+0] = (metadata->video_width >> 8) & 0xFF;
    p_buf[buf_pos+1] = (metadata->video_width >> 0) & 0xFF;
    p_buf[buf_pos+2] = 0x00;
    p_buf[buf_pos+3] = 0x00;
    buf_pos += 4;

    /* write it now */
    fwrite(p_buf+HEADER_SIZE_BOX, 1, buf_pos-HEADER_SIZE_BOX, fp_mp4);

    buf_pos += write_esds_box(fp_mp4, p_buf, metadata);

    /* record file tail */
    file_tail_pos = ftell(fp_mp4);

    /* write box header */
    fseek(fp_mp4, box_header_pos, SEEK_SET);
    generate_box_header(buf_pos, "mp4a", p_buf);
    fwrite(p_buf, 1, HEADER_SIZE_BOX, fp_mp4);

    /* move to file tail */
    fseek(fp_mp4, file_tail_pos, SEEK_SET);

    return buf_pos;
}

