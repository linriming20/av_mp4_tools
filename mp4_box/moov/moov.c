#include "mp4_box_header.h"

#include "moov.h"

#include "mvhd.h"
#include "trak.h"


int write_moov_box(FILE *fp_mp4, char *p_buf, metadata_t *metadata)
{
    uint32_t box_size = 0;
    uint64_t file_tail_pos = 0;

    /* 0~7 is [box header], generate last. */
    uint64_t box_header_pos = ftell(fp_mp4);
    box_size += HEADER_SIZE_BOX;
    fseek(fp_mp4, HEADER_SIZE_BOX, SEEK_CUR);

    /* 8~ is [box data] */

    box_size += write_mvhd_box(fp_mp4, p_buf, metadata);

    for(int track_id = 0; track_id < TRACK_ID_MAX; track_id++)
    {
        box_size += write_trak_box(fp_mp4, p_buf, metadata, track_id);
    }

    /* record file tail */
    file_tail_pos = ftell(fp_mp4);

    /* write box header */
    fseek(fp_mp4, box_header_pos, SEEK_SET);
    generate_box_header(box_size, "moov", p_buf);
    fwrite(p_buf, 1, HEADER_SIZE_BOX, fp_mp4);

    /* move to file tail */
    fseek(fp_mp4, file_tail_pos, SEEK_SET);

    /* must write mdat box before moov box, because it alloc memory first when writing mdat box */
    free_metadata_memory(metadata);

    return box_size;
}

