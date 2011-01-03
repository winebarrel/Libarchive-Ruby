#!/bin/sh
VERSION=0.1.2

rm *.gem *.tar.bz2
rm -rf doc
cat ext/libarchive*.c > libarchive.c
rdoc -w 4 -SHN -m  README.txt README.txt libarchive.c COPYING.libarchive LICENSE.libbzip2 --title 'Libarchive/Ruby - Ruby bindings for Libarchive.'
tar jcvf libarchive-${VERSION}.tar.bz2 --exclude=.svn README.txt *.gemspec ext doc COPYING.libarchive LICENSE.libbzip2
gem build libarchive.gemspec
gem build libarchive-mswin32.gemspec
cp libarchive-${VERSION}-x86-mswin32.gem libarchive-${VERSION}-mswin32.gem
rm libarchive.c
