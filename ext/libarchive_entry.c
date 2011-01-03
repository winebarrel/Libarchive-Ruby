#include "libarchive_internal.h"

extern VALUE rb_mArchive;
VALUE rb_cArchiveEntry;
extern VALUE rb_eArchiveError;

static void rb_libarchive_entry_free(struct rb_libarchive_entry_container *p) {
  xfree(p);
}

static VALUE rb_libarchive_entry_alloc(VALUE klass) {
  struct rb_libarchive_entry_container *p = ALLOC(struct rb_libarchive_entry_container);
  p->ae = NULL;
  p->must_close = 1;
  return Data_Wrap_Struct(klass, 0, rb_libarchive_entry_free, p);
}

/* */
VALUE rb_libarchive_entry_close(VALUE self) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);

  if (!p->must_close) {
    rb_raise(rb_eArchiveError, "Close entry failed: It is not necessary to close");
  }

  archive_entry_free(p->ae);
  p->ae = NULL;
  return Qnil;
}

VALUE rb_libarchive_entry_new(struct archive_entry *ae, int must_close) {
  VALUE entry;
  struct rb_libarchive_entry_container *p;
  entry = rb_funcall(rb_cArchiveEntry, rb_intern("new"), 0);
  Data_Get_Struct(entry, struct rb_libarchive_entry_container, p);
  p->ae = ae;
  p->must_close = must_close;
  return entry;
}

/* */
static VALUE rb_libarchive_entry_atime(VALUE self) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  return LONG2TIME((long) archive_entry_atime(p->ae));
}

/* */
static VALUE rb_libarchive_entry_atime_nsec(VALUE self) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  return LONG2NUM(archive_entry_atime_nsec(p->ae));
}

/* */
static VALUE rb_libarchive_entry_atime_is_set(VALUE self) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  return archive_entry_atime_is_set(p->ae) ? Qtrue : Qfalse;
}

/* */
static VALUE rb_libarchive_entry_birthtime(VALUE self) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  return LONG2TIME((long) archive_entry_birthtime(p->ae));
}

/* */
static VALUE rb_libarchive_entry_birthtime_nsec(VALUE self) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  return LONG2NUM(archive_entry_birthtime_nsec(p->ae));
}

/* */
static VALUE rb_libarchive_entry_birthtime_is_set(VALUE self) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  return archive_entry_birthtime_is_set(p->ae) ? Qtrue : Qfalse;
}

/* */
static VALUE rb_libarchive_entry_ctime(VALUE self) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  return LONG2TIME((long) archive_entry_ctime(p->ae));
}

/* */
static VALUE rb_libarchive_entry_ctime_nsec(VALUE self) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  return LONG2NUM(archive_entry_ctime_nsec(p->ae));
}

/* */
static VALUE rb_libarchive_entry_ctime_is_set(VALUE self) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  return archive_entry_ctime_is_set(p->ae) ? Qtrue : Qfalse;
}

/* */
static VALUE rb_libarchive_entry_dev(VALUE self) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  return LONG2NUM(archive_entry_dev(p->ae));
}

/* */
static VALUE rb_libarchive_entry_devmajor(VALUE self) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  return LONG2NUM(archive_entry_devmajor(p->ae));
}

/* */
static VALUE rb_libarchive_entry_devminor(VALUE self) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  return LONG2NUM(archive_entry_devminor(p->ae));
}

/* */
static VALUE rb_libarchive_entry_filetype(VALUE self) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  return INT2NUM(archive_entry_filetype(p->ae));
}

/* */
static VALUE rb_libarchive_entry_is_directory(VALUE self) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  return S_ISDIR(archive_entry_filetype(p->ae)) ? Qtrue : Qfalse;
}

/* */
static VALUE rb_libarchive_entry_is_character_special(VALUE self) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  return S_ISCHR(archive_entry_filetype(p->ae)) ? Qtrue : Qfalse;
}

/* */
static VALUE rb_libarchive_entry_is_block_special(VALUE self) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  return S_ISBLK(archive_entry_filetype(p->ae)) ? Qtrue : Qfalse;
}

