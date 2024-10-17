#include <string.h>

#include "mp4_box_header.h"

#include "stbl.h"

#include "stsz.h"
#include "stsd.h"
#include "stts.h"
#include "stss.h"
#include "stsc.h"
#include "stco.h"


int write_stbl_box(FILE *fp_mp4, char *p_buf, metadata_t *metadata, track_id_e track_id)
{
    uint32_t buf_pos = 0;
    uint64_t file_tail_pos = 0;

    /* 0~7 is [box header], generate last. */
    uint64_t box_header_pos = ftell(fp_mp4);
    buf_pos += HEADER_SIZE_BOX;
    fseek(fp_mp4, HEADER_SIZE_BOX, SEEK_CUR);

    /* 8~ is [box data] */
    buf_pos += write_stsz_box(fp_mp4, p_buf, metadata, track_id);

    buf_pos += write_stsd_box(fp_mp4, p_buf, metadata, track_id);

    buf_pos += write_stts_box(fp_mp4, p_buf, metadata, track_id);

    if(track_id == TRACK_ID_VIDEO)
    {
        buf_pos += write_stss_box(fp_mp4, p_buf, metadata);
    }

    buf_pos += write_stsc_box(fp_mp4, p_buf, metadata, track_id);

    buf_pos += write_stco_box(fp_mp4, p_buf, metadata, track_id);

    /* record file tail */
    file_tail_pos = ftell(fp_mp4);

    /* write box header */
    fseek(fp_mp4, box_header_pos, SEEK_SET);
    generate_box_header(buf_pos, "stbl", p_buf);
    fwrite(p_buf, 1, HEADER_SIZE_BOX, fp_mp4);

    /* move to file tail */
    fseek(fp_mp4, file_tail_pos, SEEK_SET);

    return buf_pos;
}

