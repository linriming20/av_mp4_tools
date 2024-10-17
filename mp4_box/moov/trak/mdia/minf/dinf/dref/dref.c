#include <string.h>

#include "mp4_box_header.h"

#include "dref.h"

#include "url.h"


int write_dref_box(FILE *fp_mp4, char *p_buf)
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

    buf_pos += write_url_box(fp_mp4, p_buf);

    /* record file tail */
    file_tail_pos = ftell(fp_mp4);

    /* write box header */
    fseek(fp_mp4, box_header_pos, SEEK_SET);
    generate_fullbox_header(buf_pos, "dref", 0x00, 0x00, p_buf);
    fwrite(p_buf, 1, HEADER_SIZE_FULLBOX, fp_mp4);

    /* move to file tail */
    fseek(fp_mp4, file_tail_pos, SEEK_SET);

    return buf_pos;
}