/* */
static VALUE rb_libarchive_entry_is_regular(VALUE self) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  return S_ISREG(archive_entry_filetype(p->ae)) ? Qtrue : Qfalse;
}

/* */
static VALUE rb_libarchive_entry_is_symbolic_link(VALUE self) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  return S_ISLNK(archive_entry_filetype(p->ae)) ? Qtrue : Qfalse;
}

/* */
static VALUE rb_libarchive_entry_is_socket(VALUE self) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  return S_ISSOCK(archive_entry_filetype(p->ae)) ? Qtrue : Qfalse;
}

/* */
static VALUE rb_libarchive_entry_is_fifo(VALUE self) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  return S_ISFIFO(archive_entry_filetype(p->ae)) ? Qtrue : Qfalse;
}

/* */
static VALUE rb_libarchive_entry_fflags(VALUE self) {
  struct rb_libarchive_entry_container *p;
  long set, clear;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  archive_entry_fflags(p->ae, &set, &clear);
  return rb_ary_new3(2, LONG2NUM(set), LONG2NUM(clear));
}

/* */
static VALUE rb_libarchive_entry_fflags_text(VALUE self) {
  struct rb_libarchive_entry_container *p;
  const char *fflags_text;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  fflags_text = archive_entry_fflags_text(p->ae);
  return (fflags_text != NULL) ? rb_str_new2(fflags_text) : Qnil;
}

/* */
static VALUE rb_libarchive_entry_gid(VALUE self) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  return LONG2NUM(archive_entry_gid(p->ae));
}

/* */
static VALUE rb_libarchive_entry_gname(VALUE self) {
  struct rb_libarchive_entry_container *p;
  const char *gname;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  gname = archive_entry_gname(p->ae);
  return (gname != NULL) ? rb_str_new2(gname) : Qnil;
}

/* */
static VALUE rb_libarchive_entry_hardlink(VALUE self) {
  struct rb_libarchive_entry_container *p;
  const char *hardlink;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  hardlink = archive_entry_hardlink(p->ae);
  return (hardlink != NULL) ? rb_str_new2(hardlink) : Qnil;
}

/* */
static VALUE rb_libarchive_entry_ino(VALUE self) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  return LONG2NUM(archive_entry_ino(p->ae));
}

/* */
static VALUE rb_libarchive_entry_mode(VALUE self) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  return LONG2NUM(archive_entry_mode(p->ae));
}

/* */
static VALUE rb_libarchive_entry_mtime(VALUE self) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  return LONG2TIME((long) archive_entry_mtime(p->ae));
}

/* */
static VALUE rb_libarchive_entry_mtime_nsec(VALUE self) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  return LONG2NUM(archive_entry_mtime_nsec(p->ae));
}

/* */
static VALUE rb_libarchive_entry_mtime_is_set(VALUE self) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  return archive_entry_mtime_is_set(p->ae) ? Qtrue : Qfalse;
}

/* */
static VALUE rb_libarchive_entry_nlink(VALUE self) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  return LONG2NUM(archive_entry_nlink(p->ae));
}

/* */
static VALUE rb_libarchive_entry_pathname(VALUE self) {
  struct rb_libarchive_entry_container *p;
  const char *pathname;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  pathname = archive_entry_pathname(p->ae);
  return (pathname != NULL) ? rb_str_new2(pathname) : Qnil;
}

/* */
static VALUE rb_libarchive_entry_rdev(VALUE self) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  return LONG2NUM(archive_entry_rdev(p->ae));
}

/* */
static VALUE rb_libarchive_entry_rdevmajor(VALUE self) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  return LONG2NUM(archive_entry_rdevmajor(p->ae));
}

/* */
static VALUE rb_libarchive_entry_rdevminor(VALUE self) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  return LONG2NUM(archive_entry_rdevminor(p->ae));
}

