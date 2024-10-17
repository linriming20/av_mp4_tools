#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>

#include "debug.h"

#include "mp4_box_header.h"
#include "mp4_box.h"
#include "mp4.h"

#include "h264_AnnexB.h"
#include "h265_AnnexB.h"
#include "aac_adts.h"


#define DESC(msg, x)    (x)
#define FUNC_ENABLE     (1)
#define FUNC_DISABLE    (0)



#if DESC("MP4 mux", FUNC_ENABLE)
int mp4_mux_h26x_aac(char *h26x_filename, uint32_t width, uint32_t height, uint32_t video_fps, char *aac_filename, char *mp4_filename)
{
    char *p_buf = calloc(1, MP4_BOX_BUF_MAX_SIZE);
    FILE *fp_h26x = fopen(h26x_filename, "rb");
    FILE *fp_aac  = fopen(aac_filename,  "rb");
    FILE *fp_mp4  = fopen(mp4_filename,  "wb");
    metadata_t metadata = {};
    adts_header_t adts_header_info;
    uint32_t audio_fps = 0;

    /* prepare some info from media file. */
    if(strstr(h26x_filename, "264"))
    {
        metadata.video_type = VIDEO_TYPE_H264;
        printf("[video info] type: H.264, %dx%d %dfps.\n", width, height, video_fps);
    }
    else if(strstr(h26x_filename, "265"))
    {
        metadata.video_type = VIDEO_TYPE_H265;
        printf("[video info] type: H.265, %dx%d %dfps.\n", width, height, video_fps);
    }
    metadata.video_width = width;
    metadata.video_height = height;
    metadata.video_fps = video_fps;

    #ifdef ENABLE_DEBUG
    time_t current_time = 0x7C25B080;
    #else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    time_t current_time = tv.tv_sec + 2082844800; // 208284480 is (((1970 - 1904) * 365) + 17) * 24 * 60 * 60
    #endif
    metadata.creation_time = current_time;
    metadata.modification_time = current_time;
    metadata.timescale = 1000; // 90000
    metadata.trackid[TRACK_ID_AUDIO] = TRACK_ID_AUDIO;
    metadata.trackid[TRACK_ID_VIDEO] = TRACK_ID_VIDEO;

    // get aac framerate
    int ret = get_aac_adts_frame(fp_aac, p_buf, &adts_header_info);
    if(!ret)
    {
        fseek(fp_aac, 0, SEEK_SET); // reset
        switch(adts_header_info.sampling_freq_index)
        {
            case SFI_96000: audio_fps = 1000.0/(1024*1000/96000); break;
            case SFI_88200: audio_fps = 1000.0/(1024*1000/88200); break;
            case SFI_64000: audio_fps = 1000.0/(1024*1000/64000); break;
            case SFI_48000: audio_fps = 1000.0/(1024*1000/48000); break;
            case SFI_44100: audio_fps = 1000.0/(1024*1000/44100); break;
            case SFI_32000: audio_fps = 1000.0/(1024*1000/32000); break;
            case SFI_24000: audio_fps = 1000.0/(1024*1000/24000); break;
            case SFI_22050: audio_fps = 1000.0/(1024*1000/22050); break;
            case SFI_16000: audio_fps = 1000.0/(1024*1000/16000); break;
            case SFI_12000: audio_fps = 1000.0/(1024*1000/12000); break;
            case SFI_11025: audio_fps = 1000.0/(1024*1000/11025); break;
            case SFI_8000:  audio_fps = 1000.0/(1024*1000/ 8000); break;
            case SFI_7350:  audio_fps = 1000.0/(1024*1000/ 7350); break;
            default:        audio_fps = -1; break;
        }
        printf("[audio info] channel count: %d, %dfps.\n", adts_header_info.channel_configuration, audio_fps);
        metadata.audio_channel_cnt = adts_header_info.channel_configuration;
        metadata.audio_samplerate_idx = adts_header_info.sampling_freq_index; // equal
        metadata.audio_fps = audio_fps;
    }


    /* start mux and write mp4 */
    write_ftyp_box(fp_mp4, p_buf);

    write_free_box(fp_mp4, p_buf);

    write_mdat_box(fp_mp4, fp_h26x, fp_aac, p_buf, &metadata);

    write_moov_box(fp_mp4, p_buf, &metadata);
    /* end mux and write mp4 */


    if(p_buf)
        free(p_buf);
    if(fp_h26x)
        fclose(fp_h26x);
    if(fp_aac)
        fclose(fp_aac);
    if(fp_mp4)
    {
        fflush(fp_mp4);
        fclose(fp_mp4);
    }

    return 0;
}
#endif



