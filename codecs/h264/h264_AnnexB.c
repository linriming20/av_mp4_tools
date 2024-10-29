#include "h264_AnnexB.h"

#include "debug.h"

int get_one_H264_AnnexB_nalu(FILE *fp, uint8_t *p_nalu_data, h264_nalu_info_t *pt_nalu_info)
{
    uint32_t readBytes = 0;
    uint32_t pos = 0;

    if(!fp || !p_nalu_data || !pt_nalu_info)
        return -1;

    if((readBytes = fread(p_nalu_data, 1, 4, fp)) <= 0)
        return -2;

    // judge the type of NALU start code
    if(p_nalu_data[0] == 0 && p_nalu_data[1] == 0 && p_nalu_data[2] == 0 && p_nalu_data[3] == 1)
    {
        pos = 4; // start by p_nalu_data[4]
        pt_nalu_info->startcode_len = 4;
    }
    else if(p_nalu_data[0] == 0 && p_nalu_data[1] == 0 && p_nalu_data[2] == 1)
    {
        pos = 3;
        pt_nalu_info->startcode_len = 3;
        fseek(fp, -1, SEEK_CUR); // if start code type is 3, need to adapt point
    }
    else
        return -3;

    // find next NALU
    while(1)
    {
        int val = 0;
        if((val = fgetc(fp)) != EOF)
        {
            p_nalu_data[pos] = (unsigned char)val;
        }
        else
        {
            // file end, pos should not add 1 in last loop
            pos -= 1;
            break;
        }

        /* judge the start code type of "00 00 00 01" or "00 00 01",
         * and must judge the "00 00 00 01", as it include the position of "00 00 01"
         */
        if(p_nalu_data[pos-3] == 0 && p_nalu_data[pos-2] == 0 && p_nalu_data[pos-1] == 0 && p_nalu_data[pos] == 1)
        {
            fseek(fp, -4, SEEK_CUR);
            pos -= 4;
            break;
        }
        else if(p_nalu_data[pos-2] == 0 && p_nalu_data[pos-1] == 0 && p_nalu_data[pos] == 1)
        {
            fseek(fp, -3, SEEK_CUR);
            pos -= 3;
            break;
        }
        pos++;
    }

    // NALU length = array index + 1
    pt_nalu_info->data_len = pos+1;
    pt_nalu_info->nalu_type = p_nalu_data[pt_nalu_info->startcode_len] & 0x1F;

    return 0;
}


int get_one_H264_frame(FILE *fp, unsigned char *p_frame_data, uint32_t *frame_size, uint32_t *is_keyframe)
{
    int readBytes = 0;
    unsigned int pos = 0;
    unsigned int nalu_start_pos = 0;
    char nalu_type = 0;
    int is_idr_frame = 0;
    int find_next_nalu = 0;
    int next_nalu_startcode_len = 0;
    static unsigned long nalu_index = 1;
    static unsigned long frame_index = 0;

    if(!fp)
        return -1;

    if((readBytes = fread(p_frame_data, 1, 4, fp)) <= 0)
        return -1;

    if(p_frame_data[0] == 0 && p_frame_data[1] == 0 && p_frame_data[2] == 0 && p_frame_data[3] == 1)
    {
        pos = 4;
    }
    else
    {
        return -1;
    }

    fread(&nalu_type, 1, 1, fp);
    fseek(fp, -1, SEEK_CUR);
    if((nalu_type & 0x1F) == H264_NALU_SEI ||\
       (nalu_type & 0x1F) == H264_NALU_SPS)
    {
        is_idr_frame = 1;
    }

    while(1)
    {
        int val = 0;
        if((val = fgetc(fp)) != EOF)
        {
            p_frame_data[pos] = (unsigned char)val;
        }
        else
        {
            DEBUG("NALU index: %lu\t size: %d\t SLICE\t frame index: %lu\n", nalu_index++, pos-next_nalu_startcode_len, frame_index++);
            *frame_size = pos;
            return 0;
        }

        if(p_frame_data[pos-3] == 0 && p_frame_data[pos-2] == 0 && p_frame_data[pos-1] == 0 && p_frame_data[pos] == 1)
        {
            next_nalu_startcode_len = 4;
            find_next_nalu = 1;
        }

        if(find_next_nalu)
        {
            if(is_idr_frame)
            {
                fread(&nalu_type, 1, 1, fp);
                fseek(fp, -1, SEEK_CUR);

                if((nalu_type & 0x1F) == H264_NALU_PSLICE)
                {
                    fseek(fp, -next_nalu_startcode_len, SEEK_CUR);

                    DEBUG("NALU index: %lu\t size: %d\t \033[31mIDR\033[0m\t frame index: %lu\n", nalu_index++, pos-nalu_start_pos+1-next_nalu_startcode_len, frame_index++);
                    *frame_size = pos+1-next_nalu_startcode_len;
                    *is_keyframe = 1;
                    return 0;
                }
                else
                {
                    find_next_nalu = 0;
                    if((nalu_type & 0x1F) == H264_NALU_SPS) // last nalu is SEI
                    {
                        DEBUG("NALU index: %lu\t size: %d\t \033[32mSEI\033[0m\n", nalu_index++, pos+1-next_nalu_startcode_len);
                        nalu_start_pos = pos+1-next_nalu_startcode_len;
                    }
                    else if((nalu_type & 0x1F) == H264_NALU_PPS) // last nalu is SPS
                    {
                        DEBUG("NALU index: %lu\t size: %d\t \033[35mSPS\033[0m\n", nalu_index++, pos+1-next_nalu_startcode_len);
                        nalu_start_pos = pos+1-next_nalu_startcode_len;
                    }
                    else if((nalu_type & 0x1F) == H264_NALU_ISLICE) // last nalu is PPS
                    {
                        DEBUG("NALU index: %lu\t size: %d\t \033[33mPPS\033[0m\n", nalu_index++, pos+1-next_nalu_startcode_len-nalu_start_pos);
                        nalu_start_pos = pos+1-next_nalu_startcode_len;
                    }
                }
            }
            else
            {
                fseek(fp, -next_nalu_startcode_len, SEEK_CUR);
                DEBUG("NALU index: %lu\t size: %d\t SLICE\t frame index: %lu\n", nalu_index++, pos+1-next_nalu_startcode_len, frame_index++);
                *frame_size = pos+1-next_nalu_startcode_len;
                *is_keyframe = 0;
                return 0;
            }
        }

        pos++;
    }

    return -1;
}


