#include <stdlib.h>
#include <string.h>

#include "mp4_metadata.h"

int alloc_metadata_memory(metadata_t *metadata)
{
    metadata->cur_record_max = MAX_SAMPLE_CNT;

    metadata->stss_sample_number = calloc(MAX_SAMPLE_CNT, sizeof(uint32_t));
    for(int track_id = 0; track_id < TRACK_ID_MAX; track_id++)
    {
        metadata->stsz_entry_size[track_id]    = calloc(MAX_SAMPLE_CNT, sizeof(uint32_t));
        metadata->stts_sample_count[track_id]  = calloc(MAX_SAMPLE_CNT, sizeof(uint32_t));
        metadata->stts_sample_delta[track_id]  = calloc(MAX_SAMPLE_CNT, sizeof(uint32_t));
        metadata->stsc_first_chunk[track_id]   = calloc(MAX_SAMPLE_CNT, sizeof(uint32_t));
        metadata->stsc_samples_per_chunk[track_id] = calloc(MAX_SAMPLE_CNT, sizeof(uint32_t));
        metadata->stsc_sample_description_index[track_id] = calloc(MAX_SAMPLE_CNT, sizeof(uint32_t));
        metadata->stco_chunk_offset[track_id] = calloc(MAX_SAMPLE_CNT, sizeof(uint32_t));
    }

    return 0;
}


int realloc_metadata_memory(metadata_t *metadata)
{
    int realloc_cnt = 0;
    metadata->cur_record_max += MAX_SAMPLE_CNT;
    realloc_cnt = metadata->cur_record_max;
    printf("Dynamically alloc memory to record more sample info. current can record max sample is %d\n", metadata->cur_record_max);

    metadata->stss_sample_number = realloc(metadata->stss_sample_number, realloc_cnt*sizeof(uint32_t));
    for(int track_id = 0; track_id < TRACK_ID_MAX; track_id++)
    {
        metadata->stsz_entry_size[track_id]    = realloc(metadata->stsz_entry_size[track_id], realloc_cnt*sizeof(uint32_t));
        metadata->stts_sample_count[track_id]  = realloc(metadata->stts_sample_count[track_id], realloc_cnt*sizeof(uint32_t));
        metadata->stts_sample_delta[track_id]  = realloc(metadata->stts_sample_delta[track_id], realloc_cnt*sizeof(uint32_t));
        metadata->stsc_first_chunk[track_id]   = realloc(metadata->stsc_first_chunk[track_id], realloc_cnt*sizeof(uint32_t));
        metadata->stsc_samples_per_chunk[track_id] = realloc(metadata->stsc_samples_per_chunk[track_id], realloc_cnt*sizeof(uint32_t));
        metadata->stsc_sample_description_index[track_id] = realloc(metadata->stsc_sample_description_index[track_id], realloc_cnt*sizeof(uint32_t));
        metadata->stco_chunk_offset[track_id] = realloc(metadata->stco_chunk_offset[track_id], realloc_cnt*sizeof(uint32_t));
    }

    return 0;
}


int free_metadata_memory(metadata_t *metadata)
{
    metadata->cur_record_max = 0;

    free(metadata->stss_sample_number);
    for(int track_id = 0; track_id < TRACK_ID_MAX; track_id++)
    {
        free(metadata->stsz_entry_size[track_id]);
        free(metadata->stts_sample_count[track_id]);
        free(metadata->stts_sample_delta[track_id]);
        free(metadata->stsc_first_chunk[track_id]);
        free(metadata->stsc_samples_per_chunk[track_id]);
        free(metadata->stsc_sample_description_index[track_id]);
        free(metadata->stco_chunk_offset[track_id]);
    }

    return 0;
}

