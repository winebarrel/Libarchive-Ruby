#include "libarchive_internal.h"

VALUE rb_mArchive;
VALUE rb_eArchiveError;

/* */
VALUE rb_libarchive_s_version_number(VALUE self) {
#if ARCHIVE_VERSION >= 2005000
  return INT2NUM(archive_version_number());
#else
  return INT2NUM(archive_version_stamp());
#endif
}

/* */
VALUE rb_libarchive_s_version_string(VALUE self) {
#if ARCHIVE_VERSION_NUMBER >= 2005000
  return rb_str_new2(archive_version_string());
#else
  return rb_str_new2(archive_version());
#endif
}

void DLLEXPORT Init_rblibarchive() {
  rb_mArchive = rb_define_module("Archive");
  rb_define_const(rb_mArchive, "VERSION", rb_str_new2(VERSION));

  rb_define_const(rb_mArchive, "COMPRESSION_NONE",     INT2NUM(ARCHIVE_COMPRESSION_NONE));
#ifdef HAVE_ZLIB_H
  rb_define_const(rb_mArchive, "COMPRESSION_GZIP",     INT2NUM(ARCHIVE_COMPRESSION_GZIP));
#endif
#ifdef HAVE_BZLIB_H
  rb_define_const(rb_mArchive, "COMPRESSION_BZIP2",    INT2NUM(ARCHIVE_COMPRESSION_BZIP2));
#endif
  rb_define_const(rb_mArchive, "COMPRESSION_COMPRESS", INT2NUM(ARCHIVE_COMPRESSION_COMPRESS));
  // XXX:
  /*
  rb_define_const(rb_mArchive, "COMPRESSION_PROGRAM",  INT2NUM(ARCHIVE_COMPRESSION_PROGRAM));
  rb_define_const(rb_mArchive, "COMPRESSION_LZMA",     INT2NUM(ARCHIVE_COMPRESSION_LZMA));
  */

  rb_define_const(rb_mArchive, "FORMAT_BASE_MASK",           INT2NUM(ARCHIVE_FORMAT_BASE_MASK));
  rb_define_const(rb_mArchive, "FORMAT_CPIO",                INT2NUM(ARCHIVE_FORMAT_CPIO));
  rb_define_const(rb_mArchive, "FORMAT_CPIO_POSIX",          INT2NUM(ARCHIVE_FORMAT_CPIO_POSIX));
  rb_define_const(rb_mArchive, "FORMAT_CPIO_BIN_LE",         INT2NUM(ARCHIVE_FORMAT_CPIO_BIN_LE));
  rb_define_const(rb_mArchive, "FORMAT_CPIO_BIN_BE",         INT2NUM(ARCHIVE_FORMAT_CPIO_BIN_BE));
  rb_define_const(rb_mArchive, "FORMAT_CPIO_SVR4_NOCRC",     INT2NUM(ARCHIVE_FORMAT_CPIO_SVR4_NOCRC));
  rb_define_const(rb_mArchive, "FORMAT_CPIO_SVR4_CRC",       INT2NUM(ARCHIVE_FORMAT_CPIO_SVR4_CRC));
  rb_define_const(rb_mArchive, "FORMAT_SHAR",                INT2NUM(ARCHIVE_FORMAT_SHAR));
  rb_define_const(rb_mArchive, "FORMAT_SHAR_BASE",           INT2NUM(ARCHIVE_FORMAT_SHAR_BASE));
  rb_define_const(rb_mArchive, "FORMAT_SHAR_DUMP",           INT2NUM(ARCHIVE_FORMAT_SHAR_DUMP));
  rb_define_const(rb_mArchive, "FORMAT_TAR",                 INT2NUM(ARCHIVE_FORMAT_TAR));
  rb_define_const(rb_mArchive, "FORMAT_TAR_USTAR",           INT2NUM(ARCHIVE_FORMAT_TAR_USTAR));
  rb_define_const(rb_mArchive, "FORMAT_TAR_PAX_INTERCHANGE", INT2NUM(ARCHIVE_FORMAT_TAR_PAX_INTERCHANGE));
  rb_define_const(rb_mArchive, "FORMAT_TAR_PAX_RESTRICTED",  INT2NUM(ARCHIVE_FORMAT_TAR_PAX_RESTRICTED));
  rb_define_const(rb_mArchive, "FORMAT_TAR_GNUTAR",          INT2NUM(ARCHIVE_FORMAT_TAR_GNUTAR));
  rb_define_const(rb_mArchive, "FORMAT_ISO9660",             INT2NUM(ARCHIVE_FORMAT_ISO9660));
  rb_define_const(rb_mArchive, "FORMAT_ISO9660_ROCKRIDGE",   INT2NUM(ARCHIVE_FORMAT_ISO9660_ROCKRIDGE));
  rb_define_const(rb_mArchive, "FORMAT_ZIP",                 INT2NUM(ARCHIVE_FORMAT_ZIP));
  rb_define_const(rb_mArchive, "FORMAT_EMPTY",               INT2NUM(ARCHIVE_FORMAT_EMPTY));
  rb_define_const(rb_mArchive, "FORMAT_AR",                  INT2NUM(ARCHIVE_FORMAT_AR));
  rb_define_const(rb_mArchive, "FORMAT_AR_GNU",              INT2NUM(ARCHIVE_FORMAT_AR_GNU));
  rb_define_const(rb_mArchive, "FORMAT_AR_BSD",              INT2NUM(ARCHIVE_FORMAT_AR_BSD));
#ifdef ARCHIVE_FORMAT_MTREE
  rb_define_const(rb_mArchive, "FORMAT_MTREE",               INT2NUM(ARCHIVE_FORMAT_MTREE));
#endif

  rb_define_const(rb_mArchive, "EXTRACT_OWNER",              INT2NUM(ARCHIVE_EXTRACT_OWNER));
  rb_define_const(rb_mArchive, "EXTRACT_PERM",               INT2NUM(ARCHIVE_EXTRACT_PERM));
  rb_define_const(rb_mArchive, "EXTRACT_TIME",               INT2NUM(ARCHIVE_EXTRACT_TIME));
  rb_define_const(rb_mArchive, "EXTRACT_NO_OVERWRITE",       INT2NUM(ARCHIVE_EXTRACT_NO_OVERWRITE));
  rb_define_const(rb_mArchive, "EXTRACT_UNLINK",             INT2NUM(ARCHIVE_EXTRACT_UNLINK));
  rb_define_const(rb_mArchive, "EXTRACT_ACL",                INT2NUM(ARCHIVE_EXTRACT_ACL));
  rb_define_const(rb_mArchive, "EXTRACT_FFLAGS",             INT2NUM(ARCHIVE_EXTRACT_FFLAGS));
  rb_define_const(rb_mArchive, "EXTRACT_XATTR",              INT2NUM(ARCHIVE_EXTRACT_XATTR));
  rb_define_const(rb_mArchive, "EXTRACT_SECURE_SYMLINKS",    INT2NUM(ARCHIVE_EXTRACT_SECURE_SYMLINKS));
  rb_define_const(rb_mArchive, "EXTRACT_SECURE_NODOTDOT",    INT2NUM(ARCHIVE_EXTRACT_SECURE_NODOTDOT));
  rb_define_const(rb_mArchive, "EXTRACT_NO_AUTODIR",         INT2NUM(ARCHIVE_EXTRACT_NO_AUTODIR));
  rb_define_const(rb_mArchive, "EXTRACT_NO_OVERWRITE_NEWER", INT2NUM(ARCHIVE_EXTRACT_NO_OVERWRITE_NEWER));
#ifdef ARCHIVE_EXTRACT_SPARSE
  rb_define_const(rb_mArchive, "EXTRACT_SPARSE",             INT2NUM(ARCHIVE_EXTRACT_SPARSE));
#endif

  rb_define_module_function(rb_mArchive, "version_number", rb_libarchive_s_version_number, 0);
  rb_define_module_function(rb_mArchive, "version_string", rb_libarchive_s_version_string, 0);

  rb_eArchiveError = rb_define_class_under(rb_mArchive, "Error", rb_eStandardError);

  Init_libarchive_reader();
  Init_libarchive_writer();
  Init_libarchive_archive();
  Init_libarchive_entry();
}
