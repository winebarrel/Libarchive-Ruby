#include "libarchive_internal.h"

extern VALUE rb_mArchive;
VALUE rb_cArchiveWriter;
extern VALUE rb_eArchiveError;
extern VALUE rb_cArchiveEntry;

static void rb_libarchive_writer_close0(struct rb_libarchive_archive_container *p) {
  archive_write_close(p->ar);
  archive_write_finish(p->ar);
  p->ar = NULL;
}

/* */
static VALUE rb_libarchive_writer_close(VALUE self) {
  struct rb_libarchive_archive_container *p;
  Data_Get_Struct(self, struct rb_libarchive_archive_container, p);
  Check_Archive(p);
  rb_libarchive_writer_close0(p);
  return Qnil;
}

static VALUE rb_libarchive_writer_s_open0(int (*archive_open)(struct rb_libarchive_archive_container *, void *), void *arg, int compression, int format, const char *cmd) {
  VALUE writer;
  struct rb_libarchive_archive_container *p;
  int r;
  writer = rb_funcall(rb_cArchiveWriter, rb_intern("new"), 0);
  Data_Get_Struct(writer, struct rb_libarchive_archive_container, p);

  if ((p->ar = archive_write_new()) == NULL) {
    rb_raise(rb_eArchiveError, "Open writer failed: %s", strerror(errno));
  }

  if (cmd != NULL) {
    r = archive_write_set_compression_program(p->ar,  cmd);
  } else {
    r = archive_write_set_compression(p->ar, compression);
  }

  if (r != ARCHIVE_OK) {
    char error_string[BUFSIZ];
    archive_copy_error_string(p->ar, error_string, BUFSIZ);
    rb_libarchive_writer_close0(p);
    rb_raise(rb_eArchiveError, "Set compression failed: %s", error_string);
  }

  if (archive_write_set_format(p->ar, format) != ARCHIVE_OK) {
    char error_string[BUFSIZ];
    archive_copy_error_string(p->ar, error_string, BUFSIZ);
    rb_libarchive_writer_close0(p);
    rb_raise(rb_eArchiveError, "Set format failed: %s", error_string);
  }

  if (archive_open(p, arg) != ARCHIVE_OK) {
    char error_string[BUFSIZ];
    archive_copy_error_string(p->ar, error_string, BUFSIZ);
    rb_libarchive_writer_close0(p);
    rb_raise(rb_eArchiveError, "Open writer failed: %s", error_string);
  }

  if (rb_block_given_p()) {
    VALUE retval;
    int status;
    retval = rb_protect(rb_yield, writer, &status);
    rb_libarchive_writer_close0(p);

    if (status != 0) {
      rb_jump_tag(status);
    }

    return retval;
  } else {
    return writer;
  }
}

static int rb_libarchive_writer_s_open_filename0(struct rb_libarchive_archive_container *p, void *arg) {
  const char *filename = (const char *) arg;
  return archive_write_open_filename(p->ar, filename);
}

/* */
static VALUE rb_libarchive_writer_s_open_filename(VALUE self, VALUE v_filename, VALUE v_compression, VALUE v_format) {
  const char *filename = NULL;
  int compression, format;
  const char *cmd = NULL;
  Check_Type(v_filename, T_STRING);

  if (RSTRING_LEN(v_filename) < 1) {
    rb_raise(rb_eArchiveError, "Open writer failed: No such file or directory");
  }

  filename = RSTRING_PTR(v_filename);

  if (T_STRING == TYPE(v_compression)) {
    compression = -1;
    cmd = RSTRING_PTR(v_compression);
  } else {
    compression = NUM2INT(v_compression);
  }


  format = NUM2INT(v_format);
  return rb_libarchive_writer_s_open0(rb_libarchive_writer_s_open_filename0, (void *) filename, compression, format, cmd);
}

static int rb_libarchive_writer_s_open_memory0(struct rb_libarchive_archive_container *p, void *arg) {
  VALUE str = (VALUE) arg;
  p->memory = str;
  return archive_write_open_rb_str(p->ar, str);
}

/* */
static VALUE rb_libarchive_writer_s_open_memory(VALUE self, VALUE v_memory, VALUE v_compression, VALUE v_format) {
  int compression, format;
  const char *cmd = NULL;
  Check_Type(v_memory, T_STRING);

  if (T_STRING == TYPE(v_compression)) {
    compression = -1;
    cmd = RSTRING_PTR(v_compression);
  } else {
    compression = NUM2INT(v_compression);
  }

  format = NUM2INT(v_format);
  return rb_libarchive_writer_s_open0(rb_libarchive_writer_s_open_memory0, (void *) v_memory, compression, format, cmd);
}

