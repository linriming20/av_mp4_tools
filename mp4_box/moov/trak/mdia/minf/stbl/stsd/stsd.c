#include <string.h>

#include "mp4_box_header.h"

#include "stsd.h"

#include "avc1.h"
#include "hvc1.h"
#include "mp4a.h"

int write_stsd_box(FILE *fp_mp4, char *p_buf, metadata_t *metadata, track_id_e track_id)
{
    uint32_t buf_pos = 0;
    uint64_t file_tail_pos = 0;

    /* 0~11 is [box header], generate last. */
    uint64_t box_header_pos = ftell(fp_mp4);
    buf_pos += HEADER_SIZE_FULLBOX;
    fseek(fp_mp4, HEADER_SIZE_FULLBOX, SEEK_CUR);

    /* 12~ is [box data] */

    /* entry count */
    p_buf[buf_pos+0] = 0x00;
    p_buf[buf_pos+1] = 0x00;
    p_buf[buf_pos+2] = 0x00;
    p_buf[buf_pos+3] = 0x01;
    buf_pos += 4;

    fwrite(p_buf+HEADER_SIZE_FULLBOX, 1, buf_pos-HEADER_SIZE_FULLBOX, fp_mp4);

    switch(track_id)
    {
        case TRACK_ID_AUDIO:
            buf_pos += write_mp4a_box(fp_mp4, p_buf, metadata);
            break;
        case TRACK_ID_VIDEO:
            if(metadata->video_type == VIDEO_TYPE_H264)
                buf_pos += write_avc1_box(fp_mp4, p_buf, metadata);
            else if(metadata->video_type == VIDEO_TYPE_H265)
                buf_pos += write_hvc1_box(fp_mp4, p_buf, metadata);
            break;
    }

    /* record file tail */
    file_tail_pos = ftell(fp_mp4);

    /* write box header */
    fseek(fp_mp4, box_header_pos, SEEK_SET);
    generate_fullbox_header(buf_pos, "stsd", 0x00, 0x00, p_buf);
    fwrite(p_buf, 1, HEADER_SIZE_FULLBOX, fp_mp4);

    /* move to file tail */
    fseek(fp_mp4, file_tail_pos, SEEK_SET);

    return buf_pos;
}


