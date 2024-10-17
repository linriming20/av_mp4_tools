#include <string.h>

#include "mp4_box_header.h"

#include "hvc1.h"

#include "hvcC.h"


int write_hvc1_box(FILE *fp_mp4, char *p_buf, metadata_t *metadata)
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

    /* pre-defined */
    p_buf[buf_pos+0] = 0x00;
    p_buf[buf_pos+1] = 0x00;
    buf_pos += 2;

    /* reserved[2] */
    memset(p_buf + buf_pos, 0, 2);
    buf_pos += 2;

    /* pre-defined[12] */
    memset(p_buf + buf_pos, 0, 12);
    buf_pos += 12;

    /* width */
    p_buf[buf_pos+0] = (metadata->video_width >> 8) & 0xFF;
    p_buf[buf_pos+1] = (metadata->video_width >> 0) & 0xFF;
    buf_pos += 2;

    /* hieght */
    p_buf[buf_pos+0] = (metadata->video_height >> 8) & 0xFF;
    p_buf[buf_pos+1] = (metadata->video_height >> 0) & 0xFF;
    buf_pos += 2;

    /* horiz resolution 0x00480000: 72 DPI */
    p_buf[buf_pos+0] = 0x00;
    p_buf[buf_pos+1] = 0x48;
    p_buf[buf_pos+2] = 0x00;
    p_buf[buf_pos+3] = 0x00;
    buf_pos += 4;

    /* vert resolution 0x00480000: 72 DPI */
    p_buf[buf_pos+0] = 0x00;
    p_buf[buf_pos+1] = 0x48;
    p_buf[buf_pos+2] = 0x00;
    p_buf[buf_pos+3] = 0x00;
    buf_pos += 4;

    /* reserved[4] */
    memset(p_buf + buf_pos, 0, 4);
    buf_pos += 4;

    /* frame count */
    p_buf[buf_pos+0] = 0x00;
    p_buf[buf_pos+1] = 0x01;
    buf_pos += 2;

    /* compressor name */
    memset(p_buf + buf_pos, 0, 32);
    buf_pos += 32;

    /* depth 0x0018 */
    p_buf[buf_pos+0] = 0x00;
    p_buf[buf_pos+1] = 0x18;
    buf_pos += 2;

    /* pre-defined[2]: -1 */
    memset(p_buf + buf_pos, 0xFF, 2);
    buf_pos += 2;

    /* write it */
    fwrite(p_buf+HEADER_SIZE_BOX, 1, buf_pos-HEADER_SIZE_BOX, fp_mp4);

    buf_pos += write_hvcC_box(fp_mp4, p_buf, metadata);

    /* record file tail */
    file_tail_pos = ftell(fp_mp4);

    /* write box header */
    fseek(fp_mp4, box_header_pos, SEEK_SET);
    generate_box_header(buf_pos, "hvc1", p_buf);
    fwrite(p_buf, 1, HEADER_SIZE_BOX, fp_mp4);

    /* move to file tail */
    fseek(fp_mp4, file_tail_pos, SEEK_SET);

    return buf_pos;
}

