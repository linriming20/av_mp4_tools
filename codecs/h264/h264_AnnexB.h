#ifndef __H264_ANNEXB_H__
#define __H264_ANNEXB_H__

#include <stdio.h>
#include <stdint.h>



/* the nalu type of H264 */
typedef enum{
    H264_NALU_UNKNOWN = 0,
    H264_NALU_PSLICE      = 1,
    H264_NALU_SLICE_DPA   = 2,
    H264_NALU_SLICE_DPB   = 3,
    H264_NALU_SLICE_DPC   = 4,
    H264_NALU_ISLICE      = 5,
    H264_NALU_SEI         = 6,
    H264_NALU_SPS         = 7,
    H264_NALU_PPS         = 8,
    H264_NALU_AU_DELIMITER= 9,
    /* ref_idc == 0 for 6,9,10,11,12 */
    H264_NALU_END_OF_SEQ  = 10,
    H264_NALU_END_OF_STREAM = 11,
    H264_NALU_FILLER_DATA = 12,
    /* 13-23 */
    H264_NALU_SPS_EXT     = 13,
    H264_NALU_PREFIX      = 14,
    H264_NALU_SUBSET_SPS  = 15,
    H264_NALU_DEPTH_PS    = 16,
    H264_NALU_RESERVED_17 = 17,
    H264_NALU_RESERVED_18 = 18,
    H264_NALU_SLICE_WP    = 19,
    H264_NALU_SLICE_EXT   = 20,
    H264_NALU_SLICE_3D_EXT= 21,
    H264_NALU_RESERVED_22 = 22,
    H264_NALU_RESERVED_23 = 23,

    H264_NALU_BUTT
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


/************************************************************************
 * function describe: get one frame from h264 file.
 * params:
 *   [fp]: H.264 file handler.(in)
 *   [p_frame_data]: the function will fill the frame data in it, must be
 *                alloced memory before call this function.(out)
 *   [frame_size]: the frame size in data.(out)
 *   [is_keyframe]: is the sync frame.(out)
 * return: 0:success other:error
 ************************************************************************/
int get_one_H264_frame(FILE *fp, unsigned char *p_frame_data, uint32_t *frame_size, uint32_t *is_keyframe);

#endif /* __H264_ANNEXB_H__ */
