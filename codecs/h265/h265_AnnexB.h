#ifndef __H265_ANNEXB_H__
#define __H265_ANNEXB_H__

#include <stdio.h>
#include <stdint.h>



/* the nalu type of H265 */
typedef enum{
    H265_NALU_TRAIL_N    = 0,
    H265_NALU_PSLICE     = 1, /* PSLICE types, vcl */
    H265_NALU_TSA_N      = 2,
    H265_NALU_TSA_R      = 3,
    H265_NALU_STSA_N     = 4,
    H265_NALU_STSA_R     = 5,
    H265_NALU_RADL_N     = 6,
    H265_NALU_RADL_R     = 7,
    H265_NALU_RASL_N     = 8,
    H265_NALU_RASL_R     = 9,
    H265_NALU_VCL_N10    = 10,
    H265_NALU_VCL_R11    = 11,
    H265_NALU_VCL_N12    = 12,
    H265_NALU_VCL_R13    = 13,
    H265_NALU_VCL_N14    = 14,
    H265_NALU_VCL_R15    = 15,
    H265_NALU_BLA_W_LP   = 16,  /* I Frame */
    H265_NALU_BLA_W_RADL = 17,  /* I Frame */
    H265_NALU_BLA_N_LP   = 18,  /* I Frame */
    H265_NALU_ISLICE     = 19,  /* I Frame */
    H265_NALU_IDR_N_LP   = 20,  /* I Frame */
    H265_NALU_CRA_NUT    = 21,  /* I Frame */
    H265_NALU_IRAP_VCL22 = 22,
    H265_NALU_IRAP_VCL23 = 23,
    H265_NALU_RSV_VCL24  = 24,
    H265_NALU_RSV_VCL25  = 25,
    H265_NALU_RSV_VCL26  = 26,
    H265_NALU_RSV_VCL27  = 27,
    H265_NALU_RSV_VCL28  = 28,
    H265_NALU_RSV_VCL29  = 29,
    H265_NALU_RSV_VCL30  = 30,
    H265_NALU_RSV_VCL31  = 31,
    H265_NALU_VPS        = 32,  /* VPS types, none-vcl */
    H265_NALU_SPS        = 33,  /* SPS types */
    H265_NALU_PPS        = 34,  /* PPS types */
    H265_NALU_AUD        = 35,
    H265_NALU_EOS_NUT    = 36,
    H265_NALU_EOB_NUT    = 37,
    H265_NALU_FD_NUT     = 38,
    H265_NALU_SEI_PREFIX = 39,  /* SEI types */
    H265_NALU_SEI_SUFFIX = 40,  /* SEI types */
    H265_NALU_RSV_NVCL41 = 41,
    H265_NALU_RSV_NVCL42 = 42,
    H265_NALU_RSV_NVCL43 = 43,
    H265_NALU_RSV_NVCL44 = 44,
    H265_NALU_RSV_NVCL45 = 45,
    H265_NALU_RSV_NVCL46 = 46,
    H265_NALU_RSV_NVCL47 = 47,
    H265_NALU_UNSPEC48   = 48,
    H265_NALU_UNSPEC49   = 49,
    H265_NALU_UNSPEC50   = 50,
    H265_NALU_UNSPEC51   = 51,
    H265_NALU_UNSPEC52   = 52,
    H265_NALU_UNSPEC53   = 53,
    H265_NALU_UNSPEC54   = 54,
    H265_NALU_UNSPEC55   = 55,
    H265_NALU_UNSPEC56   = 56,
    H265_NALU_UNSPEC57   = 57,
    H265_NALU_UNSPEC58   = 58,
    H265_NALU_UNSPEC59   = 59,
    H265_NALU_UNSPEC60   = 60,
    H265_NALU_UNSPEC61   = 61,
    H265_NALU_UNSPEC62   = 62,
    H265_NALU_UNSPEC63   = 63,
    H265_NALU_BUTT
}h265_nalu_type_e;



typedef struct{
    uint32_t data_len; /* the length of this nalu.(include start code) */
    uint32_t startcode_len;
    h265_nalu_type_e nalu_type;
}h265_nalu_info_t, *pt_h265_nalu_info;



/************************************************************************
 * function describe: get one AnnexB nalu(include start code) from h265 file.
 * params:
 *   [fp]: H.265 file handler.(in)
 *   [p_nalu_data]: the function will fill the nalu data in it, must be
 *                alloced memory before call this function.(out)
 *   [pt_nalu_info]: the information in this nalu.(out)
 * return: 0:success other:error
 ************************************************************************/
int get_one_H265_AnnexB_nalu(FILE *fp, uint8_t *p_nalu_data, h265_nalu_info_t *pt_nalu_info);


/************************************************************************
 * function describe: get one frame from h265 file.
 * params:
 *   [fp]: H.265 file handler.(in)
 *   [p_frame_data]: the function will fill the frame data in it, must be
 *                alloced memory before call this function.(out)
 *   [frame_size]: the frame size in data.(out)
 *   [is_keyframe]: is the sync frame.(out)
 * return: 0:success other:error
 ************************************************************************/
int get_one_H265_frame(FILE *fp, unsigned char *p_frame_data, uint32_t *frame_size, uint32_t *is_keyframe);

#endif /* __H265_ANNEXB_H__ */