#if ARCHIVE_VERSION_NUMBER >= 2005003
/* */
static VALUE rb_libarchive_entry_sourcepath(VALUE self) {
  struct rb_libarchive_entry_container *p;
  const char *sourcepath;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  sourcepath = archive_entry_sourcepath(p->ae);
  return (sourcepath != NULL) ? rb_str_new2(sourcepath) : Qnil;
}
#endif

/* */
static VALUE rb_libarchive_entry_size(VALUE self) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  return LONG2NUM(archive_entry_size(p->ae));
}

/* */
static VALUE rb_libarchive_entry_size_is_set(VALUE self) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  return archive_entry_size_is_set(p->ae) ? Qtrue : Qfalse;
}

#if ARCHIVE_VERSION_NUMBER >= 2003002
/* */
static VALUE rb_libarchive_entry_strmode(VALUE self) {
  struct rb_libarchive_entry_container *p;
  const char *strmode;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  strmode = archive_entry_strmode(p->ae);
  return (strmode != NULL) ? rb_str_new2(strmode) : Qnil;
}
#endif

/* */
static VALUE rb_libarchive_entry_symlink(VALUE self) {
  struct rb_libarchive_entry_container *p;
  const char *symlink;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  symlink = archive_entry_symlink(p->ae);
  return (symlink != NULL) ? rb_str_new2(symlink) : Qnil;
}

/* */
static VALUE rb_libarchive_entry_uid(VALUE self) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  return LONG2NUM(archive_entry_uid(p->ae));
}

/* */
static VALUE rb_libarchive_entry_uname(VALUE self) {
  struct rb_libarchive_entry_container *p;
  const char *uname;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  uname = archive_entry_uname(p->ae);
  return (uname != NULL) ? rb_str_new2(uname) : Qnil;
}

