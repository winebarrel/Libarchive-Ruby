Gem::Specification.new do |spec|
  spec.name              = 'libarchive'
  spec.version           = '0.1.1'
  spec.platform          = 'mswin32'
  spec.summary           = 'Ruby bindings for Libarchive.'
  spec.require_paths     = %w(lib/i386-mswin32)
  spec.files             = %w(lib/i386-mswin32/libarchive_ruby.so README.txt libarchive.c COPYING.libarchive LICENSE.libbzip2)
  spec.author            = 'winebarrel'
  spec.email             = 'sgwr_dts@yahoo.co.jp'
  spec.homepage          = 'http://libarchive.rubyforge.org'
  spec.has_rdoc          = true
  spec.rdoc_options      << '--title' << 'Libarchive/Ruby - Ruby bindings for Libarchive.'
  spec.extra_rdoc_files  = %w(README.txt libarchive.c COPYING.libarchive LICENSE.libbzip2)
  spec.rubyforge_project = 'libarchive'
end