#if DESC("MP4 parse and demux", FUNC_ENABLE)

/* HEADER_SIZE_LARGE_BOX: read box size and box type, not read version and flag at fullbox */
#define READ_HEAD_MAX_SIZE  (16)

/* big endian storage change to little endian variable */
#define U8BUF_TO_UINT32_LE(u8)  (uint32_t)((u8[3]<<0)|(u8[2]<<8)|(u8[1]<<16)|(u8[0]<<24))
#define U8BUF_TO_UINT16_LE(u8)  (uint16_t)((u8[1]<<0)|(u8[0]<<8))


static struct {
    char *box_type;
    uint8_t has_inside_box;
}box_info[] = {
    {"ftyp", 0},
    {"free", 0},
    {"mdat", 0},
    {"moov", 1},
        {"mvhd", 0},
        {"trak", 1},
            {"tkhd", 0},
            {"mdia", 1},
                {"mdhd", 0},
                {"hdlr", 0},
                {"minf", 1},
                    {"vmhd", 0},
                    {"smhd", 0},
                    {"dinf", 1},
                        {"dref", 1},
                            {"url ", 0},
                    {"stbl", 1},
                        {"stsz", 0},
                        {"stsd", 1},
                            {"avc1", 1},
                                {"avcC", 0},
                            {"hvc1", 1},
                                {"hvcC", 0},
                            {"mp4a", 1},
                                {"esds", 0},
                        {"stts", 0},
                        {"stss", 0},
                        {"stsc", 0},
                        {"stco", 0},
};



static int mp4_get_box_info(FILE *fp, char box_type[HEADER_SIZE_BOX_TYPE], uint64_t *p_box_size)
{
    uint8_t buf[READ_HEAD_MAX_SIZE] = {0}; // not use int8_t
    uint64_t val[READ_HEAD_MAX_SIZE] = {0};
    uint32_t box_size;
    uint64_t large_box_size;
    uint64_t file_pos_bak = ftell(fp); // record file pos

    int ret = fread(&buf, 1, READ_HEAD_MAX_SIZE, fp);
    if(ret <= 0)
    {
        DEBUG("mp4 file end.\n");
        return -1;
    }
    for(int i = 0; i < READ_HEAD_MAX_SIZE; i++)
    {
        val[i] = buf[i];
    }

    box_size = (val[3] << 0) |(val[2] << 8)|\
               (val[1] << 16)|(val[0] << 24);
    memcpy(box_type, &buf[4], HEADER_SIZE_BOX_TYPE);
    large_box_size = (val[15] << 0) |(val[14] << 8)|\
                     (val[13] << 16)|(val[12] << 24)|\
                     (val[11] << 32)|(val[10] << 40)|\
                     (val[9]  << 48)|(val[8] << 56);

    if(box_size == 1)
        *p_box_size = large_box_size;
    else
        *p_box_size = box_size;

    fseek(fp, file_pos_bak, SEEK_SET); // reset file pos

    return 0;
}

static uint64_t mp4_get_box_size(uint8_t buf[READ_HEAD_MAX_SIZE])
{
    uint64_t val[READ_HEAD_MAX_SIZE] = {0};
    uint32_t box_size;
    uint64_t large_box_size;

    for(int i = 0; i < READ_HEAD_MAX_SIZE; i++)
    {
        val[i] = buf[i];
    }

    box_size = (val[3] << 0) |(val[2] << 8)|\
               (val[1] << 16)|(val[0] << 24);
    large_box_size = (val[15] << 0) |(val[14] << 8)|\
                     (val[13] << 16)|(val[12] << 24)|\
                     (val[11] << 32)|(val[10] << 40)|\
                     (val[9]  << 48)|(val[8] << 56);

    if(box_size == 1)
        return large_box_size;
    else
        return box_size;
}


