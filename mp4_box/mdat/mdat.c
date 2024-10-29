#include <stdlib.h>
#include <string.h>

#include "mp4_box_header.h"

#include "mdat.h"
#include "debug.h"

#include "h264_AnnexB.h"
#include "h265_AnnexB.h"
#include "aac_adts.h"


int write_mdat_box(FILE *fp_mp4, FILE *fp_h264, FILE *fp_aac, char *p_buf, metadata_t *metadata)
{
    uint32_t box_size = 0;
    uint64_t timestamp_ms = 0;
    uint64_t timestamp_ms_video = 0;
    uint64_t timestamp_ms_audio = 0;
    int ret = 0;
    h264_nalu_info_t h264_nalu_info;
    h265_nalu_info_t h265_nalu_info;
    adts_header_t adts_header_info;
    uint64_t file_tail_pos = 0;

    alloc_metadata_memory(metadata);

    /* 0~7 is [box header], generate last. */
    uint64_t box_header_pos = ftell(fp_mp4);
    box_size += HEADER_SIZE_BOX;
    fseek(fp_mp4, HEADER_SIZE_BOX, SEEK_CUR);

    /* 8~ is [box data] */
    while(1)
    {
        if (timestamp_ms % (1000/metadata->video_fps) == 0)
        {
            if(metadata->video_type == VIDEO_TYPE_H264)
            {
                do
                {
                    ret = get_one_H264_AnnexB_nalu(fp_h264, p_buf, &h264_nalu_info);
                    if (ret < 0)
                    {
                        if(ret == -2)
                            DEBUG("video file end!\n");
                        else
                            printf(RED"get_one_H264_AnnexB_nalu error!\n"COLOR_END);
                        goto exit;
                    }
                    DEBUG(GREEN"[video] get one H.264 NALU(0x%02X) with length: %d\n"COLOR_END, p_buf[h264_nalu_info.startcode_len], h264_nalu_info.data_len);

                    if (h264_nalu_info.nalu_type == H264_NALU_SPS && metadata->sps_len == 0)
                    {
                        DEBUG("NALU_TYPE_SPS | nalu_info.data_len: %d\n", h264_nalu_info.data_len);
                        memcpy(metadata->sps, p_buf+h264_nalu_info.startcode_len, h264_nalu_info.data_len-h264_nalu_info.startcode_len);
                        metadata->sps_len = h264_nalu_info.data_len-h264_nalu_info.startcode_len;
                    }
                    else if (h264_nalu_info.nalu_type == H264_NALU_PPS && metadata->pps_len == 0)
                    {
                        DEBUG("NALU_TYPE_PPS | nalu_info.data_len: %d\n", h264_nalu_info.data_len);
                        memcpy(metadata->pps, p_buf+h264_nalu_info.startcode_len, h264_nalu_info.data_len-h264_nalu_info.startcode_len);
                        metadata->pps_len = h264_nalu_info.data_len-h264_nalu_info.startcode_len;
                    }

                }while((h264_nalu_info.nalu_type != H264_NALU_ISLICE) &&\
                                 (h264_nalu_info.nalu_type != H264_NALU_PSLICE));

                p_buf[0] = ((h264_nalu_info.data_len-h264_nalu_info.startcode_len) >> 24) & 0xFF;
                p_buf[1] = ((h264_nalu_info.data_len-h264_nalu_info.startcode_len) >> 16) & 0xFF;
                p_buf[2] = ((h264_nalu_info.data_len-h264_nalu_info.startcode_len) >> 8)  & 0xFF;
                p_buf[3] = ((h264_nalu_info.data_len-h264_nalu_info.startcode_len) >> 0)  & 0xFF;
                fwrite(p_buf, 1, h264_nalu_info.data_len, fp_mp4);

                metadata->stsz_entry_size[TRACK_ID_VIDEO][metadata->stsz_sample_count[TRACK_ID_VIDEO]] = h264_nalu_info.data_len;
                metadata->stsz_sample_count[TRACK_ID_VIDEO]++;

                if (h264_nalu_info.nalu_type == H264_NALU_ISLICE)
                {
                    metadata->stss_sample_number[metadata->stss_entry_count] = metadata->stsz_sample_count[TRACK_ID_VIDEO]; // frame index
                    metadata->stss_entry_count++;
                }

                metadata->stsc_first_chunk[TRACK_ID_VIDEO][metadata->stsc_entry_count[TRACK_ID_VIDEO]] = metadata->stsc_entry_count[TRACK_ID_VIDEO] + 1;
                metadata->stsc_samples_per_chunk[TRACK_ID_VIDEO][metadata->stsc_entry_count[TRACK_ID_VIDEO]] = 0x01;
                metadata->stsc_sample_description_index[TRACK_ID_VIDEO][metadata->stsc_entry_count[TRACK_ID_VIDEO]] = 0x01;
                metadata->stsc_entry_count[TRACK_ID_VIDEO]++;

                metadata->stco_chunk_offset[TRACK_ID_VIDEO][metadata->stco_entry_count[TRACK_ID_VIDEO]] = box_size + box_header_pos;
                metadata->stco_entry_count[TRACK_ID_VIDEO]++;

                if(metadata->stco_entry_count[TRACK_ID_VIDEO] >= metadata->cur_record_max - 1)
                    realloc_metadata_memory(metadata);

                timestamp_ms_video = timestamp_ms;
                box_size += h264_nalu_info.data_len;
            }
            else if(metadata->video_type == VIDEO_TYPE_H265)
            {
                do
                {
                    ret = get_one_H265_AnnexB_nalu(fp_h264, p_buf, &h265_nalu_info);
                    if (ret < 0)
                    {
                        if(ret == -2)
                            DEBUG("video file end!\n");
                        else
                            printf(RED"get_one_H265_AnnexB_nalu error!\n"COLOR_END);
                        goto exit;
                    }
                    DEBUG(GREEN"[video] get one H.265 NALU(0x%02X) with length: %d\n"COLOR_END, p_buf[h265_nalu_info.startcode_len], h265_nalu_info.data_len);

                    if (h265_nalu_info.nalu_type == H265_NALU_VPS && metadata->vps_len == 0)
                    {
                        DEBUG("NALU_TYPE_SPS | nalu_info.data_len: %d\n", h265_nalu_info.data_len);
                        memcpy(metadata->vps, p_buf+h265_nalu_info.startcode_len, h265_nalu_info.data_len-h265_nalu_info.startcode_len);
                        metadata->vps_len = h265_nalu_info.data_len-h265_nalu_info.startcode_len;
                    }
                    else if (h265_nalu_info.nalu_type == H265_NALU_SPS && metadata->sps_len == 0)
                    {
                        DEBUG("NALU_TYPE_SPS | nalu_info.data_len: %d\n", h265_nalu_info.data_len);
                        memcpy(metadata->sps, p_buf+h265_nalu_info.startcode_len, h265_nalu_info.data_len-h265_nalu_info.startcode_len);
                        metadata->sps_len = h265_nalu_info.data_len-h265_nalu_info.startcode_len;
                    }
                    else if (h265_nalu_info.nalu_type == H265_NALU_PPS && metadata->pps_len == 0)
                    {
                        DEBUG("NALU_TYPE_PPS | nalu_info.data_len: %d\n", h265_nalu_info.data_len);
                        memcpy(metadata->pps, p_buf+h265_nalu_info.startcode_len, h265_nalu_info.data_len-h265_nalu_info.startcode_len);
                        metadata->pps_len = h265_nalu_info.data_len-h265_nalu_info.startcode_len;
                    }

                }while((h265_nalu_info.nalu_type != H265_NALU_ISLICE) &&\
                                  (h265_nalu_info.nalu_type != H265_NALU_PSLICE));

                p_buf[0] = ((h265_nalu_info.data_len-h265_nalu_info.startcode_len) >> 24) & 0xFF;
                p_buf[1] = ((h265_nalu_info.data_len-h265_nalu_info.startcode_len) >> 16) & 0xFF;
                p_buf[2] = ((h265_nalu_info.data_len-h265_nalu_info.startcode_len) >> 8)  & 0xFF;
                p_buf[3] = ((h265_nalu_info.data_len-h265_nalu_info.startcode_len) >> 0)  & 0xFF;
                fwrite(p_buf, 1, h265_nalu_info.data_len, fp_mp4);

                metadata->stsz_entry_size[TRACK_ID_VIDEO][metadata->stsz_sample_count[TRACK_ID_VIDEO]] = h265_nalu_info.data_len;
                metadata->stsz_sample_count[TRACK_ID_VIDEO]++;

                if (h265_nalu_info.nalu_type == H265_NALU_ISLICE)
                {
                    metadata->stss_sample_number[metadata->stss_entry_count] = metadata->stsz_sample_count[TRACK_ID_VIDEO]; // frame index
                    metadata->stss_entry_count++;
                }

                metadata->stsc_first_chunk[TRACK_ID_VIDEO][metadata->stsc_entry_count[TRACK_ID_VIDEO]] = metadata->stsc_entry_count[TRACK_ID_VIDEO] + 1;
                metadata->stsc_samples_per_chunk[TRACK_ID_VIDEO][metadata->stsc_entry_count[TRACK_ID_VIDEO]] = 0x01;
                metadata->stsc_sample_description_index[TRACK_ID_VIDEO][metadata->stsc_entry_count[TRACK_ID_VIDEO]] = 0x01;
                metadata->stsc_entry_count[TRACK_ID_VIDEO]++;

                metadata->stco_chunk_offset[TRACK_ID_VIDEO][metadata->stco_entry_count[TRACK_ID_VIDEO]] = box_size + box_header_pos;
                metadata->stco_entry_count[TRACK_ID_VIDEO]++;

                if(metadata->stco_entry_count[TRACK_ID_VIDEO] >= metadata->cur_record_max - 1)
                    realloc_metadata_memory(metadata);

                timestamp_ms_video = timestamp_ms;
                box_size += h265_nalu_info.data_len;
            }
        }

        if (timestamp_ms % (1000/metadata->audio_fps) == 0)
        {
            ret = get_aac_adts_frame(fp_aac, p_buf, &adts_header_info);
            if (ret < 0)
            {
                if(ret == -2)
                    DEBUG("audio file end!\n");
                else
                    printf(RED"getAdtsFrame error!\n"COLOR_END);
                goto exit;
            }
            fwrite(p_buf + AAC_ADTS_HEADER_SIZE, 1, adts_header_info.aac_frame_length - AAC_ADTS_HEADER_SIZE, fp_mp4);
            DEBUG(RED"[audio] get one AAC-ADTS frame with length: %d\n"COLOR_END, adts_header_info.aac_frame_length);

            metadata->stsz_entry_size[TRACK_ID_AUDIO][metadata->stsz_sample_count[TRACK_ID_AUDIO]] = adts_header_info.aac_frame_length - AAC_ADTS_HEADER_SIZE;
            metadata->stsz_sample_count[TRACK_ID_AUDIO]++;

            metadata->stsc_first_chunk[TRACK_ID_AUDIO][metadata->stsc_entry_count[TRACK_ID_AUDIO]] = metadata->stsc_entry_count[TRACK_ID_AUDIO] + 1;
            metadata->stsc_samples_per_chunk[TRACK_ID_AUDIO][metadata->stsc_entry_count[TRACK_ID_AUDIO]] = 0x01;
            metadata->stsc_sample_description_index[TRACK_ID_AUDIO][metadata->stsc_entry_count[TRACK_ID_AUDIO]] = 0x01;
            metadata->stsc_entry_count[TRACK_ID_AUDIO]++;

            metadata->stco_chunk_offset[TRACK_ID_AUDIO][metadata->stco_entry_count[TRACK_ID_AUDIO]] = box_size + box_header_pos;
            metadata->stco_entry_count[TRACK_ID_AUDIO]++;

            if(metadata->stco_entry_count[TRACK_ID_AUDIO] >= metadata->cur_record_max - 1)
                realloc_metadata_memory(metadata);

            timestamp_ms_audio = timestamp_ms;
            box_size += (adts_header_info.aac_frame_length - AAC_ADTS_HEADER_SIZE);
        }
        timestamp_ms++;
    }

exit:

    /* record file tail */
    file_tail_pos = ftell(fp_mp4);

    /* write box header */
    fseek(fp_mp4, box_header_pos, SEEK_SET);
    generate_box_header(box_size, "mdat", p_buf);
    fwrite(p_buf, 1, HEADER_SIZE_BOX, fp_mp4);

    /* move to file tail */
    fseek(fp_mp4, file_tail_pos, SEEK_SET);

    /* MP4 info */
    metadata->duration_mp4 = timestamp_ms_audio > timestamp_ms_video ? timestamp_ms_audio : timestamp_ms_video;
    metadata->duration[TRACK_ID_AUDIO] = timestamp_ms_audio;
    metadata->duration[TRACK_ID_VIDEO] = timestamp_ms_video;
    printf("[duration] total: %lums, video: %lums, audio: %lums\n", metadata->duration_mp4, metadata->duration[TRACK_ID_VIDEO], metadata->duration[TRACK_ID_AUDIO]);

    metadata->stts_entry_count[TRACK_ID_VIDEO] = 0x01;
    metadata->stts_sample_count[TRACK_ID_VIDEO][0] = metadata->stsz_sample_count[TRACK_ID_VIDEO];
    metadata->stts_sample_delta[TRACK_ID_VIDEO][0] = timestamp_ms_video/metadata->stsz_sample_count[TRACK_ID_VIDEO];
    metadata->stts_entry_count[TRACK_ID_AUDIO] = 0x01;
    metadata->stts_sample_count[TRACK_ID_AUDIO][0] = metadata->stsz_sample_count[TRACK_ID_AUDIO];
    metadata->stts_sample_delta[TRACK_ID_AUDIO][0] = timestamp_ms_audio/metadata->stsz_sample_count[TRACK_ID_AUDIO];


    return box_size;
}

