#include "libarchive_internal.h"

extern VALUE rb_cArchiveReader;
extern VALUE rb_cArchiveWriter;
extern VALUE rb_eArchiveError;

static void rb_libarchive_archive_free(struct rb_libarchive_archive_container *p) {
  xfree(p);
}

static void rb_libarchive_archive_mark(struct rb_libarchive_archive_container *p) {
  rb_gc_mark(p->memory);
}

VALUE rb_libarchive_archive_alloc(VALUE klass) {
  struct rb_libarchive_archive_container *p = ALLOC(struct rb_libarchive_archive_container);
  p->ar = NULL;
  p->eof = 0;
  p->memory = Qnil;
  return Data_Wrap_Struct(klass, rb_libarchive_archive_mark, rb_libarchive_archive_free, p);
}

/* */
static VALUE rb_libarchive_archive_position_compressed(VALUE self) {
  struct rb_libarchive_archive_container *p;
  Data_Get_Struct(self, struct rb_libarchive_archive_container, p);
  Check_Archive(p);
  return LONG2NUM(archive_position_compressed(p->ar));
}

/* */
static VALUE rb_libarchive_archive_position_uncompressed(VALUE self) {
  struct rb_libarchive_archive_container *p;
  Data_Get_Struct(self, struct rb_libarchive_archive_container, p);
  Check_Archive(p);
  return LONG2NUM(archive_position_uncompressed(p->ar));
}

/* */
static VALUE rb_libarchive_archive_compression_name(VALUE self) {
  struct rb_libarchive_archive_container *p;
  Data_Get_Struct(self, struct rb_libarchive_archive_container, p);
  Check_Archive(p);
  return rb_str_new2(archive_compression_name(p->ar));
}

/* */
static VALUE rb_libarchive_archive_compression(VALUE self) {
  struct rb_libarchive_archive_container *p;
  Data_Get_Struct(self, struct rb_libarchive_archive_container, p);
  Check_Archive(p);
  return INT2NUM(archive_compression(p->ar));
}

/* */
static VALUE rb_libarchive_archive_format_name(VALUE self) {
  struct rb_libarchive_archive_container *p;
  Data_Get_Struct(self, struct rb_libarchive_archive_container, p);
  Check_Archive(p);
  return rb_str_new2(archive_format_name(p->ar));
}

/* */
static VALUE rb_libarchive_archive_format(VALUE self) {
  struct rb_libarchive_archive_container *p;
  Data_Get_Struct(self, struct rb_libarchive_archive_container, p);
  Check_Archive(p);
  return NUM2INT(archive_format(p->ar));
}

void Init_libarchive_archive() {
  rb_define_method(rb_cArchiveReader, "position_compressed", rb_libarchive_archive_position_compressed, 0);
  rb_define_method(rb_cArchiveWriter, "position_compressed", rb_libarchive_archive_position_compressed, 0);
  rb_define_method(rb_cArchiveReader, "position_uncompressed", rb_libarchive_archive_position_uncompressed, 0);
  rb_define_method(rb_cArchiveWriter, "position_uncompressed", rb_libarchive_archive_position_uncompressed, 0);
  rb_define_method(rb_cArchiveReader, "compression_name", rb_libarchive_archive_compression_name, 0);
  rb_define_method(rb_cArchiveWriter, "compression_name", rb_libarchive_archive_compression_name, 0);
  rb_define_method(rb_cArchiveReader, "compression", rb_libarchive_archive_compression, 0);
  rb_define_method(rb_cArchiveWriter, "compression", rb_libarchive_archive_compression, 0);
  rb_define_method(rb_cArchiveReader, "format_name", rb_libarchive_archive_format_name, 0);
  rb_define_method(rb_cArchiveWriter, "format_name", rb_libarchive_archive_format_name, 0);
  rb_define_method(rb_cArchiveReader, "format", rb_libarchive_archive_format, 0);
  rb_define_method(rb_cArchiveWriter, "format", rb_libarchive_archive_format, 0);
}
