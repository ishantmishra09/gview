project(
  'gview',
  'c',
  version: '0.1.0',
  default_options: [
    'warning_level=3',
    'c_std=c17'
  ]
)

gtk = dependency('gtk4')

sources = [
  'src/main.c'
]

executable(
  'gview',
  sources,
  dependencies: gtk,
  install: true
)
