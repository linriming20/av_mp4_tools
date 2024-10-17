#include <string.h>

#include "mp4_box_header.h"

#include "stss.h"

int write_stss_box(FILE *fp_mp4, char *p_buf, metadata_t *metadata)
{
    uint32_t buf_pos = 0;

    /* 0~11 is [box header], generate last. */
    buf_pos += HEADER_SIZE_FULLBOX;

    /* 12~ is [box data] */

    /* entry count */
    p_buf[buf_pos+0] = (metadata->stss_entry_count >> 24) & 0xFF;
    p_buf[buf_pos+1] = (metadata->stss_entry_count >> 16) & 0xFF;
    p_buf[buf_pos+2] = (metadata->stss_entry_count >> 8)  & 0xFF;
    p_buf[buf_pos+3] = (metadata->stss_entry_count >> 0)  & 0xFF;
    buf_pos += 4;

    for(int i = 0; i < metadata->stss_entry_count; i++)
    {
        p_buf[buf_pos+0] = (metadata->stss_sample_number[i] >> 24) & 0xFF;
        p_buf[buf_pos+1] = (metadata->stss_sample_number[i] >> 16) & 0xFF;
        p_buf[buf_pos+2] = (metadata->stss_sample_number[i] >> 8)  & 0xFF;
        p_buf[buf_pos+3] = (metadata->stss_sample_number[i] >> 0)  & 0xFF;
        buf_pos += 4;
    }

    generate_fullbox_header(buf_pos, "stss", 0x0, 0x00, p_buf);

    fwrite(p_buf, 1, buf_pos, fp_mp4);

    return buf_pos;
}

