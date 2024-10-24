#include <string.h>

#include "mp4_box_header.h"

#include "co64.h"

int write_co64_box(FILE *fp_mp4, char *p_buf, metadata_t *metadata, track_id_e track_id)
{
    uint32_t buf_pos = 0;

    /* 0~11 is [box header], generate last. */
    buf_pos += HEADER_SIZE_FULLBOX;

    /* 12~ is [box data] */

    /* entry count */
    p_buf[buf_pos+0] = (metadata->co64_entry_count[track_id] >> 24) & 0xFF;
    p_buf[buf_pos+1] = (metadata->co64_entry_count[track_id] >> 16) & 0xFF;
    p_buf[buf_pos+2] = (metadata->co64_entry_count[track_id] >> 8)  & 0xFF;
    p_buf[buf_pos+3] = (metadata->co64_entry_count[track_id] >> 0)  & 0xFF;
    buf_pos += 4;

    for(int i = 0; i < metadata->co64_entry_count[track_id]; i++)
    {
        p_buf[buf_pos+0] = (metadata->co64_chunk_offset[track_id][i] >> 56) & 0xFF;
        p_buf[buf_pos+1] = (metadata->co64_chunk_offset[track_id][i] >> 48) & 0xFF;
        p_buf[buf_pos+2] = (metadata->co64_chunk_offset[track_id][i] >> 40) & 0xFF;
        p_buf[buf_pos+3] = (metadata->co64_chunk_offset[track_id][i] >> 32) & 0xFF;
        p_buf[buf_pos+4] = (metadata->co64_chunk_offset[track_id][i] >> 24) & 0xFF;
        p_buf[buf_pos+5] = (metadata->co64_chunk_offset[track_id][i] >> 16) & 0xFF;
        p_buf[buf_pos+6] = (metadata->co64_chunk_offset[track_id][i] >> 8)  & 0xFF;
        p_buf[buf_pos+7] = (metadata->co64_chunk_offset[track_id][i] >> 0)  & 0xFF;
        buf_pos += 8;
    }

    generate_fullbox_header(buf_pos, "co64", 0x0, 0x00, p_buf);

    fwrite(p_buf, 1, buf_pos, fp_mp4);

    return buf_pos;
}


