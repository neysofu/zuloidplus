project('tutorial', 'cpp', default_options : ['cpp_std=c++17'])

boost_dep = dependency('boost')
src = ['src/main.cpp', 'src/gamestate.cpp']
executable('zuloidplus', src, dependencies : boost_dep, include_directories : 'src')
