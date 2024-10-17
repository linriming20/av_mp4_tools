#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include "mp4.h"



void print_usage(const char *process)
{
    printf("\033[33m(Note: Only support H.26x and AAC(LC) in this demo now.)\033[0m\n"
       "examples: \n"
       "    %s -h\n"
       "    %s --help\n"
       "    %s -a ./avfile/test1_44100_stereo.aac -v ./avfile/test1_856x480_24fps.h264 -W 856 -H 480 -f 24 -o ./test1_out.mp4\n"
       "    %s --audio_file=./avfile/test2_44100_mono.aac "
       "--video_file=./avfile/test2_1280x720_20fps.h265 --video_width=1280 --video_height=720 --video_fps=20 --output_mp4=./test2_out.mp4\n",
       process, process, process, process);
}


int main(int argc, char *argv[])
{
    FILE *fp_video = NULL;
    FILE *fp_audio = NULL;
    char audio_filename[128] = {0};
    char video_filename[128] = {0};
    char mp4_filename[128] = {0};
    unsigned int audio_samplerate = 0;
    unsigned int audio_channels = 0;
    unsigned int video_width = 0;
    unsigned int video_height = 0;
    unsigned int video_fps = 0;

    if(argc == 1)
    {
        print_usage(argv[0]);
        return -1;
    }

    char option = 0;
    int option_index = 0;
    const char *short_options = "ha:v:W:H:f:o:";
    struct option long_options[] =
    {
        {"help",            no_argument,       NULL, 'h'},
        {"audio_file",      required_argument, NULL, 'a'},
        {"video_file",      required_argument, NULL, 'v'},
        {"video_width",     required_argument, NULL, 'W'},
        {"video_height",    required_argument, NULL, 'H'},
        {"video_fps",       required_argument, NULL, 'f'},
        {"output_mp4",      required_argument, NULL, 'o'},
        {NULL,              0,                 NULL,  0 },
    };
    while((option = getopt_long_only(argc, argv, short_options, long_options, &option_index)) != -1)
    {
        switch(option)
        {
            case 'h':
                print_usage(argv[0]);
                return 0;
            case 'a':
                strncpy(audio_filename, optarg, 128);
                    break;
            case 'r':
                audio_samplerate = atoi(optarg);
                break;
            case 'c':
                audio_channels = atoi(optarg);
                break;
            case 'v':
                strncpy(video_filename, optarg, 128);
                break;
            case 'W':
                video_width = atoi(optarg);
                break;
            case 'H':
                video_height = atoi(optarg);
                break;
            case 'f':
                video_fps = atoi(optarg);
                break;
            case 'o':
                strncpy(mp4_filename, optarg, 128);
                break;
            defalut:
                printf("Unknown argument!\n");
                break;
        }
    }

    if(!video_width || !video_height || !video_fps ||\
       !strlen(audio_filename) || !strlen(video_filename) || !strlen(mp4_filename))
    {
        printf("Parameter not set!\n");
        print_usage(argv[0]);
        return -1;
    }
    else
    {
        printf("\n**************************************\n"
           "input: \n"
           "\t audio file name: %s\n"
           "\t video file name: %s\n"
           "\t  - width: %d\n"
           "\t  - height: %d\n"
           "\t  - fps: %d\n"
           "output: \n"
           "\t file name: %s\n"
           "**************************************\n\n",
          audio_filename, video_filename, video_width, video_height, video_fps, mp4_filename);
    }

    return mp4_mux_h26x_aac(video_filename, video_width, video_height, video_fps, audio_filename, mp4_filename);
}

