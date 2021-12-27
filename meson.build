project(
  'mpd-httpd',
  'cpp',
  version : '0.0.1',
  default_options : [
    'warning_level=3',
    'cpp_std=c++11'
  ]
)

cmake = import('cmake')

cc = meson.get_compiler('cpp')

if build_machine.system() == 'darwin'
  libmpdclient_dep = cc.find_library('libmpdclient', dirs: ['/usr/lib', '/usr/local/lib'])
else 
  libmpdclient_dep = dependency('libmpdclient', version: '>= 2.16', fallback: ['libmpdclient', 'libmpdclient_dep'])
endif

crow_proj = cmake.subproject('crow')
crow_dep = crow_proj.dependency('Crow')

exe = executable(
  'mpd-httpd',
  'src/mpd.cpp',
  'src/main.cpp',
  dependencies: [
    crow_dep,
    libmpdclient_dep
  ],
  install : true
)

test('basic', exe)
