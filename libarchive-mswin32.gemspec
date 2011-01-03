Gem::Specification.new do |spec|
  spec.name              = 'libarchive'
  spec.version           = '0.1.2'
  spec.platform          = 'mswin32'
  spec.summary           = 'Ruby bindings for Libarchive.'
  spec.description       = 'Ruby bindings for Libarchive. Libarchive is a programming library that can create and read several different streaming archive formats, including most popular tar variants, several cpio formats, and both BSD and GNU ar variants.'
  spec.require_paths     = %w(lib lib/i386-mswin32)
  spec.files             = %w(lib/libarchive.rb lib/libarchive_ruby.rb lib/i386-mswin32/rblibarchive.so README.txt libarchive.c COPYING.libarchive LICENSE.libbzip2)
  spec.author            = 'winebarrel'
  spec.email             = 'sgwr_dts@yahoo.co.jp'
  spec.homepage          = 'http://libarchive.rubyforge.org'
  spec.has_rdoc          = true
  spec.rdoc_options      << '--title' << 'Libarchive/Ruby - Ruby bindings for Libarchive.'
  spec.extra_rdoc_files  = %w(README.txt libarchive.c COPYING.libarchive LICENSE.libbzip2)
  spec.rubyforge_project = 'libarchive'
end
