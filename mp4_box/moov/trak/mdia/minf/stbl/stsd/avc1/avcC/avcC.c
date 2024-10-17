#include <string.h>

#include "mp4_box_header.h"

#include "avcC.h"

int write_avcC_box(FILE *fp_mp4, char *p_buf, metadata_t *metadata)
{
    uint32_t buf_pos = 0;

    /* 0~7 is [box header], generate last. */
    buf_pos += HEADER_SIZE_BOX;

    /* 8~ is [box data] */

    /* configuration version */
    p_buf[buf_pos+0] = 0x01;
    buf_pos += 1;

    /* avc profile indication: 66-Baseline Profile 77-Main Profile 100-High Profile */
    p_buf[buf_pos+0] = metadata->sps[1];
    buf_pos += 1;

    /* profile compatibility */
    p_buf[buf_pos+0] = metadata->sps[2];
    buf_pos += 1;

    /* avc level indication: 31-Level 3.1 40-Level 4.0 */
    p_buf[buf_pos+0] = metadata->sps[3];
    buf_pos += 1;

    /* reserved_1 length size minus one */
    p_buf[buf_pos+0] = 0xFF;
    buf_pos += 1;

    /* reserved_2 num of sequence parameter sets */
    p_buf[buf_pos+0] = 0xE1;
    buf_pos += 1;

    /* sequence parameter set length */
    p_buf[buf_pos+0] = (metadata->sps_len >> 8) & 0xFF;
    p_buf[buf_pos+1] = (metadata->sps_len >> 0) & 0xFF;
    buf_pos += 2;

    /* sequence parameter set NAL unit */
    memcpy(p_buf+buf_pos, metadata->sps, metadata->sps_len);
    buf_pos += metadata->sps_len;

    /* num of picture parameter sets */
    p_buf[buf_pos+0] = 0x01;
    buf_pos += 1;

    /* picture parameter set length */
    p_buf[buf_pos+0] = (metadata->pps_len >> 8) & 0xFF;
    p_buf[buf_pos+1] = (metadata->pps_len >> 0) & 0xFF;
    buf_pos += 2;

    /* picture parameter set NAL unit */
    memcpy(p_buf+buf_pos, metadata->pps, metadata->pps_len);
    buf_pos += metadata->pps_len;

    generate_box_header(buf_pos, "avcC", p_buf);

    fwrite(p_buf, 1, buf_pos, fp_mp4);

    return buf_pos;
}




