project('glad', 'c')

inc = include_directories('include')
lib = library('glad', ['src/glad.c'], include_directories: inc)

glad_dep = declare_dependency(include_directories: inc, link_with: lib)
