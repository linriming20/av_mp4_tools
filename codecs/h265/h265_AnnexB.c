#include "h265_AnnexB.h"

int get_one_H265_AnnexB_nalu(FILE *fp, uint8_t *p_nalu_data, h265_nalu_info_t *pt_nalu_info)
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
    pt_nalu_info->nalu_type = (p_nalu_data[pt_nalu_info->startcode_len] >> 1) & 0x3F;

    return 0;
}

