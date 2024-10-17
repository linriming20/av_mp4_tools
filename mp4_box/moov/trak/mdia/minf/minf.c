#include "mp4_box_header.h"

#include "minf.h"

#include "vmhd.h"
#include "smhd.h"
#include "dinf.h"
#include "stbl.h"


int write_minf_box(FILE *fp_mp4, char *p_buf, metadata_t *metadata, track_id_e track_id)
{
    uint32_t buf_pos = 0;
    uint64_t file_tail_pos = 0;

    /* 0~7 is [box header], generate last. */
    uint64_t box_header_pos = ftell(fp_mp4);
    buf_pos += HEADER_SIZE_BOX;
    fseek(fp_mp4, HEADER_SIZE_BOX, SEEK_CUR);

    /* 8~ is [box data] */
    switch(track_id)
    {
        case TRACK_ID_AUDIO:
            buf_pos += write_smhd_box(fp_mp4, p_buf);
            break;
        case TRACK_ID_VIDEO:
            buf_pos += write_vmhd_box(fp_mp4, p_buf);
            break;
    }

    buf_pos += write_dinf_box(fp_mp4, p_buf);

    buf_pos += write_stbl_box(fp_mp4, p_buf, metadata, track_id);

    /* record file tail */
    file_tail_pos = ftell(fp_mp4);

    /* write box header */
    fseek(fp_mp4, box_header_pos, SEEK_SET);
    generate_box_header(buf_pos, "minf", p_buf);
    fwrite(p_buf, 1, HEADER_SIZE_BOX, fp_mp4);

    /* move to file tail */
    fseek(fp_mp4, file_tail_pos, SEEK_SET);

    return buf_pos;
}

