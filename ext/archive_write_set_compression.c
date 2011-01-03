#include <errno.h>
#include <archive.h>

static struct {
  int code;
  int (*setter)(struct archive *);
} codes[] = {
  { ARCHIVE_COMPRESSION_NONE,     archive_write_set_compression_none     },
  { ARCHIVE_COMPRESSION_GZIP,     archive_write_set_compression_gzip     },
  { ARCHIVE_COMPRESSION_BZIP2,    archive_write_set_compression_bzip2    },
#if ARCHIVE_VERSION_NUMBER >= 2005000
  { ARCHIVE_COMPRESSION_COMPRESS, archive_write_set_compression_compress },
#endif
  // XXX:
  /*
  { ARCHIVE_COMPRESSION_PROGRAM,  archive_read_support_compression_program  },
  */
  { -1,                           NULL                                      },
};

int archive_write_set_compression(struct archive *a, int code) {
  int i;

  for (i = 0; codes[i].code >= 0; i++) {
    if (codes[i].code == code) {
      return codes[i].setter(a);
    }
  }

  archive_set_error(a, EINVAL, "No such compression");
  return ARCHIVE_FATAL;
}
