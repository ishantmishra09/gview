project(
  'gview',
  'c',
  version: '0.1.0',
  default_options: [
    'c_std=c11',
    'warning_level=3',
  ],
)

cc = meson.get_compiler('c')

sdl2_dep = dependency('sdl2')
nfd_dep = dependency('nfd')
m_dep = cc.find_library('m', required: false)

executable(
  'gview',
  sources: ['src/main.c', 'src/image.c', 'src/viewer.c', 'src/nfd_g.c'],
  include_directories: include_directories('vendor'),
  dependencies: [sdl2_dep, m_dep, nfd_dep],
  install: true,
)
