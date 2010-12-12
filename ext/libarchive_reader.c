#include "libarchive_internal.h"

extern VALUE rb_mArchive;
VALUE rb_cArchiveReader;
extern VALUE rb_eArchiveError;
extern VALUE rb_cArchiveEntry;

static void rb_libarchive_reader_close0(struct rb_libarchive_archive_container *p) {
  archive_read_close(p->ar);
  archive_read_finish(p->ar);
  p->ar = NULL;
}

/* */
static VALUE rb_libarchive_reader_close(VALUE self) {
  struct rb_libarchive_archive_container *p;
  Data_Get_Struct(self, struct rb_libarchive_archive_container, p);
  Check_Archive(p);
  rb_libarchive_reader_close0(p);
  return Qnil;
}

static VALUE rb_libarchive_reader_s_open0(int (*archive_open)(struct rb_libarchive_archive_container *, void *), void *arg, int compression, int format, const char *cmd) {
  VALUE reader;
  struct rb_libarchive_archive_container *p;
  int r;
  reader = rb_funcall(rb_cArchiveReader, rb_intern("new"), 0);
  Data_Get_Struct(reader, struct rb_libarchive_archive_container, p);

  if ((p->ar = archive_read_new()) == NULL) {
    rb_raise(rb_eArchiveError, "Open reader failed: %s", strerror(errno));
  }

  if (cmd != NULL) {
    r = archive_read_support_compression_program(p->ar, cmd);
  } else if (compression != -1) {
    r = archive_read_support_compression(p->ar, compression);
  } else {
    r = archive_read_support_compression_all(p->ar);
  }

  if (r != ARCHIVE_OK) {
    char error_string[BUFSIZ];
    archive_copy_error_string(p->ar, error_string, BUFSIZ);
    rb_libarchive_reader_close0(p);
    rb_raise(rb_eArchiveError, "Support compression failed: %s", error_string);
  }

  if (format != -1) {
    r = archive_read_support_format(p->ar, format);
  } else {
    r = archive_read_support_format_all(p->ar);
  }

  if (r != ARCHIVE_OK) {
    char error_string[BUFSIZ];
    archive_copy_error_string(p->ar, error_string, BUFSIZ);
    rb_libarchive_reader_close0(p);
    rb_raise(rb_eArchiveError, "Support format failed: %s", error_string);
  }

  if (archive_open(p, arg) != ARCHIVE_OK) {
    char error_string[BUFSIZ];
    archive_copy_error_string(p->ar, error_string, BUFSIZ);
    rb_libarchive_reader_close0(p);
    rb_raise(rb_eArchiveError, "Open reader failed: %s", error_string);
  }

  if (rb_block_given_p()) {
    VALUE retval;
    int status;
    retval = rb_protect(rb_yield, reader, &status);
    rb_libarchive_reader_close0(p);

    if (status != 0) {
      rb_jump_tag(status);
    }

    return retval;
  } else {
    return reader;
  }
}

static int rb_libarchive_reader_s_open_filename0(struct rb_libarchive_archive_container *p, void *arg) {
  const char *filename = (const char *) arg;

  if (filename != NULL) {
    struct stat s;

    if (stat(filename, &s) != 0) {
      archive_set_error(p->ar, -1, strerror(errno));
      return (ARCHIVE_FATAL);
    }
  }

  return archive_read_open_filename(p->ar, filename, BLOCK_SIZE);
}

/* */
static VALUE rb_libarchive_reader_s_open_filename(int argc, VALUE *argv, VALUE self) {
  VALUE v_filename, v_compression, v_format;
  const char *filename = NULL;
  int compression = -1, format = -1;
  const char *cmd = NULL;
  rb_scan_args(argc, argv, "12", &v_filename, &v_compression, &v_format);
  Check_Type(v_filename, T_STRING);
  filename = RSTRING_PTR(v_filename);

  if (T_STRING == TYPE(v_compression)) {
    compression = -1;
    cmd = RSTRING_PTR(v_compression);
  } else if (!NIL_P(v_compression)) {
    compression = NUM2INT(v_compression);
  }

  if (!NIL_P(v_format)) {
    format = NUM2INT(v_format);
  }

  return rb_libarchive_reader_s_open0(rb_libarchive_reader_s_open_filename0, (void *) filename, compression, format, cmd);
}

