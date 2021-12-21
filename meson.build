project(
  'mpd-httpd',
  'c',
  version : '0.0.1',
  default_options : [
    'warning_level=3',
    'libmpdclient:default_library=static'
  ]
)

cc = meson.get_compiler('c')

if build_machine.system() == 'darwin'
  libmpdclient_dep = cc.find_library('libmpdclient', dirs: ['/usr/lib', '/usr/local/lib'])
else 
  libmpdclient_dep = dependency('libmpdclient', version: '>= 2.16', fallback: ['libmpdclient', 'libmpdclient_dep'])
endif

facil_dep = subproject('facil').get_variable('facil_dep')
cjson_dep = subproject('cjson').get_variable('libcjson_dep')

exe = executable(
  'mpd-httpd',
  'src/mpd.c',
  'src/main.c',
  dependencies: [
    facil_dep,
    cjson_dep,
    libmpdclient_dep
  ],
  install : true
)

test('basic', exe)
