project(
  'gview',
  'c',
  version: '0.1.0',
  default_options: [
    'c_std=c11',
    'warning_level=3',
  ],
)

gtk_dep = dependency('gtk4')

executable(
  'gview',
  sources: [
    'src/main.c',
  ],
  dependencies: [gtk_dep],
  install: true,
)
