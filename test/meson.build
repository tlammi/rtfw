
gtest = dependency('gtest')
gtest_main = dependency('gtest_main')

tests = [
	{'name': 'dummy', 'srcs': ['dummy.cpp']},
	{'name': 'list', 'srcs': ['list.cpp']},
]


foreach t : tests
	e = executable(t['name'], t['srcs'], dependencies: [gtest, gtest_main], include_directories: rtfw_include)
	test(t['name'], e)
endforeach