/* */
static VALUE rb_libarchive_entry_set_atime(VALUE self, VALUE v_time) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  Check_Class(v_time, rb_cTime);
  archive_entry_set_atime(p->ae, TIME2LONG(v_time), 0);
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_set_atime2(VALUE self, VALUE v_time, VALUE v_ns) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  Check_Class(v_time, rb_cTime);
  Check_Type(v_ns, T_FIXNUM);
  archive_entry_set_atime(p->ae, TIME2LONG(v_time), NUM2LONG(v_ns));
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_unset_atime(VALUE self) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  archive_entry_unset_atime(p->ae);
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_set_birthtime(VALUE self, VALUE v_time) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  Check_Class(v_time, rb_cTime);
  archive_entry_set_birthtime(p->ae, TIME2LONG(v_time), 0);
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_set_birthtime2(VALUE self, VALUE v_time, VALUE v_ns) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  Check_Class(v_time, rb_cTime);
  Check_Type(v_ns, T_FIXNUM);
  archive_entry_set_birthtime(p->ae, TIME2LONG(v_time), NUM2LONG(v_ns));
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_unset_birthtime(VALUE self) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  archive_entry_unset_birthtime(p->ae);
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_set_ctime(VALUE self, VALUE v_time) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  Check_Class(v_time, rb_cTime);
  archive_entry_set_ctime(p->ae, TIME2LONG(v_time), 0);
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_set_ctime2(VALUE self, VALUE v_time, VALUE v_ns) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  Check_Class(v_time, rb_cTime);
  Check_Type(v_ns, T_FIXNUM);
  archive_entry_set_ctime(p->ae, TIME2LONG(v_time), NUM2LONG(v_ns));
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_unset_ctime(VALUE self) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  archive_entry_unset_ctime(p->ae);
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_set_dev(VALUE self, VALUE v_dev) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  Check_Type(v_dev, T_FIXNUM);
  archive_entry_set_dev(p->ae, NUM2LONG(v_dev));
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_set_devmajor(VALUE self, VALUE v_dev) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  Check_Type(v_dev, T_FIXNUM);
  archive_entry_set_devmajor(p->ae, NUM2LONG(v_dev));
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_set_devminor(VALUE self, VALUE v_dev) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  Check_Type(v_dev, T_FIXNUM);
  archive_entry_set_devminor(p->ae, NUM2LONG(v_dev));
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_set_filetype(VALUE self, VALUE v_type) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  Check_Type(v_type, T_FIXNUM);
  archive_entry_set_filetype(p->ae, NUM2LONG(v_type));
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_set_fflags(VALUE self, VALUE v_set, VALUE v_clear) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  Check_Type(v_set, T_FIXNUM);
  Check_Type(v_clear, T_FIXNUM);
  archive_entry_set_fflags(p->ae, (unsigned long) NUM2LONG(v_set), (unsigned long) NUM2LONG(v_clear));
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_copy_fflags_text(VALUE self, VALUE v_fflags_text) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  Check_Type(v_fflags_text, T_STRING);
  archive_entry_copy_fflags_text(p->ae, RSTRING_PTR(v_fflags_text));
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_set_gid(VALUE self, VALUE v_gid) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  Check_Type(v_gid, T_FIXNUM);
  archive_entry_set_gid(p->ae, NUM2INT(v_gid));
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_set_gname(VALUE self, VALUE v_gname) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  Check_Type(v_gname, T_STRING);
  archive_entry_set_gname(p->ae, RSTRING_PTR(v_gname));
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_copy_gname(VALUE self, VALUE v_gname) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  Check_Type(v_gname, T_STRING);
  archive_entry_copy_gname(p->ae, RSTRING_PTR(v_gname));
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_set_hardlink(VALUE self, VALUE v_hardlink) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  Check_Type(v_hardlink, T_STRING);
  archive_entry_set_hardlink(p->ae, RSTRING_PTR(v_hardlink));
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_copy_hardlink(VALUE self, VALUE v_hardlink) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  Check_Type(v_hardlink, T_STRING);
  archive_entry_copy_hardlink(p->ae, RSTRING_PTR(v_hardlink));
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_set_ino(VALUE self, VALUE v_ino) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  Check_Type(v_ino, T_FIXNUM);
  archive_entry_set_ino(p->ae, (unsigned long) NUM2LONG(v_ino));
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_set_link(VALUE self, VALUE v_link) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  Check_Type(v_link, T_STRING);
  archive_entry_set_link(p->ae, RSTRING_PTR(v_link));
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_copy_link(VALUE self, VALUE v_link) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  Check_Type(v_link, T_STRING);
  archive_entry_copy_link(p->ae, RSTRING_PTR(v_link));
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_set_mode(VALUE self, VALUE v_mode) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  Check_Type(v_mode, T_FIXNUM);
  archive_entry_set_mode(p->ae, NUM2INT(v_mode));
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_set_mtime(VALUE self, VALUE v_time) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  Check_Class(v_time, rb_cTime);
  archive_entry_set_mtime(p->ae, TIME2LONG(v_time), 0);
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_set_mtime2(VALUE self, VALUE v_time, VALUE v_ns) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  Check_Class(v_time, rb_cTime);
  Check_Type(v_ns, T_FIXNUM);
  archive_entry_set_mtime(p->ae, TIME2LONG(v_time), NUM2LONG(v_ns));
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_unset_mtime(VALUE self) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  archive_entry_unset_mtime(p->ae);
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_set_nlink(VALUE self, VALUE v_nlink) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  Check_Type(v_nlink, T_FIXNUM);
  archive_entry_set_nlink(p->ae, NUM2LONG(v_nlink));
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_set_pathname(VALUE self, VALUE v_filename) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  Check_Type(v_filename, T_STRING);
  archive_entry_set_pathname(p->ae, RSTRING_PTR(v_filename));
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_copy_pathname(VALUE self, VALUE v_filename) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  Check_Type(v_filename, T_STRING);
  archive_entry_copy_pathname(p->ae, RSTRING_PTR(v_filename));
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_set_perm(VALUE self, VALUE v_perm) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  Check_Type(v_perm, T_FIXNUM);
  archive_entry_set_perm(p->ae, NUM2INT(v_perm));
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_set_rdev(VALUE self, VALUE v_rdev) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  Check_Type(v_rdev, T_FIXNUM);
  archive_entry_set_rdev(p->ae, NUM2LONG(v_rdev));
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_set_rdevmajor(VALUE self, VALUE v_rdev) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  Check_Type(v_rdev, T_FIXNUM);
  archive_entry_set_rdevmajor(p->ae, NUM2LONG(v_rdev));
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_set_rdevminor(VALUE self, VALUE v_rdev) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  Check_Type(v_rdev, T_FIXNUM);
  archive_entry_set_rdevminor(p->ae, NUM2LONG(v_rdev));
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_set_size(VALUE self, VALUE v_size) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  Check_Type(v_size, T_FIXNUM);
  archive_entry_set_size(p->ae, NUM2LONG(v_size));
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_unset_size(VALUE self) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  archive_entry_unset_size(p->ae);
  return Qnil;
}

