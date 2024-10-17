#ifndef __MP4_METADATA_H__
#define __MP4_METADATA_H__


#include <stdio.h>
#include <stdint.h>

#define MAX_SAMPLE_CNT      (2048) // If it samples count exceeds the size, it can dynamically increase.

typedef enum {
    VIDEO_TYPE_UNKNOWN = -1,
    VIDEO_TYPE_H264 = 0,
    VIDEO_TYPE_H265 = 1,
}video_type_e;

typedef enum {
    TRACK_ID_AUDIO = 0,
    TRACK_ID_VIDEO = 1,
    TRACK_ID_MAX   = 2,
}track_id_e;


typedef struct{
    /* MP4 info */
    uint32_t timescale;
    uint64_t duration_mp4;
    uint64_t duration[TRACK_ID_MAX];
    uint32_t trackid[TRACK_ID_MAX];
    uint64_t creation_time;
    uint64_t modification_time;
    /* for read */
    uint32_t trackid_visual;
    uint32_t trackid_auditory;

    /* video info */
    uint32_t video_type; /* @video_type_e */
    uint32_t video_width;
    uint32_t video_height;
    uint32_t video_fps;
    /* h264/h265 */
    uint8_t  vps[128];
    uint32_t vps_len;
    uint8_t  sps[128];
    uint32_t sps_len;
    uint8_t  pps[128];
    uint32_t pps_len;

    /* audio info */
    uint32_t audio_channel_cnt;
    uint32_t audio_samplerate_idx;
    uint32_t audio_fps;

    /* current support record max sample count */
    uint32_t cur_record_max;

    /* stsz info */
    uint32_t  stsz_sample_count[TRACK_ID_MAX];
    uint32_t* stsz_entry_size[TRACK_ID_MAX];

    /* stts info */
    uint32_t  stts_entry_count[TRACK_ID_MAX];
    uint32_t* stts_sample_count[TRACK_ID_MAX];
    uint32_t* stts_sample_delta[TRACK_ID_MAX];

    /* stss info */
    uint32_t  stss_entry_count;
    uint32_t* stss_sample_number;

    /* stsc info */
    uint32_t  stsc_entry_count[TRACK_ID_MAX];
    uint32_t* stsc_first_chunk[TRACK_ID_MAX];
    uint32_t* stsc_samples_per_chunk[TRACK_ID_MAX];
    uint32_t* stsc_sample_description_index[TRACK_ID_MAX];

    /* stco info */
    uint32_t  stco_entry_count[TRACK_ID_MAX];
    uint32_t* stco_chunk_offset[TRACK_ID_MAX];
}metadata_t, *pt_metadata;


int alloc_metadata_memory(metadata_t *metadata);
int realloc_metadata_memory(metadata_t *metadata);
int free_metadata_memory(metadata_t *metadata);


#endif /* __MP4_METADATA_H__ */

