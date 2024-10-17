#include <string.h>

#include "mp4_box_header.h"

#include "hvcC.h"

int write_hvcC_box(FILE *fp_mp4, char *p_buf, metadata_t *metadata)
{
    uint32_t buf_pos = 0;

    /* 0~7 is [box header], generate last. */
    buf_pos += HEADER_SIZE_BOX;

    /* 8~ is [box data] */

    /* configuration version: fix 0x01 */
    p_buf[buf_pos+0] = 0x01;
    buf_pos += 1;

    /* general profile space (2 bits), tier flag (1 bit), general profile idc (5 bits) */
    p_buf[buf_pos+0] = 0x00; // 0x21
    buf_pos += 1;

    /* general profile compatibility flags */
    p_buf[buf_pos+0] = 0x00;
    p_buf[buf_pos+1] = 0x00;
    p_buf[buf_pos+2] = 0x00;
    p_buf[buf_pos+3] = 0x00;
    buf_pos += 4;

    /* general constraint indicator flags */
    p_buf[buf_pos+0] = 0x00;
    p_buf[buf_pos+1] = 0x00;
    p_buf[buf_pos+2] = 0x00;
    p_buf[buf_pos+3] = 0x00;
    p_buf[buf_pos+4] = 0x00;
    p_buf[buf_pos+5] = 0x00;
    buf_pos += 6;

    /* general level idc */
    p_buf[buf_pos+0] = 0x00;
    buf_pos += 1;

    /* 4bit reserved(0b1111) + 12bit min spatial segmentation idc */
    p_buf[buf_pos+0] = 0xF0;
    p_buf[buf_pos+1] = 0x00;
    buf_pos += 2;

    /* parallelism type */
    p_buf[buf_pos+0] = 0xFC;
    buf_pos += 1;

    /* chroma format idc */
    p_buf[buf_pos+0] = 0xFC;
    buf_pos += 1;

    /* bit depth luma */
    p_buf[buf_pos+0] = 0xF8;
    buf_pos += 1;

    /* bit depth chroma */
    p_buf[buf_pos+0] = 0xF8;
    buf_pos += 1;

    /* average frame rate */
    p_buf[buf_pos+0] = 0x00;
    p_buf[buf_pos+1] = 0x00;
    buf_pos += 2;

    /* constant frame rate (2 bit), num temporal layers (3 bits), temporal ID nested (1 bit), length size minus one (2 bits, always 3: nalu size is 4B) */
    p_buf[buf_pos+0] = 0x03;
    buf_pos += 1;

    /* NAL unit arrays: VPS+SPS+PPS */
    p_buf[buf_pos+0] = 0x03;
    buf_pos += 1;


    /* VPS(video parameter set) */
    p_buf[buf_pos+0] = 0xa0;
    buf_pos += 1;

    /* num of video parameter sets */
    p_buf[buf_pos+0] = 0x00;
    p_buf[buf_pos+1] = 0x01;
    buf_pos += 2;

    /* video parameter set length */
    p_buf[buf_pos+0] = (metadata->vps_len >> 8) & 0xFF;
    p_buf[buf_pos+1] = (metadata->vps_len >> 0) & 0xFF;
    buf_pos += 2;

    /* video parameter set NAL unit */
    memcpy(p_buf+buf_pos, metadata->vps, metadata->vps_len);
    buf_pos += metadata->vps_len;


    /* SPS(sequence parameter set) */
    p_buf[buf_pos+0] = 0xa1;
    buf_pos += 1;

    /* num of sequence parameter sets */
    p_buf[buf_pos+0] = 0x00;
    p_buf[buf_pos+1] = 0x01;
    buf_pos += 2;

    /* sequence parameter set length */
    p_buf[buf_pos+0] = (metadata->sps_len >> 8) & 0xFF;
    p_buf[buf_pos+1] = (metadata->sps_len >> 0) & 0xFF;
    buf_pos += 2;

    /* sequence parameter set NAL unit */
    memcpy(p_buf+buf_pos, metadata->sps, metadata->sps_len);
    buf_pos += metadata->sps_len;


    /* PPS(picture parameter set) */
    p_buf[buf_pos+0] = 0xa2;
    buf_pos += 1;

    /* num of picture parameter sets */
    p_buf[buf_pos+0] = 0x00;
    p_buf[buf_pos+1] = 0x01;
    buf_pos += 2;

    /* picture parameter set length */
    p_buf[buf_pos+0] = (metadata->pps_len >> 8) & 0xFF;
    p_buf[buf_pos+1] = (metadata->pps_len >> 0) & 0xFF;
    buf_pos += 2;

    /* picture parameter set NAL unit */
    memcpy(p_buf+buf_pos, metadata->pps, metadata->pps_len);
    buf_pos += metadata->pps_len;

    generate_box_header(buf_pos, "hvcC", p_buf);

    fwrite(p_buf, 1, buf_pos, fp_mp4);

    return buf_pos;
}