static int rb_libarchive_reader_s_open_memory0(struct rb_libarchive_archive_container *p, void *arg) {
  VALUE v_buff = (VALUE) arg;
  return archive_read_open_memory(p->ar, RSTRING_PTR(v_buff), RSTRING_LEN(v_buff));
}

/* */
static VALUE rb_libarchive_reader_s_open_memory(int argc, VALUE *argv, VALUE self) {
  VALUE v_memory, v_compression, v_format;
  int compression = -1, format = -1;
  const char *cmd = NULL;
  rb_scan_args(argc, argv, "12", &v_memory, &v_compression, &v_format);
  Check_Type(v_memory, T_STRING);

  if (T_STRING == TYPE(v_compression)) {
    compression = -1;
    cmd = RSTRING_PTR(v_compression);
  } else if (!NIL_P(v_compression)) {
    compression = NUM2INT(v_compression);
  }

  if (!NIL_P(v_format)) {
    format = NUM2INT(v_format);
  }

  return rb_libarchive_reader_s_open0(rb_libarchive_reader_s_open_memory0, (void *) v_memory, compression, format, cmd);
}

/* */
static VALUE rb_libarchive_reader_next_header(VALUE self) {
  struct rb_libarchive_archive_container *p;
  struct archive_entry *ae;
  int r;
  Data_Get_Struct(self, struct rb_libarchive_archive_container, p);
  Check_Archive(p);

  if (p->eof) {
    return Qnil;
  }

  r = archive_read_next_header(p->ar, &ae);

  if (r == ARCHIVE_EOF) {
    p->eof = 1;
    return Qnil;
  } else if (r != ARCHIVE_OK) {
    rb_raise(rb_eArchiveError, "Fetch entry failed: %s", archive_error_string(p->ar));
  }

  return rb_libarchive_entry_new(ae, 0);
}

/* */
static VALUE rb_libarchive_reader_header_position(VALUE self) {
  struct rb_libarchive_archive_container *p;
  Data_Get_Struct(self, struct rb_libarchive_archive_container, p);
  Check_Archive(p);
  return LONG2NUM((long) archive_read_header_position(p->ar));
}

/* */
static VALUE rb_libarchive_reader_read_data(int argc, VALUE *argv, VALUE self) {
  VALUE v_size;
  struct rb_libarchive_archive_container *p;
  char *buff;
  size_t size = DATA_BUFFER_SIZE;
  ssize_t n;
  rb_scan_args(argc, argv, "01", &v_size);

  if (!NIL_P(v_size)) {
    size = NUM2INT(v_size);
  }

  Data_Get_Struct(self, struct rb_libarchive_archive_container, p);
  Check_Archive(p);

  if (p->eof) {
    return Qnil;
  }

  if (rb_block_given_p()) {
    ssize_t len = 0;
    int status = 0;
    buff = xmalloc(size);

    while ((n = archive_read_data(p->ar, buff, size)) > 0) {
      rb_protect(rb_yield, rb_str_new(buff, n), &status);

      if (status != 0) {
        break;
      }

      len += n;
    }

    xfree(buff);

    if (status != 0) {
      rb_jump_tag(status);
    }

    if (n < 0) {
      rb_raise(rb_eArchiveError, "Read data failed: %s", archive_error_string(p->ar));
    }

    return LONG2NUM(len);
  } else {
    VALUE retval = rb_str_new("", 0);
    buff = xmalloc(size);

    while ((n = archive_read_data(p->ar, buff, size)) > 0) {
      rb_str_cat(retval, buff, n);
    }

    xfree(buff);

    if (n < 0) {
      rb_raise(rb_eArchiveError, "Read data failed: %s", archive_error_string(p->ar));
    }

    return retval;
  }
}

