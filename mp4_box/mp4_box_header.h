#ifndef __MP4_BOX_HEADER_H__
#define __MP4_BOX_HEADER_H__

#include <stdint.h>

#define HEADER_SIZE_BOX_SIZE        (4)
#define HEADER_SIZE_BOX_TYPE        (4)

#define HEADER_SIZE_BOX             (8)
#define HEADER_SIZE_LARGE_BOX       (16)
#define HEADER_SIZE_FULLBOX         (12)
#define HEADER_SIZE_LARGE_FULLBOX   (20)


/* Box */
int generate_box_header(uint32_t box_size, char box_type[4], uint8_t header_buf[8]);
int generate_large_box_header(uint64_t large_box_size, char box_type[4], uint8_t header_buf[16]);

/* FullBox */
int generate_fullbox_header(uint32_t box_size, char box_type[4], uint8_t version, uint32_t flags, uint8_t header_buf[12]);
int generate_large_fullbox_header(uint64_t large_box_size, char box_type[4], uint8_t version, uint32_t flags, uint8_t header_buf[20]);


#endif /* __MP4_BOX_HEADER_H__ */