/* p_buf pos should be after [box size] + [box type] */
static int mp4_parse_inside_box(char *p_buf, uint64_t buf_size, int enable_print, metadata_t *metadata)
{
    static uint8_t level = 0;
    uint64_t box_size;
    level++;

    for(int i = 0; i <= buf_size-HEADER_SIZE_BOX_TYPE; i++)
    {
        for(int j = 0; j < sizeof(box_info)/sizeof(box_info[0]); j++)
        {
            if(!memcmp(box_info[j].box_type, p_buf+i, HEADER_SIZE_BOX_TYPE))
            {
                box_size = mp4_get_box_size(p_buf+i-HEADER_SIZE_BOX_TYPE);

                /* print box info */
                for(int l = 0; l < level; l++)
                    if(enable_print) printf("   ");
                #ifdef ENABLE_DEBUG
                if(enable_print) printf("[%s](%lu)    buf_size:%lu,  box type start pos:%d,  find next box type pos:%lu\n",
                        box_info[j].box_type, box_size, buf_size, i, i+box_size-1);
                #else
                if(enable_print) printf("[%s](%lu)\n", box_info[j].box_type, box_size);
                #endif

                /* record something */
                #if 1
                if(metadata != NULL)
                {
                    static int cur_read_trackid = -1;
                    if(!memcmp("trak", p_buf+i, HEADER_SIZE_BOX_TYPE))
                    {
                        cur_read_trackid++;
                    }
                    else if(!memcmp("stsz", p_buf+i, HEADER_SIZE_BOX_TYPE))
                    {
                        uint8_t *p_stsz_sample_count = (p_buf+i-HEADER_SIZE_BOX_SIZE)/*box start*/ + HEADER_SIZE_FULLBOX + 4/*skip sample size*/;
                        metadata->stsz_sample_count[cur_read_trackid] = \
                            (p_stsz_sample_count[3] << 0) |(p_stsz_sample_count[2] << 8)|\
                            (p_stsz_sample_count[1] << 16)|(p_stsz_sample_count[0] << 24);
                        DEBUG("track id: %d, stsz_sample_count: %d\n", cur_read_trackid, metadata->stsz_sample_count[cur_read_trackid]);
                        metadata->stsz_entry_size[cur_read_trackid] = calloc(sizeof(uint32_t), metadata->stsz_sample_count[cur_read_trackid]);
                        for(int samples_id = 0; samples_id < metadata->stsz_sample_count[cur_read_trackid]; samples_id++)
                        {
                            uint8_t *p_stsz_entry_size = (p_stsz_sample_count+4)+samples_id*sizeof(uint32_t);
                            metadata->stsz_entry_size[cur_read_trackid][samples_id] = U8BUF_TO_UINT32_LE(p_stsz_entry_size);
                        }
                    }
                    else if(!memcmp("stco", p_buf+i, HEADER_SIZE_BOX_TYPE))
                    {
                        uint8_t *p_stco_entry_count = (p_buf+i-HEADER_SIZE_BOX_SIZE)/*box start*/ + HEADER_SIZE_FULLBOX;
                        metadata->stco_entry_count[cur_read_trackid] = \
                            (p_stco_entry_count[3] << 0) |(p_stco_entry_count[2] << 8)|\
                            (p_stco_entry_count[1] << 16)|(p_stco_entry_count[0] << 24);
                        DEBUG("track id: %d, stco_entry_count: %d\n", cur_read_trackid, metadata->stco_entry_count[cur_read_trackid]);
                        metadata->stco_chunk_offset[cur_read_trackid] = calloc(sizeof(uint32_t), metadata->stco_entry_count[cur_read_trackid]);
                        for(int samples_id = 0; samples_id < metadata->stco_entry_count[cur_read_trackid]; samples_id++)
                        {
                            uint8_t *p_stco_chunk_offset = (p_stco_entry_count+4)+samples_id*sizeof(uint32_t);
                            metadata->stco_chunk_offset[cur_read_trackid][samples_id] = U8BUF_TO_UINT32_LE(p_stco_chunk_offset);
                        }
                    }
                    else if(!memcmp("stss", p_buf+i, HEADER_SIZE_BOX_TYPE))
                    {
                        uint8_t *p_stss_entry_count = (p_buf+i-HEADER_SIZE_BOX_SIZE)/*box start*/ + HEADER_SIZE_FULLBOX;
                        metadata->stss_entry_count = \
                            (p_stss_entry_count[3] << 0) |(p_stss_entry_count[2] << 8)|\
                            (p_stss_entry_count[1] << 16)|(p_stss_entry_count[0] << 24);
                        DEBUG("stss_entry_count: %d\n", metadata->stss_entry_count);
                        metadata->stss_sample_number = calloc(sizeof(uint32_t), metadata->stss_entry_count);
                        for(int samples_id = 0; samples_id < metadata->stss_entry_count; samples_id++)
                        {
                            uint8_t *p_stss_sample_number = (p_stss_entry_count+4)+samples_id*sizeof(uint32_t);
                            metadata->stss_sample_number[samples_id] = U8BUF_TO_UINT32_LE(p_stss_sample_number);
                        }
                    }
                    else if(!memcmp("avc1", p_buf+i, HEADER_SIZE_BOX_TYPE) || !memcmp("hvc1", p_buf+i, HEADER_SIZE_BOX_TYPE))
                    {
                        uint8_t *p_video_width  = (p_buf+i-HEADER_SIZE_BOX_SIZE)/*box start*/ + HEADER_SIZE_BOX + 24;
                        uint8_t *p_video_height = (p_buf+i-HEADER_SIZE_BOX_SIZE)/*box start*/ + HEADER_SIZE_BOX + 26;
                        metadata->video_width  = U8BUF_TO_UINT16_LE(p_video_width);
                        metadata->video_height = U8BUF_TO_UINT16_LE(p_video_height);
                        metadata->trackid_visual = cur_read_trackid;
                    }
                    else if(!memcmp("avcC", p_buf+i, HEADER_SIZE_BOX_TYPE))
                    {
                        metadata->video_type = VIDEO_TYPE_H264;
                        /* sps */
                        uint8_t *p_sps_len  = (p_buf+i-HEADER_SIZE_BOX_SIZE)/*box start*/ + HEADER_SIZE_BOX + 6;
                        uint8_t *p_sps_buf  = (p_buf+i-HEADER_SIZE_BOX_SIZE)/*box start*/ + HEADER_SIZE_BOX + 6 + sizeof(uint16_t)/*sps_len*/;
                        metadata->sps_len = U8BUF_TO_UINT16_LE(p_sps_len);
                        memcpy(metadata->sps, p_sps_buf, metadata->sps_len);
                        /* pps */
                        uint8_t *p_pps_len  = (p_buf+i-HEADER_SIZE_BOX_SIZE)/*box start*/ + HEADER_SIZE_BOX + 6 + sizeof(uint16_t)/*sps_len*/ + metadata->sps_len + 1/*pps_num*/;
                        uint8_t *p_pps_buf  = (p_buf+i-HEADER_SIZE_BOX_SIZE)/*box start*/ + HEADER_SIZE_BOX + 6 + sizeof(uint16_t)/*sps_len*/ + metadata->sps_len + 1/*pps_num*/ + 2/*pps_len*/;
                        metadata->pps_len = U8BUF_TO_UINT16_LE(p_pps_len);
                        memcpy(metadata->pps, p_pps_buf, metadata->pps_len);
                    }
                    else if(!memcmp("hvcC", p_buf+i, HEADER_SIZE_BOX_TYPE))
                    {
                        metadata->video_type = VIDEO_TYPE_H265;
                        /* vps */
                        uint8_t *p_vps_len  = (p_buf+i-HEADER_SIZE_BOX_SIZE)/*box start*/ + HEADER_SIZE_BOX + 26;
                        uint8_t *p_vps_buf  = (p_buf+i-HEADER_SIZE_BOX_SIZE)/*box start*/ + HEADER_SIZE_BOX + 26 + 2/*vps_len*/;
                        metadata->vps_len = U8BUF_TO_UINT16_LE(p_vps_len);
                        memcpy(metadata->vps, p_vps_buf, metadata->vps_len);
                        /* sps */
                        uint8_t *p_sps_len  = (p_buf+i-HEADER_SIZE_BOX_SIZE)/*box start*/ + HEADER_SIZE_BOX + 26 + 2/*vps_len*/ + metadata->vps_len + 1/*sps_flag*/ + 2/*sps_num*/;
                        uint8_t *p_sps_buf  = (p_buf+i-HEADER_SIZE_BOX_SIZE)/*box start*/ + HEADER_SIZE_BOX + 26 + 2/*vps_len*/ + metadata->vps_len + 1/*sps_flag*/ + 2/*sps_num*/ + 2/*sps_len*/;
                        metadata->sps_len = U8BUF_TO_UINT16_LE(p_sps_len);
                        memcpy(metadata->sps, p_sps_buf, metadata->sps_len);
                        /* pps */
                        uint8_t *p_pps_len  = (p_buf+i-HEADER_SIZE_BOX_SIZE)/*box start*/ + HEADER_SIZE_BOX + 26 + 2/*vps_len*/ + metadata->vps_len + 1/*pps_flag*/ + 2/*pps_num*/ + 2/*sps_len*/ + metadata->sps_len + 1/*pps_flag*/ + 2/*pps_num*/;
                        uint8_t *p_pps_buf  = (p_buf+i-HEADER_SIZE_BOX_SIZE)/*box start*/ + HEADER_SIZE_BOX + 26 + 2/*vps_len*/ + metadata->vps_len + 1/*pps_flag*/ + 2/*pps_num*/ + 2/*sps_len*/ + metadata->sps_len + 1/*pps_flag*/ + 2/*pps_num*/ + 2;
                        metadata->pps_len = U8BUF_TO_UINT16_LE(p_pps_len);
                        memcpy(metadata->pps, p_pps_buf, metadata->pps_len);
                    }
                    else if(!memcmp("esds", p_buf+i, HEADER_SIZE_BOX_TYPE))
                    {
                        uint8_t *p_descr_data  = (p_buf+i-HEADER_SIZE_BOX_SIZE)/*box start*/ + HEADER_SIZE_FULLBOX + 22;
                        metadata->audio_samplerate_idx = ((p_descr_data[0]&0x07)<<1)|((p_descr_data[1]>>7)&0x01);
                        metadata->audio_channel_cnt = (p_descr_data[1]>>3)&0x0F;
                        metadata->trackid_auditory = cur_read_trackid;
                    }
                }
                #endif

                /* recursion find */
                if(box_info[j].has_inside_box)
                {
                    /* p_buf+i is index to [box type] */
                    mp4_parse_inside_box(p_buf+i+HEADER_SIZE_BOX_TYPE, box_size-HEADER_SIZE_BOX_SIZE-HEADER_SIZE_BOX_TYPE, enable_print, metadata);
                }

                i += (box_size-1); /* for loop will add 1, so here sub 1 */
                break;
            }
        }
    }
    level--;

    return 0;
}


