TRAGET := mp4_mux mp4_tree mp4_demux

CC := gcc
CFLAGS := \
    -I. \
    -Iapp \
    -Icodecs/aac \
    -Icodecs/h264 \
    -Icodecs/h265 \
    -Imp4_box \
    -Imp4_box/ftyp \
    -Imp4_box/mdat \
    -Imp4_box/moov \
    -Imp4_box/moov/trak/tkhd \
    -Imp4_box/moov/trak/mdia/mdhd \
    -Imp4_box/moov/trak/mdia \
    -Imp4_box/moov/trak/mdia/minf/dinf \
    -Imp4_box/moov/trak/mdia/minf/dinf/dref \
    -Imp4_box/moov/trak/mdia/minf/dinf/dref/url \
    -Imp4_box/moov/trak/mdia/minf/smhd \
    -Imp4_box/moov/trak/mdia/minf/vmhd \
    -Imp4_box/moov/trak/mdia/minf \
    -Imp4_box/moov/trak/mdia/minf/stbl \
    -Imp4_box/moov/trak/mdia/minf/stbl/stss \
    -Imp4_box/moov/trak/mdia/minf/stbl/stsd \
    -Imp4_box/moov/trak/mdia/minf/stbl/stsd/avc1 \
    -Imp4_box/moov/trak/mdia/minf/stbl/stsd/avc1/avcC \
    -Imp4_box/moov/trak/mdia/minf/stbl/stsd/hvc1 \
    -Imp4_box/moov/trak/mdia/minf/stbl/stsd/hvc1/hvcC \
    -Imp4_box/moov/trak/mdia/minf/stbl/stsd/mp4a \
    -Imp4_box/moov/trak/mdia/minf/stbl/stsd/mp4a/esds \
    -Imp4_box/moov/trak/mdia/minf/stbl/stsz \
    -Imp4_box/moov/trak/mdia/minf/stbl/stco \
    -Imp4_box/moov/trak/mdia/minf/stbl/co64 \
    -Imp4_box/moov/trak/mdia/minf/stbl/stts \
    -Imp4_box/moov/trak/mdia/minf/stbl/stsc \
    -Imp4_box/moov/trak/mdia/hdlr \
    -Imp4_box/moov/trak \
    -Imp4_box/moov/mvhd \
    -Imp4_box/free

COMMON_SRC := \
    mp4.c \
    codecs/aac/aac_adts.c \
    codecs/h264/h264_AnnexB.c \
    codecs/h265/h265_AnnexB.c \
    mp4_box/mp4_box_header.c \
    mp4_box/mp4_metadata.c \
    mp4_box/ftyp/ftyp.c \
    mp4_box/mdat/mdat.c \
    mp4_box/moov/moov.c \
    mp4_box/moov/trak/tkhd/tkhd.c \
    mp4_box/moov/trak/mdia/mdhd/mdhd.c \
    mp4_box/moov/trak/mdia/mdia.c \
    mp4_box/moov/trak/mdia/minf/dinf/dinf.c \
    mp4_box/moov/trak/mdia/minf/dinf/dref/dref.c \
    mp4_box/moov/trak/mdia/minf/dinf/dref/url/url.c \
    mp4_box/moov/trak/mdia/minf/smhd/smhd.c \
    mp4_box/moov/trak/mdia/minf/vmhd/vmhd.c \
    mp4_box/moov/trak/mdia/minf/minf.c \
    mp4_box/moov/trak/mdia/minf/stbl/stss/stss.c \
    mp4_box/moov/trak/mdia/minf/stbl/stsd/stsd.c \
    mp4_box/moov/trak/mdia/minf/stbl/stsd/avc1/avc1.c \
    mp4_box/moov/trak/mdia/minf/stbl/stsd/avc1/avcC/avcC.c \
    mp4_box/moov/trak/mdia/minf/stbl/stsd/hvc1/hvc1.c \
    mp4_box/moov/trak/mdia/minf/stbl/stsd/hvc1/hvcC/hvcC.c \
    mp4_box/moov/trak/mdia/minf/stbl/stsd/mp4a/mp4a.c \
    mp4_box/moov/trak/mdia/minf/stbl/stsd/mp4a/esds/esds.c \
    mp4_box/moov/trak/mdia/minf/stbl/stbl.c \
    mp4_box/moov/trak/mdia/minf/stbl/stsz/stsz.c \
    mp4_box/moov/trak/mdia/minf/stbl/stco/stco.c \
    mp4_box/moov/trak/mdia/minf/stbl/co64/co64.c \
    mp4_box/moov/trak/mdia/minf/stbl/stts/stts.c \
    mp4_box/moov/trak/mdia/minf/stbl/stsc/stsc.c \
    mp4_box/moov/trak/mdia/hdlr/hdlr.c \
    mp4_box/moov/trak/trak.c \
    mp4_box/moov/mvhd/mvhd.c \
    mp4_box/free/free.c



# debug control
ifeq ($(DEBUG), 1)
CFLAGS += -DENABLE_DEBUG
endif

all : $(TRAGET)

mp4_mux : $(COMMON_SRC) app/mp4_mux.c
	$(CC) $^ $(CFLAGS) -o $@

mp4_tree : $(COMMON_SRC) app/mp4_tree.c
	$(CC) $^ $(CFLAGS) -o $@

mp4_demux : $(COMMON_SRC) app/mp4_demux.c
	$(CC) $^ $(CFLAGS) -o $@

clean : 
	rm -rf $(TRAGET) *out*
.PHONY := clean

