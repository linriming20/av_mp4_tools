#include "mp4_box_header.h"



int generate_box_header(uint32_t box_size, char box_type[4], uint8_t header_buf[8])
{
    /* header: size(4B) + type(4B) */
    uint32_t buf_pos = 0;

    /* box size */
    header_buf[buf_pos+0] = (box_size >> 24) & 0xFF;
    header_buf[buf_pos+1] = (box_size >> 16) & 0xFF;
    header_buf[buf_pos+2] = (box_size >> 8)  & 0xFF;
    header_buf[buf_pos+3] = (box_size >> 0)  & 0xFF;
    buf_pos += 4;

    /* box type */
    header_buf[buf_pos+0] = box_type[0];
    header_buf[buf_pos+1] = box_type[1];
    header_buf[buf_pos+2] = box_type[2];
    header_buf[buf_pos+3] = box_type[3];
    buf_pos += 4;

    return buf_pos; //HEADER_SIZE_BOX
}

int generate_large_box_header(uint64_t large_box_size, char box_type[4], uint8_t header_buf[16])
{
    /* header: size(4B) + type(4B) + large_size(8B, if size == 1) */
    uint32_t buf_pos = 0;

    /* box size */
    header_buf[buf_pos+0] = 0x00;
    header_buf[buf_pos+1] = 0x00;
    header_buf[buf_pos+2] = 0x00;
    header_buf[buf_pos+3] = 0x01;
    buf_pos += 4;

    /* box type */
    header_buf[buf_pos+0] = box_type[0];
    header_buf[buf_pos+1] = box_type[1];
    header_buf[buf_pos+2] = box_type[2];
    header_buf[buf_pos+3] = box_type[3];
    buf_pos += 4;

    /* large box size */
    header_buf[buf_pos+0] = (large_box_size >> 56) & 0xFF;
    header_buf[buf_pos+1] = (large_box_size >> 48) & 0xFF;
    header_buf[buf_pos+2] = (large_box_size >> 40) & 0xFF;
    header_buf[buf_pos+3] = (large_box_size >> 32) & 0xFF;
    header_buf[buf_pos+4] = (large_box_size >> 24) & 0xFF;
    header_buf[buf_pos+5] = (large_box_size >> 16) & 0xFF;
    header_buf[buf_pos+6] = (large_box_size >> 8)  & 0xFF;
    header_buf[buf_pos+7] = (large_box_size >> 0)  & 0xFF;
    buf_pos += 8;

    return buf_pos; //HEADER_SIZE_LARGE_BOX
}


int generate_fullbox_header(uint32_t box_size, char box_type[4], uint8_t version, uint32_t flags, uint8_t header_buf[12])
{
    /* header: size(4B) + type(4B) + version(1B) + flags(3B) */
    uint32_t buf_pos = 0;

    /* box size */
    header_buf[buf_pos+0] = (box_size >> 24) & 0xFF;
    header_buf[buf_pos+1] = (box_size >> 16) & 0xFF;
    header_buf[buf_pos+2] = (box_size >> 8)  & 0xFF;
    header_buf[buf_pos+3] = (box_size >> 0)  & 0xFF;
    buf_pos += 4;

    /* box type */
    header_buf[buf_pos+0] = box_type[0];
    header_buf[buf_pos+1] = box_type[1];
    header_buf[buf_pos+2] = box_type[2];
    header_buf[buf_pos+3] = box_type[3];
    buf_pos += 4;


    /* version */
    header_buf[buf_pos+0] = version;
    buf_pos += 1;

    /* flags */
    header_buf[buf_pos+0] = (flags >> 16) & 0xFF;
    header_buf[buf_pos+1] = (flags >> 8)  & 0xFF;
    header_buf[buf_pos+2] = (flags >> 0)  & 0xFF;
    buf_pos += 3;

    return buf_pos; //HEADER_SIZE_FULLBOX
}

int generate_large_fullbox_header(uint64_t large_box_size, char box_type[4], uint8_t version, uint32_t flags, uint8_t header_buf[20])
{
    /* header: size(4B) + type(4B)+ large_size(8B, if size == 1) + version(1B) + flags(3B) */
    uint32_t buf_pos = 0;

    /* box size */
    header_buf[buf_pos+0] = 0x00;
    header_buf[buf_pos+1] = 0x00;
    header_buf[buf_pos+2] = 0x00;
    header_buf[buf_pos+3] = 0x01;
    buf_pos += 4;

    /* box type */
    header_buf[buf_pos+0] = box_type[0];
    header_buf[buf_pos+1] = box_type[1];
    header_buf[buf_pos+2] = box_type[2];
    header_buf[buf_pos+3] = box_type[3];
    buf_pos += 4;

    /* large box size */
    header_buf[buf_pos+0] = (large_box_size >> 56) & 0xFF;
    header_buf[buf_pos+1] = (large_box_size >> 48) & 0xFF;
    header_buf[buf_pos+2] = (large_box_size >> 40) & 0xFF;
    header_buf[buf_pos+3] = (large_box_size >> 32) & 0xFF;
    header_buf[buf_pos+4] = (large_box_size >> 24) & 0xFF;
    header_buf[buf_pos+5] = (large_box_size >> 16) & 0xFF;
    header_buf[buf_pos+6] = (large_box_size >> 8)  & 0xFF;
    header_buf[buf_pos+7] = (large_box_size >> 0)  & 0xFF;
    buf_pos += 8;

    /* version */
    header_buf[buf_pos+0] = version;
    buf_pos += 1;

    /* flags */
    header_buf[buf_pos+0] = (flags >> 16) & 0xFF;
    header_buf[buf_pos+1] = (flags >> 8)  & 0xFF;
    header_buf[buf_pos+2] = (flags >> 0)  & 0xFF;
    buf_pos += 3;

    return buf_pos; //HEADER_SIZE_LARGE_FULLBOX
}



