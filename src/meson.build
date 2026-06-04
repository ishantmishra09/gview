gview_sources = files(
  'app.c',
  'main.c',
  'window.c',
)

gview_include = include_directories('../include')

executable(
  'gview',
  gview_sources,
  include_directories: gview_include,
  dependencies: [gtk4_dep],
  install: true,
)
