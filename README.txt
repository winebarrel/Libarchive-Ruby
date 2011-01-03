= Libarchive/Ruby

Copyright (c) 2009 SUGAWARA Genki <sgwr_dts@yahoo.co.jp>

== Description

Ruby bindings for Libarchive.

Libarchive is a programming library that can create and read several different streaming archive formats, including most popular tar variants, several cpio formats, and both BSD and GNU ar variants.

== Support archive (from Libarchive 2.6.0)
=== reading
* gzip compression
* bzip2 compression
* compress/LZW compression
* GNU tar format (including GNU long filenames, long link names, and
  sparse files)
* Solaris 9 extended tar format (including ACLs)
* Old V7 tar archives
* POSIX ustar
* POSIX pax interchange format
* POSIX octet-oriented cpio
* SVR4 ASCII cpio
* Binary cpio (big-endian or little-endian)
* ISO9660 CD-ROM images (with optional Rockridge extensions)
* ZIP archives (with uncompressed or "deflate" compressed entries)
* GNU and BSD 'ar' archives
* 'mtree' format
  * lzma compression not supported

=== writing
* gzip compression
* bzip2 compression
* compress/LZW compression
* POSIX ustar
* POSIX pax interchange format
* "restricted" pax format, which will create ustar archives except for entries that require pax extensions (for long filenames, etc).
  * ACLs not supported
* POSIX octet-oriented cpio
* SVR4 "newc" cpio
* shar archives
* GNU and BSD 'ar' archives

== Project Page

http://rubyforge.org/projects/libarchive

== Source Code

https://bitbucket.org/winebarrel/libarchive-ruby

== Dependency

Libarchive/Ruby depend on Libarchive.

Please install Libarchive. (version 2.6.0 or more is recommended)

== Install

gem install libarchive

== Example
=== reading archive
    require 'libarchive'
    
    Archive.read_open_filename('foo.tar.gz') do |ar|
      while entry = ar.next_header
        name = entry.pathname
        data = ar.read_data
    
        #data = ""
        #ar.read_data(1024) do |x|
        #  data << x
        #end
    
        puts "#{name} (size=#{data.size})"
      end
    end

=== creating archive
    require 'libarchive'
    
    Archive.write_open_filename('foo.tar.bz2', Archive::COMPRESSION_BZIP2, Archive::FORMAT_TAR) do |ar|
      Dir.glob('*.c').each do |fn|
        ar.new_entry do |entry|
          entry.copy_stat(fn)
          entry.pathname = fn
          ar.write_header(entry)
          ar.write_data(open(fn) {|f| f.read })
    
          #open(fn) do |f|
          #  ar.write_data do
          #    f.read(1024)
          #  end
          #end
        end
      end
    end

=== creating archive by extarnal program
    require 'libarchive'
    
    Archive.write_open_filename('foo.tar.lzo', 'lzop', Archive::FORMAT_TAR_USTAR) do |ar|
      ...
    end

== License
    Copyright (c) 2009 SUGAWARA Genki <sgwr_dts@yahoo.co.jp>
    All rights reserved.
    
    Redistribution and use in source and binary forms, with or without modification,
    are permitted provided that the following conditions are met:
    
        * Redistributions of source code must retain the above copyright notice, 
          this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright notice, 
          this list of conditions and the following disclaimer in the documentation 
          and/or other materials provided with the distribution.
        * The names of its contributors may be used to endorse or promote products 
           derived from this software without specific prior written permission.
    
    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
    ANY EXPRESS OR IMPLIED WARRANTIES,
    INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
    FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
    OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
    EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
    OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
    STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY 
    OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH 
    DAMAGE.

=== Static link library
Libarchive/Ruby(mswin32) contains Libarchive(2.6.0) and libbzip2(1.0.5).

* Libarchive
  * http://code.google.com/p/libarchive/
  * see {COPYING.libarchive}[link:files/COPYING_libarchive.html]

* libbzip2
  * http://www.bzip.org/
  * see {LICENSE.libbzip2}[link:files/LICENSE_libbzip2.html]
