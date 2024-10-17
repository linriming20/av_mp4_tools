#ifndef __H264_ANNEXB_H__
#define __H264_ANNEXB_H__

#include <stdio.h>
#include <stdint.h>



typedef enum{
    NALU_TYPE_SLICE    = 0x1,
    NALU_TYPE_DPA      = 0x2,
    NALU_TYPE_DPB      = 0x3,
    NALU_TYPE_DPC      = 0x4,
    NALU_TYPE_IDR      = 0x5,
    NALU_TYPE_SEI      = 0x6,
    NALU_TYPE_SPS      = 0x7,
    NALU_TYPE_PPS      = 0x8,
    NALU_TYPE_AUD      = 0x9,
    NALU_TYPE_EOSEQ    = 0xa,
    NALU_TYPE_EOSTREAM = 0xb,
    NALU_TYPE_FILL     = 0xc,
}h264_nalu_type_e;



typedef struct{
    uint32_t data_len; /* the length of this nalu.(include start code) */
    uint32_t startcode_len;
    h264_nalu_type_e nalu_type;
}h264_nalu_info_t, *pt_h264_nalu_info;



/************************************************************************
 * function describe: get one AnnexB nalu(include start code) from h264 file.
 * params:
 *   [fp]: H.264 file handler.(in)
 *   [p_nalu_data]: the function will fill the nalu data in it, must be
 *                alloced memory before call this function.(out)
 *   [pt_nalu_info]: the information in this nalu.(out)
 * return: 0:success other:error
 ************************************************************************/
int get_one_H264_AnnexB_nalu(FILE *fp, uint8_t *p_nalu_data, h264_nalu_info_t *pt_nalu_info);


#endif /* __H264_ANNEXB_H__ */
