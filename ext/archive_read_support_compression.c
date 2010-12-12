#include <errno.h>
#include <archive.h>

static struct {
  int code;
  int (*setter)(struct archive *);
} codes[] = {
  { ARCHIVE_COMPRESSION_NONE,     archive_read_support_compression_none     },
  { ARCHIVE_COMPRESSION_GZIP,     archive_read_support_compression_gzip     },
  { ARCHIVE_COMPRESSION_BZIP2,    archive_read_support_compression_bzip2    },
  { ARCHIVE_COMPRESSION_COMPRESS, archive_read_support_compression_compress },
  // XXX:
  /*
  { ARCHIVE_COMPRESSION_PROGRAM,  archive_read_support_compression_program  },
  { ARCHIVE_COMPRESSION_LZMA,     archive_read_support_compression_lzma     },
  */
  { -1,                           NULL                                      },
};

int archive_read_support_compression(struct archive *a, int code) {
  int i;

  for (i = 0; codes[i].code >= 0; i++) {
    if (codes[i].code == code) {
      return codes[i].setter(a);
    }
  }

  archive_set_error(a, EINVAL, "No such compression");
  return ARCHIVE_FATAL;
}
