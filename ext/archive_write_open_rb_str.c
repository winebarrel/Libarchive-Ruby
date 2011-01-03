#include <archive.h>
#include <ruby.h>

#ifdef _WIN32
typedef long ssize_t;
#endif

static int rb_str_write_open(struct archive *a, void *client_data) {
  if (archive_write_get_bytes_in_last_block(a) == -1) { 
    archive_write_set_bytes_in_last_block(a, 1);
  }

  return ARCHIVE_OK;
}

static int rb_str_write_close(struct archive *a, void *client_data) {
  return ARCHIVE_OK;
}


static ssize_t rb_str_write(struct archive *a, void *client_data, const void *buff, size_t length) {
  VALUE str = (VALUE) client_data;
  rb_str_cat(str, buff, length);
  return length;
}

int archive_write_open_rb_str(struct archive *a, VALUE str) {
  return archive_write_open(a, (void *) str, rb_str_write_open, rb_str_write, rb_str_write_close);
}