/* mp4_filename(in): must not be NULL;
 * enable_print(in): print the mp4 box as tree;
 * metadata(out): it will be set params if is not NULL
 */
int mp4_tree(char *mp4_filename, int enable_print, metadata_t *metadata)
{
    char *p_buf = calloc(1, MP4_BOX_BUF_MAX_SIZE);
    uint64_t box_size = 0;
    char box_type[HEADER_SIZE_BOX_TYPE+1/*'\0'*/] = {};
    int ret = 0;
    FILE *fp_mp4  = fopen(mp4_filename,  "rb");
    if(fp_mp4 == NULL)
    {
        perror("open mp4 file failed");
        return -1;
    }

    while(1)
    {
        /* top level box */
        ret = mp4_get_box_info(fp_mp4, box_type, &box_size);
        if(ret < 0)
        {
            break;
        }

        int found_known_box = 0;
        for(int i = 0; i < sizeof(box_info)/sizeof(box_info[0]); i++)
        {
            if(!strcmp(box_info[i].box_type, box_type))
            {
                found_known_box = 1;
                if(enable_print) printf("[%s](%lu)\n", box_type, box_size);

                /* inside box */
                if(box_info[i].has_inside_box)
                {
                    char *p_buf = calloc(1, box_size);
                    fread(p_buf, 1, box_size, fp_mp4);
                    mp4_parse_inside_box(p_buf+HEADER_SIZE_BOX_SIZE+HEADER_SIZE_BOX_TYPE, box_size-HEADER_SIZE_BOX_SIZE-HEADER_SIZE_BOX_TYPE, enable_print, metadata);
                    free(p_buf);
                }
                else
                {
                    fseek(fp_mp4, box_size, SEEK_CUR); // do not parse, just skip
                }
                break;

            }
        }

        if(found_known_box == 0)
        {
            if(enable_print) printf("the box(%s) is unknown in process now, just skip.\n", box_type);
            fseek(fp_mp4, box_size, SEEK_CUR); // do not parse, just skip
        }

    }

    if(p_buf)
        free(p_buf);
    if(fp_mp4)
        fclose(fp_mp4);
    return 0;
}


