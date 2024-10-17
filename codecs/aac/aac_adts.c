#include "aac_adts.h"


int get_aac_adts_frame(FILE *fp, uint8_t *p_frame_data, adts_header_t *pt_header_info)
{
    uint32_t readBytes = 0;

    if(!fp || !p_frame_data || !pt_header_info)
        return -1;

    // ADTS header size is AAC_ADTS_HEADER_SIZE(=7) bytes
    readBytes = fread(p_frame_data, 1, AAC_ADTS_HEADER_SIZE, fp);
    if(readBytes <= 0)
        return -2;

    pt_header_info->syncword              = (p_frame_data[0] << 4 ) | (p_frame_data[1]  >> 4);
    pt_header_info->id                    = (p_frame_data[1] & 0x08) >> 3;
    pt_header_info->layer                 = (p_frame_data[1] & 0x06) >> 1;
    pt_header_info->protection_absent     =  p_frame_data[1] & 0x01;
    pt_header_info->profile               = (p_frame_data[2] & 0xc0) >> 6;
    pt_header_info->sampling_freq_index   = (p_frame_data[2] & 0x3c) >> 2;
    pt_header_info->private_bit           = (p_frame_data[2] & 0x02) >> 1;
    pt_header_info->channel_configuration = (((p_frame_data[2] & 0x01) << 2) | ((p_frame_data[3] & 0xc0) >> 6));
    pt_header_info->original_copy         = (p_frame_data[3] & 0x20) >> 5;
    pt_header_info->home                  = (p_frame_data[3] & 0x10) >> 4;
    pt_header_info->copyright_identification_bit   = (p_frame_data[3] & 0x08) >> 3;
    pt_header_info->copyright_identification_start = (p_frame_data[3] & 0x04) >> 2;
    pt_header_info->aac_frame_length = ((p_frame_data[3] & 0x03) << 11) |
                                            ((p_frame_data[4] & 0xFF) << 3) |
                                            ((p_frame_data[5] & 0xE0) >> 5);
    pt_header_info->adts_buffer_fullness = ((p_frame_data[5] & 0x1f) << 6 | (p_frame_data[6] & 0xfc) >> 2);
    pt_header_info->number_of_raw_data_blocks_in_frame = (p_frame_data[6] & 0x03);

    if (pt_header_info->syncword != 0xFFF)
        return -3;

    /* read the remaining frame of ADTS data outside the AAC_ADTS_HEADER_SIZE(=7) bytes header,
     * and it should be written after offsetting the header by AAC_ADTS_HEADER_SIZE(=7) bytes
     */
    readBytes = fread(p_frame_data + AAC_ADTS_HEADER_SIZE, 1, pt_header_info->aac_frame_length - AAC_ADTS_HEADER_SIZE, fp);
    if(readBytes <= 0)
        return -4;

    return 0;
}


int generate_aac_adts_header_v1(uint32_t data_size, uint32_t channels, uint32_t samplerate, uint8_t *p_out_buf, uint32_t *p_out_buf_len)
{
    int id = 0; // 0:MPEG_4
    int crc = 1;
    int profile = 1; // AAC LC
    int freq_idx = 0;

    if(!data_size || !p_out_buf || !p_out_buf_len)
    {
        printf("[%s:%d] Params invalid!\n", __FUNCTION__, __LINE__);
        return -1;
    }

    switch(samplerate)
    {
        case 96000: freq_idx = 0x0; break;
        case 88200: freq_idx = 0x1; break;
        case 64000: freq_idx = 0x2; break;
        case 48000: freq_idx = 0x3; break;
        case 44100: freq_idx = 0x4; break;
        case 32000: freq_idx = 0x5; break;
        case 24000: freq_idx = 0x6; break;
        case 22050: freq_idx = 0x7; break;
        case 16000: freq_idx = 0x8; break;
        case 12000: freq_idx = 0x9; break;
        case 11025: freq_idx = 0xa; break;
        case 8000 : freq_idx = 0xb; break;
        case 7350 : freq_idx = 0xc; break;
        default   : freq_idx = 0xd; break;
    }

    // fill in ADTS data
    p_out_buf[0] = (uint8_t) 0xFF;
    p_out_buf[1] = (uint8_t) (0xF0 | (id << 3) | crc);
    p_out_buf[2] = (uint8_t) ((profile << 6) |\
                            (freq_idx << 2)  |\
                            (channels >> 2));
    p_out_buf[3] = (uint8_t) (((channels & 0x03) << 6) | ((data_size & 0x1800) >> 11));
    p_out_buf[4] = (uint8_t) ((data_size & 0x7F8) >> 3);
    p_out_buf[5] = (uint8_t) (((data_size & 0x07) << 5) | 0x1F);
    p_out_buf[6] = (uint8_t) 0xFC;

    *p_out_buf_len = 7;

    return 0;
}

int generate_aac_adts_header_v2(uint32_t data_size, uint32_t channels, uint32_t freq_idx, uint8_t *p_out_buf, uint32_t *p_out_buf_len)
{
    int id = 0; // 0:MPEG_4
    int crc = 1;
    int profile = 1; // AAC LC

    if(!data_size || !p_out_buf || !p_out_buf_len)
    {
            printf("[%s:%d] Params invalid!\n", __FUNCTION__, __LINE__);
            return -1;
    }

    // fill in ADTS data
    p_out_buf[0] = (uint8_t) 0xFF;
    p_out_buf[1] = (uint8_t) (0xF0 | (id << 3) | crc);
    p_out_buf[2] = (uint8_t) ((profile << 6) |\
                            (freq_idx << 2)  |\
                            (channels >> 2));
    p_out_buf[3] = (uint8_t) (((channels & 0x03) << 6) | ((data_size & 0x1800) >> 11));
    p_out_buf[4] = (uint8_t) ((data_size & 0x7F8) >> 3);
    p_out_buf[5] = (uint8_t) (((data_size & 0x07) << 5) | 0x1F);
    p_out_buf[6] = (uint8_t) 0xFC;

    *p_out_buf_len = 7;

    return 0;
}


