project('Twitchbot', 'cpp')

cpp_compiler = meson.get_compiler('cpp')
add_project_arguments('--std=c++2a', '-c', '-fcoroutines', '-ggdb', language: 'cpp')
add_project_arguments('-Wall', '-Wextra', '-pedantic', '-Wshadow', '-Wpointer-arith', '-Wcast-align', '-Wwrite-strings', '-Wmissing-declarations', '-Wredundant-decls', '-Winline', '-Wno-long-long', '-Wconversion', language: 'cpp')
add_project_link_arguments('-lsockpp', language: 'cpp')
add_project_link_arguments('-otwitchbot', language: 'cpp')

subdir('src/')
