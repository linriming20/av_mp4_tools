#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include "mp4.h"



void print_usage(const char *process)
{
    printf(
       "examples: \n"
       "    %s -h\n"
       "    %s --help\n"
       "    %s -f test1_out.mp4\n"
       "    %s --mp4_filename=test2_out.mp4\n",
       process, process, process, process);
}


int main(int argc, char *argv[])
{
    char mp4_filename[128] = {0};

    if(argc == 1)
    {
        print_usage(argv[0]);
        return -1;
    }

    char option = 0;
    int option_index = 0;
    const char *short_options = "hf:";
    struct option long_options[] =
    {
        {"help",            no_argument,       NULL, 'h'},
        {"mp4_filename",    required_argument, NULL, 'f'},
        {NULL,              0,                 NULL,  0 },
    };
    while((option = getopt_long_only(argc, argv, short_options, long_options, &option_index)) != -1)
    {
        switch(option)
        {
            case 'h':
                print_usage(argv[0]);
                return 0;
            case 'f':
                strncpy(mp4_filename, optarg, 128);
                break;
            defalut:
                printf("Unknown argument!\n");
                break;
        }
    }

    if(!strlen(mp4_filename))
    {
        printf("Parameter not set!\n");
        print_usage(argv[0]);
        return -1;
    }

    return mp4_tree(mp4_filename, 1, NULL);
}

