#include <string.h>

#include "mp4_box_header.h"

#include "esds.h"


int write_esds_box(FILE *fp_mp4, char *p_buf, metadata_t *metadata)
{
    uint32_t buf_pos = 0;

    /* 0~11 is [box header], generate last. */
    buf_pos += HEADER_SIZE_FULLBOX;

    /* 12~ is [box data] */

    /* tag */
    p_buf[buf_pos+0] = 0x03;
    buf_pos += 1;

    /* length field */
    p_buf[buf_pos+0] = 0x19;
    buf_pos += 1;

    #if 1 /* id+priority: 2+1 */
    /* ES id */
    p_buf[buf_pos+0] = 0x00;
    p_buf[buf_pos+1] = 0x00;
    buf_pos += 2;

    /* streamDependenceFlag |  URL_Flag | OCRstreamFlag | streamPriority */
    p_buf[buf_pos+0] = 0x00;
    buf_pos += 1;
    #else
    memset(p_buf+buf_pos, 0x00, 3);
    buf_pos += 3;
    #endif

    /* DecoderConfigDescriptor tag */
    p_buf[buf_pos+0] = 0x04;
    buf_pos += 1;

    /* length field 1 */
    p_buf[buf_pos+0] = 0x11;
    buf_pos += 1;

    /* objectTypeIndication */
    p_buf[buf_pos+0] = 0x40; // CODEC_ID_AAC
    buf_pos += 1;

    #if 1
    /* streamType | upStream | reserved */
    p_buf[buf_pos+0] = 0x15;
    buf_pos += 1;

    /* bufferSizeDB */
    p_buf[buf_pos+0] = 0x00;
    p_buf[buf_pos+1] = 0x01;
    p_buf[buf_pos+2] = 0xD5;
    buf_pos += 3;

    /* maxBitrate */
    p_buf[buf_pos+0] = 0x00;
    p_buf[buf_pos+1] = 0x01;
    p_buf[buf_pos+2] = 0xF4;
    p_buf[buf_pos+3] = 0xF8;
    buf_pos += 4;

    /* avgBitrate */
    p_buf[buf_pos+0] = 0x00;
    p_buf[buf_pos+1] = 0x01;
    p_buf[buf_pos+2] = 0x73;
    p_buf[buf_pos+3] = 0xF8;
    buf_pos += 4;
    #else
    /* resved: 1+3+4+4 */
    memset(p_buf+buf_pos, 0x00, 12);
    buf_pos += 12;
    #endif

    /* dec specific info tag */
    p_buf[buf_pos+0] = 0x05;
    buf_pos += 1;

    /* descr length */
    p_buf[buf_pos+0] = 0x02;
    buf_pos += 1;

    /* descr data*/
    /* audioObjectType_2_GASpecificConfig : 5 | samplingFrequencyIndex :4 | channelConfiguration : 4 | cpConfig : 2 | directMapping : 1 */
    p_buf[buf_pos+0] = (0x01 << 3) | (metadata->audio_samplerate_idx >> 1);
    p_buf[buf_pos+1] = ((metadata->audio_samplerate_idx & 0x01) << 7) | (metadata->audio_channel_cnt << 3) | 0x00 | 0x00;
    buf_pos += 2;

    /* SLConfigDescrTag */
    p_buf[buf_pos+0] = 0x06;
    buf_pos += 1;

    /* descr length */
    p_buf[buf_pos+0] = 0x01;
    buf_pos += 1;

    /* pre-defined */
    p_buf[buf_pos+0] = 0x02;
    buf_pos += 1;

    generate_fullbox_header(buf_pos, "esds", 0x00, 0x00, p_buf);

    fwrite(p_buf, 1, buf_pos, fp_mp4);

    return buf_pos;
}


