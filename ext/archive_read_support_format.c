#include <errno.h>
#include <archive.h>

static struct {
  int code;
  int (*setter)(struct archive *);
} codes[] = {
  { ARCHIVE_FORMAT_CPIO,       archive_read_support_format_cpio    },
  { ARCHIVE_FORMAT_TAR,        archive_read_support_format_tar     },
  { ARCHIVE_FORMAT_TAR_GNUTAR, archive_read_support_format_gnutar  },
  { ARCHIVE_FORMAT_ISO9660,    archive_read_support_format_iso9660 },
  { ARCHIVE_FORMAT_ZIP,        archive_read_support_format_zip     },
  { ARCHIVE_FORMAT_EMPTY,      archive_read_support_format_empty   },
  { ARCHIVE_FORMAT_AR,         archive_read_support_format_ar      },
#ifdef ARCHIVE_FORMAT_MTREE
  { ARCHIVE_FORMAT_MTREE,      archive_read_support_format_mtree   },
#endif
  { -1,                        NULL                                },
};

int archive_read_support_format(struct archive *a, int code) {
  int i;

  for (i = 0; codes[i].code >= 0; i++) {
    if (codes[i].code == code) {
      return codes[i].setter(a);
    }
  }

  archive_set_error(a, EINVAL, "No such format");
  return ARCHIVE_FATAL;
}
