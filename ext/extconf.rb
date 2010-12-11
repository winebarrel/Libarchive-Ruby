require 'mkmf'

if system('/bin/sh configure') and have_header('config.h') and have_header('archive.h') and have_header('archive_entry.h')
  $libs << " -larchive"
  create_makefile('libarchive_ruby')
end
