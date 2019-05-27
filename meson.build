project('warpkern-shader', 'cpp', default_options : ['cpp_std=c++14'])

gldep = dependency('gl')
glfwdep = dependency('glfw3')
fmtdep = subproject('fmt').get_variable('fmt_dep')
gladdep = subproject('glad').get_variable('glad_dep')

sources = [
  'main.cpp',
  'shader.cpp',
]

executable('warpkern-shader', sources, dependencies : [gldep, glfwdep, fmtdep, gladdep])
