
gtest = dependency('gtest')
gtest_main = dependency('gtest_main')

tests = [
	{'name': 'dummy', 'srcs': ['dummy.cpp']},
	{'name': 'list', 'srcs': ['list.cpp']},
	{'name': 'util', 'srcs': ['util.cpp']},
	{'name': 'assertion', 'srcs': ['assertion.cpp']},
	{'name': 'config', 'srcs': ['config.cpp']},
	{'name': 'propertytree', 'srcs': ['propertytree.cpp']},
]


foreach t : tests
	e = executable(t['name'], t['srcs'], dependencies: [gtest, gtest_main], include_directories: [rtfw_include, rtfw_src_include], link_with: rtfwlib)
	test(t['name'], e)
endforeach

