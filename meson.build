project('rtfw_project', 'cpp', default_options : ['cpp_std=c++17'])

rtfw_include = include_directories('include')
rtfwlib = library('rtfw', ['src/detail/core.cpp', 'src/detail/list.cpp'], include_directories: 'include')
subdir('samples')

subdir('test')
