#define LOG_TAG "libshim"
#include <libexif/exif-entry.h>


extern "C" {

/* libaudio_route */
extern int pop_seq_init(){return 0;};
extern int pop_seq_set(){return 0;};
extern int force_flush_set() { return 0;};

/* libexif */
extern void exif_entry_gps_initialize(ExifEntry * e, ExifTag tag)
{
    exif_entry_initialize(e,tag);
}


}