/* */
static VALUE rb_libarchive_reader_save_data(int argc, VALUE *argv, VALUE self) {
  VALUE v_filename, v_flags;
  struct rb_libarchive_archive_container *p;
  const char *filename;
  int flags, fd, r;
  rb_scan_args(argc, argv, "11", &v_filename, &v_flags);
  Check_Type(v_filename, T_STRING);
  filename = RSTRING_PTR(v_filename);

  if (!NIL_P(v_flags)) {
    flags = ((O_WRONLY | NUM2INT(v_flags)) & O_FLAGS);
  } else {
    flags = (O_WRONLY | O_CREAT | O_EXCL
#ifdef O_BINARY
             | O_BINARY
#endif
             );
  }

  Data_Get_Struct(self, struct rb_libarchive_archive_container, p);
  Check_Archive(p);

  if ((fd = open(filename, flags)) == -1) {
    rb_raise(rb_eArchiveError, "Save data failed: %s", strerror(errno));
  }

  r = archive_read_data_into_fd(p->ar, fd);
  _close(fd);

  if (r != ARCHIVE_OK) {
    rb_raise(rb_eArchiveError, "Save data failed: %s", archive_error_string(p->ar));
  }

  return Qnil;
}

/* */
static VALUE rb_libarchive_reader_extract(int argc, VALUE *argv, VALUE self) {
  VALUE v_entry, v_flags;
  struct rb_libarchive_archive_container *pa;
  struct rb_libarchive_entry_container *pae;
  int flags = 0;
  rb_scan_args(argc, argv, "11", &v_entry, &v_flags);
  Check_Class(v_entry, rb_cArchiveEntry);

  if (!NIL_P(v_flags)) {
    flags = (NUM2INT(v_flags) & EXTRACT_FLAGS);
  }

  Data_Get_Struct(self, struct rb_libarchive_archive_container, pa);
  Check_Archive(pa);

  if (pa->eof) {
    rb_raise(rb_eArchiveError, "Extract archive failed: It has already reached EOF");
  }

  Data_Get_Struct(v_entry, struct rb_libarchive_entry_container, pae);
  Check_Entry(pae);

  if (archive_read_extract(pa->ar, pae->ae, flags) != ARCHIVE_OK) {
    rb_raise(rb_eArchiveError, "Extract archive failed: %s", archive_error_string(pa->ar));
  }

  return Qnil;
}

void Init_libarchive_reader() {
  rb_cArchiveReader = rb_define_class_under(rb_mArchive, "Reader", rb_cObject);
  rb_define_alloc_func(rb_cArchiveReader, rb_libarchive_archive_alloc);
  rb_funcall(rb_cArchiveReader, rb_intern("private_class_method"), 1, ID2SYM(rb_intern("new")));
  rb_define_singleton_method(rb_cArchiveReader, "open_filename", rb_libarchive_reader_s_open_filename, -1);
  rb_define_module_function(rb_mArchive, "read_open_filename", rb_libarchive_reader_s_open_filename, -1);
  rb_define_singleton_method(rb_cArchiveReader, "open_memory", rb_libarchive_reader_s_open_memory, -1);
  rb_define_module_function(rb_mArchive, "read_open_memory", rb_libarchive_reader_s_open_memory, -1);
  rb_define_method(rb_cArchiveReader, "close", rb_libarchive_reader_close, 0);
  rb_define_method(rb_cArchiveReader, "next_header", rb_libarchive_reader_next_header, 0);
  rb_define_method(rb_cArchiveReader, "header_position", rb_libarchive_reader_header_position, 0);
  rb_define_method(rb_cArchiveReader, "read_data", rb_libarchive_reader_read_data, -1);
  rb_define_method(rb_cArchiveReader, "save_data", rb_libarchive_reader_save_data, -1);
  rb_define_method(rb_cArchiveReader, "extract", rb_libarchive_reader_extract, -1);
}