int mp4_demux(char *mp4_filename)
{
    metadata_t metadata = {};
    FILE *fp_mp4    = fopen(mp4_filename,    "rb");
    #define MP4_FRAME_BUF_MAX_SIZE     (512*1024)
    char *p_buf = calloc(1, MP4_FRAME_BUF_MAX_SIZE);
    uint32_t file_offset = 0;
    uint32_t frame_size = 0;
    char video_filename[256] = {0};
    char audio_filename[256] = {0};

    if(mp4_tree(mp4_filename, 0, &metadata))
    {
        return -1;
    }

    /* output file */
    char *video_type = NULL;
    if(metadata.video_type == VIDEO_TYPE_H264)video_type="h264";
    if(metadata.video_type == VIDEO_TYPE_H265)video_type="h265";
    snprintf(video_filename, sizeof(video_filename), "out_video_%dx%d.%s", metadata.video_width, metadata.video_height, video_type);
    snprintf(audio_filename, sizeof(video_filename), "out_audio_%d_%dch.aac", metadata.audio_samplerate_idx, metadata.audio_channel_cnt);
    FILE *fp_video  = fopen(video_filename,  "wb");
    FILE *fp_audio  = fopen(audio_filename,  "wb");


    printf("[video info] resolution: %dx%d, vps_len: %d sps_len: %d, pps_len: %d\n",
        metadata.video_width, metadata.video_height, metadata.vps_len, metadata.sps_len, metadata.pps_len);
    printf("[audio info] sample rate index: %d, channels: %d\n",
        metadata.audio_samplerate_idx, metadata.audio_channel_cnt);

    for(int track_id = 0; track_id < TRACK_ID_MAX; track_id++)
    {
        DEBUG("track id: %d, frame count: %d\n", track_id, metadata.stco_entry_count[track_id]);
        for(int frame_index = 0; frame_index < metadata.stco_entry_count[track_id]; frame_index++)
        {
            frame_size = metadata.stsz_entry_size[track_id][frame_index];
            file_offset = metadata.stco_chunk_offset[track_id][frame_index];
            DEBUG("track id: %d, frame index: %d, file offset: %d, frame size: %d\n", track_id, frame_index, file_offset, frame_size);

            /* read from MP4 */
            fseek(fp_mp4, file_offset, SEEK_SET);
            fread(p_buf, 1, frame_size, fp_mp4);

            /* write to output video file or audio file */
            if(track_id == metadata.trackid_visual)
            {
                char start_code[4] = {0x00, 0x00, 0x00, 0x01};
                for(int i = 0; i < metadata.stss_entry_count; i++)
                {
                    if(frame_index == metadata.stss_sample_number[i] - 1) // sync sample(frame) // fix: stss start with 1, so should -1
                    {
                        if(metadata.video_type == VIDEO_TYPE_H265)
                        {
                            fwrite(start_code, 1, sizeof(start_code), fp_video);
                            fwrite(metadata.vps, 1, metadata.vps_len, fp_video);
                        }
                        fwrite(start_code, 1, sizeof(start_code), fp_video);
                        fwrite(metadata.sps, 1, metadata.sps_len, fp_video);
                        fwrite(start_code, 1, sizeof(start_code), fp_video);
                        fwrite(metadata.pps, 1, metadata.pps_len, fp_video);
                    }
                }
                p_buf[0] = 0x00;
                p_buf[1] = 0x00;
                p_buf[2] = 0x00;
                p_buf[3] = 0x01;
                fwrite(p_buf, 1, frame_size, fp_video);
            }
            else if(track_id == metadata.trackid_auditory)
            {
                uint8_t adts_header_buf[AAC_ADTS_HEADER_SIZE] = {0};
                uint32_t header_buf_en = 0;
                generate_aac_adts_header_v2(frame_size+AAC_ADTS_HEADER_SIZE, metadata.audio_channel_cnt, metadata.audio_samplerate_idx, adts_header_buf, &header_buf_en);
                fwrite(adts_header_buf, 1, header_buf_en, fp_audio);
                fwrite(p_buf, 1, frame_size, fp_audio);
            }
            else
            {
                fprintf(stderr, "unknown track id(%d).\n", track_id);
            }
        }
    }


    if(p_buf)
        free(p_buf);
    if(fp_mp4)
        fclose(fp_mp4);
    if(fp_video)
    {
        fflush(fp_video);
        fclose(fp_video);
    }
    if(fp_audio)
    {
        fflush(fp_audio);
        fclose(fp_audio);
    }
    return 0;
}

#endif