#if ARCHIVE_VERSION_NUMBER >= 2005003
/* */
static VALUE rb_libarchive_entry_copy_sourcepath(VALUE self, VALUE v_sourcepath) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  Check_Type(v_sourcepath, T_STRING);
  archive_entry_copy_sourcepath(p->ae, RSTRING_PTR(v_sourcepath));
  return Qnil;
}
#endif

/* */
static VALUE rb_libarchive_entry_set_symlink(VALUE self, VALUE v_symlink) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  Check_Type(v_symlink, T_STRING);
  archive_entry_set_symlink(p->ae, RSTRING_PTR(v_symlink));
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_copy_symlink(VALUE self, VALUE v_symlink) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  Check_Type(v_symlink, T_STRING);
  archive_entry_copy_symlink(p->ae, RSTRING_PTR(v_symlink));
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_set_uid(VALUE self, VALUE v_uid) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  Check_Type(v_uid, T_FIXNUM);
  archive_entry_set_uid(p->ae, NUM2INT(v_uid));
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_set_uname(VALUE self, VALUE v_uname) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  Check_Type(v_uname, T_STRING);
  archive_entry_set_uname(p->ae, RSTRING_PTR(v_uname));
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_copy_uname(VALUE self, VALUE v_uname) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  Check_Type(v_uname, T_STRING);
  archive_entry_copy_uname(p->ae, RSTRING_PTR(v_uname));
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_copy_stat(VALUE self, VALUE v_filename) {
  struct rb_libarchive_entry_container *p;
  const char *filename;
  struct stat s;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  Check_Type(v_filename, T_STRING);
  filename = RSTRING_PTR(v_filename);

  if (stat(filename, &s) != 0) {
    rb_raise(rb_eArchiveError, "Copy stat failed: %", strerror(errno));
  }

  archive_entry_copy_stat(p->ae, &s);
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_copy_lstat(VALUE self, VALUE v_filename) {
  struct rb_libarchive_entry_container *p;
  const char *filename;
  struct stat s;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  Check_Type(v_filename, T_STRING);
  filename = RSTRING_PTR(v_filename);

  if (lstat(filename, &s) != 0) {
    rb_raise(rb_eArchiveError, "Copy stat failed: %", strerror(errno));
  }

  archive_entry_copy_stat(p->ae, &s);
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_xattr_clear(VALUE self) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  archive_entry_xattr_clear(p->ae);
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_xattr_add_entry(VALUE self, VALUE v_name, VALUE v_value) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  Check_Type(v_name, T_STRING);
  Check_Type(v_value, T_STRING);
  archive_entry_xattr_add_entry(p->ae, RSTRING_PTR(v_name), RSTRING_PTR(v_value), RSTRING_LEN(v_value));
  return Qnil;
}

/* */
static VALUE rb_libarchive_entry_xattr_count(VALUE self) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  return INT2NUM(archive_entry_xattr_count(p->ae));
}

/* */
static VALUE rb_libarchive_entry_xattr_reset(VALUE self) {
  struct rb_libarchive_entry_container *p;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);
  return INT2NUM(archive_entry_xattr_reset(p->ae));
}

