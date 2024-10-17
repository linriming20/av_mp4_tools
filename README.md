
##### 前言

本项目主要有以下功能：
 - 音视频封装（mp4_mux.c）；
 - MP4Box解析（mp4_tree.c）;
 - 音视频解封装（mp4_demux.c）;

###  1. 编译

```bash
make # 或者`make DEBUG=1`打开调试打印信息
```



### 2. 使用


```bash
 ./mp4_mux
(Note: Only support H.26x and AAC(LC) in this demo now.)
examples:
    ./mp4_mux -h
    ./mp4_mux --help
    ./mp4_mux -a ./avfile/test1_44100_stereo.aac -v ./avfile/test1_856x480_24fps.h264 -W 856 -H 480 -f 24 -o ./test1_out.mp4
    ./mp4_mux --audio_file=./avfile/test2_44100_mono.aac --video_file=./avfile/test2_1280x720_20fps.h265 --video_width=1280 --video_height=720 --video_fps=20 --output_mp4=./test2_out.mp4


./mp4_tree
examples:
    ./mp4_tree -h
    ./mp4_tree --help
    ./mp4_tree -f test1_out.mp4
    ./mp4_tree --mp4_filename=test2_out.mp4


./mp4_demux
examples:
    ./mp4_demux -h
    ./mp4_demux --help
    ./mp4_demux -f test1_out.mp4
    ./mp4_demux --mp4_filename=test2_out.mp4
```


### 3. 参考文章

 - [MP4格式详解_DONGHONGBAI的专栏-CSDN博客](https://blog.csdn.net/DONGHONGBAI/article/details/84401397)

 - [H264_H265的两种格式Annex B、AVCC(H264)_HVCC(H265)_265h,cc-CSDN博客](https://blog.csdn.net/weixin_43147845/article/details/137011573)

 - [MP4 esds box解析_mp4decspecificdescr-CSDN博客](https://blog.csdn.net/qiuchangyong/article/details/26586829)

### 附录（demo目录结构）

```
tree
.
├── Makefile
├── README.md
├── app
│   ├── mp4_demux.c
│   ├── mp4_mux.c
│   └── mp4_tree.c
├── avfile
│   ├── test1_44100_stereo.aac
│   ├── test1_856x480_24fps.h264
│   ├── test1_out.mp4
│   ├── test2_1280x720_20fps.h265
│   ├── test2_44100_mono.aac
│   └── test2_out.mp4
├── codecs
│   ├── aac
│   │   ├── aac_adts.c
│   │   └── aac_adts.h
│   ├── h264
│   │   ├── h264_AnnexB.c
│   │   └── h264_AnnexB.h
│   └── h265
│       ├── h265_AnnexB.c
│       └── h265_AnnexB.h
├── debug.h
├── docs
│   ├── H264_H265的两种格式Annex B、AVCC(H264)_HVCC(H265)_265h,cc-CSDN博客.mhtml
│   ├── MP4 esds box解析_mp4decspecificdescr-CSDN博客.mhtml
│   └── MP4格式详解_DONGHONGBAI的专栏-CSDN博客.mhtml
├── mp4.c
├── mp4.h
├── mp4_box
│   ├── free
│   │   ├── free.c
│   │   └── free.h
│   ├── ftyp
│   │   ├── ftyp.c
│   │   └── ftyp.h
│   ├── mdat
│   │   ├── mdat.c
│   │   └── mdat.h
│   ├── moov
│   │   ├── moov.c
│   │   ├── moov.h
│   │   ├── mvhd
│   │   │   ├── mvhd.c
│   │   │   └── mvhd.h
│   │   └── trak
│   │       ├── mdia
│   │       │   ├── hdlr
│   │       │   │   ├── hdlr.c
│   │       │   │   └── hdlr.h
│   │       │   ├── mdhd
│   │       │   │   ├── mdhd.c
│   │       │   │   └── mdhd.h
│   │       │   ├── mdia.c
│   │       │   ├── mdia.h
│   │       │   └── minf
│   │       │       ├── dinf
│   │       │       │   ├── dinf.c
│   │       │       │   ├── dinf.h
│   │       │       │   └── dref
│   │       │       │       ├── dref.c
│   │       │       │       ├── dref.h
│   │       │       │       └── url
│   │       │       │           ├── url.c
│   │       │       │           └── url.h
│   │       │       ├── minf.c
│   │       │       ├── minf.h
│   │       │       ├── smhd
│   │       │       │   ├── smhd.c
│   │       │       │   └── smhd.h
│   │       │       ├── stbl
│   │       │       │   ├── stbl.c
│   │       │       │   ├── stbl.h
│   │       │       │   ├── stco
│   │       │       │   │   ├── stco.c
│   │       │       │   │   └── stco.h
│   │       │       │   ├── stsc
│   │       │       │   │   ├── stsc.c
│   │       │       │   │   └── stsc.h
│   │       │       │   ├── stsd
│   │       │       │   │   ├── avc1
│   │       │       │   │   │   ├── avc1.c
│   │       │       │   │   │   ├── avc1.h
│   │       │       │   │   │   └── avcC
│   │       │       │   │   │       ├── avcC.c
│   │       │       │   │   │       └── avcC.h
│   │       │       │   │   ├── hvc1
│   │       │       │   │   │   ├── hvc1.c
│   │       │       │   │   │   ├── hvc1.h
│   │       │       │   │   │   └── hvcC
│   │       │       │   │   │       ├── hvcC.c
│   │       │       │   │   │       └── hvcC.h
│   │       │       │   │   ├── mp4a
│   │       │       │   │   │   ├── esds
│   │       │       │   │   │   │   ├── esds.c
│   │       │       │   │   │   │   └── esds.h
│   │       │       │   │   │   ├── mp4a.c
│   │       │       │   │   │   └── mp4a.h
│   │       │       │   │   ├── stsd.c
│   │       │       │   │   └── stsd.h
│   │       │       │   ├── stss
│   │       │       │   │   ├── stss.c
│   │       │       │   │   └── stss.h
│   │       │       │   ├── stsz
│   │       │       │   │   ├── stsz.c
│   │       │       │   │   └── stsz.h
│   │       │       │   └── stts
│   │       │       │       ├── stts.c
│   │       │       │       └── stts.h
│   │       │       └── vmhd
│   │       │           ├── vmhd.c
│   │       │           └── vmhd.h
│   │       ├── tkhd
│   │       │   ├── tkhd.c
│   │       │   └── tkhd.h
│   │       ├── trak.c
│   │       └── trak.h
│   ├── mp4_box.h
│   ├── mp4_box_header.c
│   ├── mp4_box_header.h
│   ├── mp4_metadata.c
│   └── mp4_metadata.h
├── reference_code
│   └── H264_AAC_MP4_MUX_本地文件_1.rar
└── tools
    └── mp4info.zip

39 directories, 88 files
```