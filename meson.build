project('guiterm', 'cpp',
        default_options: [ 'cpp_std=c++23', 'warning_level=3' ],
        version: '1.0.0-alpha',
        license: 'GPL-3.0-or-later')

add_project_dependencies([
    dependency('sdl3'),
    dependency('sdl3-ttf'),
    dependency('jsoncpp'),
    dependency('libuv'),
    dependency('boost', modules: [ 'filesystem', 'system', 'process' ])
], language: 'cpp')

add_project_arguments([
    '-DAPP_NAME="@0@"'.format(meson.project_name()),
    '-DAPP_VERSION="@0@"'.format(meson.project_version()),
], language: 'cpp')

if get_option('buildtype').contains('debug')
    add_project_arguments([ '-D__DEBUG' ], language: 'cpp')
endif

source_files = files(
    'src/sdl/window.cc',

    'src/arg_parser.cc',
    'src/config.cc',
    'src/log.cc',
    'src/main.cc',
    'src/process.cc',
    'src/sdl.cc',
    'src/utils.cc',
)

executable('guiterm', source_files,
           include_directories: include_directories('include'))