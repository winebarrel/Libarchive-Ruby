require 'mkmf'

if system('/bin/sh configure') and have_header('config.h') and have_header('archive.h') and have_header('archive_entry.h')
  $libs << " -larchive"

  have_header('zlib.h')
  have_header('bzlib.h')

  create_makefile('rblibarchive')
end