/* */
static VALUE rb_libarchive_writer_new_entry(VALUE self) {
  VALUE entry;
  struct rb_libarchive_archive_container *p;
  struct archive_entry *ae;
  Data_Get_Struct(self, struct rb_libarchive_archive_container, p);
  Check_Archive(p);

  if ((ae = archive_entry_new()) == NULL) {
    rb_raise(rb_eArchiveError, "New entry failed: %s", strerror(errno));
  }

  entry = rb_libarchive_entry_new(ae, 1);

  if (rb_block_given_p()) {
    VALUE retval;
    int status;
    retval = rb_protect(rb_yield, entry, &status);
    rb_libarchive_entry_close(entry);

    if (status != 0) {
      rb_jump_tag(status);
    }

    return retval;
  } else {
    return entry;
  }
}

/* */
static VALUE rb_libarchive_writer_write_header(VALUE self, VALUE v_entry) {
  struct rb_libarchive_archive_container *pa;
  struct rb_libarchive_entry_container *pae;
  Check_Class(v_entry, rb_cArchiveEntry);
  Data_Get_Struct(self, struct rb_libarchive_archive_container, pa);
  Check_Archive(pa);
  Data_Get_Struct(v_entry, struct rb_libarchive_entry_container, pae);
  Check_Entry(pae);

  if (archive_write_header(pa->ar, pae->ae) != ARCHIVE_OK) {
    rb_raise(rb_eArchiveError, "Write header failed: %s", archive_error_string(pa->ar));
  }

  return Qnil;
}

static ssize_t rb_libarchive_writer_write_data0(struct archive *ar, VALUE v_buff) {
  const char *buff;
  size_t size;
  ssize_t n;

  if (NIL_P(v_buff)) {
    return 0;
  }

  Check_Type(v_buff, T_STRING);
  buff = RSTRING_PTR(v_buff);
  size = RSTRING_LEN(v_buff);

  if (size < 1) {
    return 0;
  }

  if ((n = archive_write_data(ar, buff, size)) < 0) {
    rb_raise(rb_eArchiveError, "Write data failed: %s", archive_error_string(ar));
  }

  return n;
}

/* */
static VALUE rb_libarchive_writer_write_data(int argc, VALUE *argv, VALUE self) {
  struct rb_libarchive_archive_container *p;
  Data_Get_Struct(self, struct rb_libarchive_archive_container, p);
  Check_Archive(p);

  if (rb_block_given_p()) {
    ssize_t len = 0;

    if (argc > 0) {
      rb_raise(rb_eArgError, "wrong number of arguments (%d for 0)", argc);
    }

    while(1) {
      VALUE retval;
      ssize_t n;
      retval = rb_yield(Qnil);

      if ((n = rb_libarchive_writer_write_data0(p->ar, retval)) < 1) {
        return LONG2NUM(len);
      }

      len += n;
    }
  } else {
    VALUE v_buff;
    ssize_t n;
    rb_scan_args(argc, argv, "10", &v_buff);
    n = rb_libarchive_writer_write_data0(p->ar, v_buff);
    return LONG2NUM(n);
  }
}

void Init_libarchive_writer() {
  rb_cArchiveWriter = rb_define_class_under(rb_mArchive, "Writer", rb_cObject);
  rb_define_alloc_func(rb_cArchiveWriter, rb_libarchive_archive_alloc);
  rb_funcall(rb_cArchiveWriter, rb_intern("private_class_method"), 1, ID2SYM(rb_intern("new")));
  rb_define_singleton_method(rb_cArchiveWriter, "open_filename", rb_libarchive_writer_s_open_filename, 3);
  rb_define_singleton_method(rb_mArchive, "write_open_filename", rb_libarchive_writer_s_open_filename, 3);
  rb_define_singleton_method(rb_cArchiveWriter, "open_memory", rb_libarchive_writer_s_open_memory, 3);
  rb_define_singleton_method(rb_mArchive, "write_open_memory", rb_libarchive_writer_s_open_memory, 3);
  rb_define_method(rb_cArchiveWriter, "close", rb_libarchive_writer_close, 0);
  rb_define_method(rb_cArchiveWriter, "new_entry", rb_libarchive_writer_new_entry, 0);
  rb_define_method(rb_cArchiveWriter, "write_header", rb_libarchive_writer_write_header, 1);
  rb_define_method(rb_cArchiveWriter, "write_data", rb_libarchive_writer_write_data, -1);
}
