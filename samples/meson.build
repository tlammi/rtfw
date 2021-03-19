executable('simple', 'simple/mymod.cpp', 'main.cpp', include_directories: '../include', link_with: rtfwlib)
executable('dependency', 'dependency/mod1.cpp','dependency/mod2.cpp', 'main.cpp', include_directories: '../include', link_with: rtfwlib)

executable('circular_dependency', 'circular_dependency/mod.cpp', 'main.cpp', include_directories: '../include', link_with: rtfwlib)
