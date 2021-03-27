project('rtfw_project', 'cpp', default_options : ['cpp_std=c++17'])

rtfw_include = include_directories('include')
rtfw_src_include = include_directories('src/detail')

boostlib = dependency('boost', modules: ['program_options'])

rtfw_srcs=[
	'src/detail/core.cpp',
	'src/detail/list.cpp',
	'src/detail/util.cpp',
]
rtfwlib = library('rtfw', rtfw_srcs, include_directories: 'include', dependencies: boostlib)
subdir('samples')

subdir('test')