/* */
static VALUE rb_libarchive_entry_xattr_next(VALUE self) {
  struct rb_libarchive_entry_container *p;
  const char *name;
  const void *value;
  size_t size;
  Data_Get_Struct(self, struct rb_libarchive_entry_container, p);
  Check_Entry(p);

  if (archive_entry_xattr_next(p->ae, &name, &value, &size) != ARCHIVE_OK) {
    return Qnil;
  } else {
    return rb_ary_new3(3, rb_str_new2(name), rb_str_new(value, size));
  }
}

void Init_libarchive_entry() {
  rb_cArchiveEntry = rb_define_class_under(rb_mArchive, "Entry", rb_cObject);
  rb_define_alloc_func(rb_cArchiveEntry, rb_libarchive_entry_alloc);
  rb_funcall(rb_cArchiveEntry, rb_intern("private_class_method"), 1, ID2SYM(rb_intern("new")));
  rb_define_method(rb_cArchiveEntry, "close", rb_libarchive_entry_close, 0);

  rb_define_method(rb_cArchiveEntry, "atime", rb_libarchive_entry_atime, 0);
  rb_define_method(rb_cArchiveEntry, "atime_nsec", rb_libarchive_entry_atime_nsec, 0);
  rb_define_method(rb_cArchiveEntry, "atime_is_set?", rb_libarchive_entry_atime_is_set, 0);
  rb_define_method(rb_cArchiveEntry, "birthtime", rb_libarchive_entry_birthtime, 0);
  rb_define_method(rb_cArchiveEntry, "birthtime_nsec", rb_libarchive_entry_birthtime_nsec, 0);
  rb_define_method(rb_cArchiveEntry, "birthtime_is_set?", rb_libarchive_entry_birthtime_is_set, 0);
  rb_define_method(rb_cArchiveEntry, "ctime", rb_libarchive_entry_ctime, 0);
  rb_define_method(rb_cArchiveEntry, "ctime_nsec", rb_libarchive_entry_ctime_nsec, 0);
  rb_define_method(rb_cArchiveEntry, "ctime_is_set?", rb_libarchive_entry_ctime_is_set, 0);
  rb_define_method(rb_cArchiveEntry, "dev", rb_libarchive_entry_dev, 0);
  rb_define_method(rb_cArchiveEntry, "devmajor", rb_libarchive_entry_devmajor, 0);
  rb_define_method(rb_cArchiveEntry, "devminor", rb_libarchive_entry_devminor, 0);
  rb_define_method(rb_cArchiveEntry, "filetype", rb_libarchive_entry_filetype, 0);
  rb_define_method(rb_cArchiveEntry, "directory?", rb_libarchive_entry_is_directory, 0);
  rb_define_method(rb_cArchiveEntry, "character_special?", rb_libarchive_entry_is_character_special, 0);
  rb_define_method(rb_cArchiveEntry, "block_special?", rb_libarchive_entry_is_block_special, 0);
  rb_define_method(rb_cArchiveEntry, "regular?", rb_libarchive_entry_is_regular, 0);
  rb_define_method(rb_cArchiveEntry, "symbolic_link?", rb_libarchive_entry_is_symbolic_link, 0);
  rb_define_method(rb_cArchiveEntry, "socket?", rb_libarchive_entry_is_socket, 0);
  rb_define_method(rb_cArchiveEntry, "fifo?", rb_libarchive_entry_is_fifo, 0);
  rb_define_method(rb_cArchiveEntry, "fflags", rb_libarchive_entry_fflags, 0);
  rb_define_method(rb_cArchiveEntry, "fflags_text", rb_libarchive_entry_fflags_text, 0);
  rb_define_method(rb_cArchiveEntry, "gid", rb_libarchive_entry_gid, 0);
  rb_define_method(rb_cArchiveEntry, "gname", rb_libarchive_entry_gname, 0);
  rb_define_method(rb_cArchiveEntry, "hardlink", rb_libarchive_entry_hardlink, 0);
  rb_define_method(rb_cArchiveEntry, "ino", rb_libarchive_entry_ino, 0);
  rb_define_method(rb_cArchiveEntry, "mode", rb_libarchive_entry_mode, 0);
  rb_define_method(rb_cArchiveEntry, "mtime", rb_libarchive_entry_mtime, 0);
  rb_define_method(rb_cArchiveEntry, "mtime_nsec", rb_libarchive_entry_mtime_nsec, 0);
  rb_define_method(rb_cArchiveEntry, "mtime_is_set?", rb_libarchive_entry_mtime_is_set, 0);
  rb_define_method(rb_cArchiveEntry, "nlink", rb_libarchive_entry_nlink, 0);
  rb_define_method(rb_cArchiveEntry, "pathname", rb_libarchive_entry_pathname, 0);
  rb_define_method(rb_cArchiveEntry, "rdev", rb_libarchive_entry_rdev, 0);
  rb_define_method(rb_cArchiveEntry, "rdevmajor", rb_libarchive_entry_rdevmajor, 0);
  rb_define_method(rb_cArchiveEntry, "rdevminor", rb_libarchive_entry_rdevminor, 0);
#if ARCHIVE_VERSION_NUMBER >= 2005003
  rb_define_method(rb_cArchiveEntry, "sourcepath", rb_libarchive_entry_sourcepath, 0);
#endif
  rb_define_method(rb_cArchiveEntry, "size", rb_libarchive_entry_size, 0);
  rb_define_method(rb_cArchiveEntry, "size_is_set?", rb_libarchive_entry_size_is_set, 0);
#if ARCHIVE_VERSION_NUMBER >= 2003002
  rb_define_method(rb_cArchiveEntry, "strmode", rb_libarchive_entry_strmode, 0);
#endif
  rb_define_method(rb_cArchiveEntry, "symlink", rb_libarchive_entry_symlink, 0);
  rb_define_method(rb_cArchiveEntry, "uid", rb_libarchive_entry_uid, 0);
  rb_define_method(rb_cArchiveEntry, "uname", rb_libarchive_entry_uname, 0);

  rb_define_method(rb_cArchiveEntry, "atime=", rb_libarchive_entry_set_atime, 1);
  rb_define_method(rb_cArchiveEntry, "set_atime", rb_libarchive_entry_set_atime2, 2);
  rb_define_method(rb_cArchiveEntry, "unset_atime", rb_libarchive_entry_unset_atime, 0);
  rb_define_method(rb_cArchiveEntry, "birthtimee=", rb_libarchive_entry_set_birthtime, 1);
  rb_define_method(rb_cArchiveEntry, "set_birthtime", rb_libarchive_entry_set_birthtime2, 2);
  rb_define_method(rb_cArchiveEntry, "unset_birthtime", rb_libarchive_entry_unset_birthtime, 0);
  rb_define_method(rb_cArchiveEntry, "ctime=", rb_libarchive_entry_set_ctime, 1);
  rb_define_method(rb_cArchiveEntry, "set_ctime", rb_libarchive_entry_set_ctime2, 2);
  rb_define_method(rb_cArchiveEntry, "unset_ctime", rb_libarchive_entry_unset_ctime, 0);
  rb_define_method(rb_cArchiveEntry, "dev=", rb_libarchive_entry_set_dev, 1);
  rb_define_method(rb_cArchiveEntry, "devmajor=", rb_libarchive_entry_set_devmajor, 1);
  rb_define_method(rb_cArchiveEntry, "devminor=", rb_libarchive_entry_set_devminor, 1);
  rb_define_method(rb_cArchiveEntry, "filetype=", rb_libarchive_entry_set_filetype, 1);
  rb_define_method(rb_cArchiveEntry, "set_fflags", rb_libarchive_entry_set_fflags, 2);
  rb_define_method(rb_cArchiveEntry, "copy_fflags_text", rb_libarchive_entry_copy_fflags_text, 1);
  rb_define_method(rb_cArchiveEntry, "gid=", rb_libarchive_entry_set_gid, 1);
  rb_define_method(rb_cArchiveEntry, "gname=", rb_libarchive_entry_set_gname, 1);
  rb_define_method(rb_cArchiveEntry, "copy_gname", rb_libarchive_entry_copy_gname, 1);
  rb_define_method(rb_cArchiveEntry, "hardlink=", rb_libarchive_entry_set_hardlink, 1);
  rb_define_method(rb_cArchiveEntry, "copy_hardlink", rb_libarchive_entry_copy_hardlink, 1);
  rb_define_method(rb_cArchiveEntry, "ino=", rb_libarchive_entry_set_ino, 1);
  rb_define_method(rb_cArchiveEntry, "link=", rb_libarchive_entry_set_link, 1);
  rb_define_method(rb_cArchiveEntry, "copy_link", rb_libarchive_entry_copy_link, 1);
  rb_define_method(rb_cArchiveEntry, "mode=", rb_libarchive_entry_set_mode, 1);
  rb_define_method(rb_cArchiveEntry, "mtime=", rb_libarchive_entry_set_mtime, 1);
  rb_define_method(rb_cArchiveEntry, "set_mtime", rb_libarchive_entry_set_mtime2, 2);
  rb_define_method(rb_cArchiveEntry, "unset_mtime", rb_libarchive_entry_unset_mtime, 0);
  rb_define_method(rb_cArchiveEntry, "nlink=", rb_libarchive_entry_set_nlink, 1);
  rb_define_method(rb_cArchiveEntry, "pathname=", rb_libarchive_entry_set_pathname, 1);
  rb_define_method(rb_cArchiveEntry, "copy_pathname", rb_libarchive_entry_copy_pathname, 1);
  rb_define_method(rb_cArchiveEntry, "perm=", rb_libarchive_entry_set_perm, 1);
  rb_define_method(rb_cArchiveEntry, "rdev=", rb_libarchive_entry_set_rdev, 1);
  rb_define_method(rb_cArchiveEntry, "rdevmajor=", rb_libarchive_entry_set_rdevmajor, 1);
  rb_define_method(rb_cArchiveEntry, "rdevminor=", rb_libarchive_entry_set_rdevminor, 1);
  rb_define_method(rb_cArchiveEntry, "size=", rb_libarchive_entry_set_size, 1);
  rb_define_method(rb_cArchiveEntry, "unset_size", rb_libarchive_entry_unset_size, 0);
#if ARCHIVE_VERSION_NUMBER >= 2005003
  rb_define_method(rb_cArchiveEntry, "copy_sourcepath", rb_libarchive_entry_copy_sourcepath, 1);
#endif
  rb_define_method(rb_cArchiveEntry, "symlink=", rb_libarchive_entry_set_symlink, 1);
  rb_define_method(rb_cArchiveEntry, "copy_symlink", rb_libarchive_entry_copy_symlink, 1);
  rb_define_method(rb_cArchiveEntry, "uid=", rb_libarchive_entry_set_uid, 1);
  rb_define_method(rb_cArchiveEntry, "uname=", rb_libarchive_entry_set_uname, 1);
  rb_define_method(rb_cArchiveEntry, "copy_uname", rb_libarchive_entry_copy_uname, 1);

  rb_define_method(rb_cArchiveEntry, "copy_stat", rb_libarchive_entry_copy_stat, 1);
  rb_define_method(rb_cArchiveEntry, "copy_lstat", rb_libarchive_entry_copy_lstat, 1);

  rb_define_method(rb_cArchiveEntry, "xattr_clear", rb_libarchive_entry_xattr_clear, 0);
  rb_define_method(rb_cArchiveEntry, "xattr_add_entry", rb_libarchive_entry_xattr_add_entry, 2);
  rb_define_method(rb_cArchiveEntry, "xattr_count", rb_libarchive_entry_xattr_count, 0);
  rb_define_method(rb_cArchiveEntry, "xattr_reset", rb_libarchive_entry_xattr_reset, 0);
  rb_define_method(rb_cArchiveEntry, "xattr_next", rb_libarchive_entry_xattr_next, 0);
}
