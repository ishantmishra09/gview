project(
  'gview',
  'c',
  version: '0.1.0',
  default_options: [
    'c_std=c11',
    'warning_level=3',
  ],
)

sdl2_dep = dependency('sdl2')

executable(
  'gview',
  sources: [
    'src/main.c',
  ],
  dependencies: [sdl2_dep],
  install: true,
)
