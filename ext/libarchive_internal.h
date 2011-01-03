#ifndef _LIBARCHIVE_INTERNAL_H_
#define _LIBARCHIVE_INTERNAL_H_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#ifndef _WIN32
#include <unistd.h>
#endif

#ifdef _WIN32
#ifdef stat
#undef stat
#endif
#define stat _stat
#endif // _WIN32

#include <archive.h>
#include <archive_entry.h>

#include <ruby.h>
#include <rubysig.h>

#ifdef _WIN32
#include "libarchive_win32.h"
#endif

#include "archive_write_open_rb_str.h"
#include "archive_read_support_compression.h"
#include "archive_read_support_format.h"
#include "archive_write_set_compression.h"

#ifdef LIBARCHIVE_EXPORTS
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

#define VERSION          "0.1.2"
#define BLOCK_SIZE       10240
#define DATA_BUFFER_SIZE 65536

#ifndef RSTRING_PTR
#define RSTRING_PTR(s) (RSTRING(s)->ptr)
#endif
#ifndef RSTRING_LEN
#define RSTRING_LEN(s) (RSTRING(s)->len)
#endif

#ifdef O_NONBLOCK
#define LIBARCHIVE_O_NONBLOCK O_NONBLOCK
#elif O_NDELAY
#define LIBARCHIVE_O_NONBLOCK O_NDELAY
#else
#define LIBARCHIVE_O_NONBLOCK 0
#endif

#ifdef O_NOCTTY
#define LIBARCHIVE_O_NOCTTY O_NOCTTY
#else
#define LIBARCHIVE_O_NOCTTY 0
#endif

#ifdef O_BINARY
#define LIBARCHIVE_O_BINARY O_BINARY
#else
#define LIBARCHIVE_O_BINARY 0
#endif

#ifdef O_SYNC
#define LIBARCHIVE_O_SYNC O_SYNC
#else
#define LIBARCHIVE_O_SYNC 0
#endif

#define O_FLAGS (O_RDONLY | O_WRONLY | O_RDWR | O_APPEND | O_CREAT | O_EXCL | O_TRUNC | \
                 LIBARCHIVE_O_NONBLOCK | LIBARCHIVE_O_NOCTTY | LIBARCHIVE_O_BINARY | LIBARCHIVE_O_SYNC)

#define EXTRACT_FLAGS_WITHOUT_ARCHIVE_EXTRACT_SPARSE \
  (ARCHIVE_EXTRACT_OWNER | ARCHIVE_EXTRACT_PERM | ARCHIVE_EXTRACT_TIME | \
   ARCHIVE_EXTRACT_NO_OVERWRITE | ARCHIVE_EXTRACT_UNLINK | ARCHIVE_EXTRACT_ACL | \
   ARCHIVE_EXTRACT_FFLAGS | ARCHIVE_EXTRACT_XATTR | ARCHIVE_EXTRACT_SECURE_SYMLINKS | \
   ARCHIVE_EXTRACT_SECURE_NODOTDOT | ARCHIVE_EXTRACT_NO_AUTODIR | \
   ARCHIVE_EXTRACT_NO_OVERWRITE_NEWER)

#ifdef ARCHIVE_EXTRACT_SPARSE
#define EXTRACT_FLAGS (EXTRACT_FLAGS_WITHOUT_ARCHIVE_EXTRACT_SPARSE | ARCHIVE_EXTRACT_SPARSE)
#else
#define EXTRACT_FLAGS EXTRACT_FLAGS_WITHOUT_ARCHIVE_EXTRACT_SPARSE
#endif

struct rb_libarchive_archive_container {
  struct archive *ar;
  int eof;
  VALUE memory;
};

struct rb_libarchive_entry_container {
  struct archive_entry *ae;
  int must_close;
};

#define Check_Archive(p) do {                      \
  if ((p)->ar == NULL) {                           \
    rb_raise(rb_eArchiveError, "Invalid archive"); \
  }                                                \
} while(0)

#define Check_Entry(p) do {                      \
  if ((p)->ae == NULL) {                         \
    rb_raise(rb_eArchiveError, "Invalid entry"); \
  }                                              \
} while(0)

#define Check_Class(v, c) do {                                      \
  if (!rb_obj_is_instance_of((v), (c))) {                           \
    rb_raise(rb_eTypeError, "wrong argument type %s (expected %s)", \
      rb_class2name(CLASS_OF(v)), rb_class2name(c));                \
  }                                                                 \
} while(0)

#define Check_Class(v, c) do {                                      \
  if (!rb_obj_is_instance_of((v), (c))) {                           \
    rb_raise(rb_eTypeError, "wrong argument type %s (expected %s)", \
      rb_class2name(CLASS_OF(v)), rb_class2name(c));                \
  }                                                                 \
} while(0)

#define LONG2TIME(i) rb_funcall(rb_cTime, rb_intern("at"), 1, LONG2NUM(i))
#define TIME2LONG(v) NUM2LONG(rb_funcall((v), rb_intern("tv_sec"), 0))

#define archive_copy_error_string(a, b, n) do { \
  strncpy((b), archive_error_string(a), (n));   \
  (b)[((n) - 1)] = '\0';                        \
} while(0)

VALUE rb_libarchive_archive_alloc(VALUE klass);
VALUE rb_libarchive_entry_new(struct archive_entry *ae, int must_close);
VALUE rb_libarchive_entry_close(VALUE self);

void Init_libarchive_reader();
void Init_libarchive_writer();
void Init_libarchive_archive();
void Init_libarchive_entry();

#endif // _LIBARCHIVE_INTERNAL_H_
