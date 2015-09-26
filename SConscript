# Type "scons --help" for a list of options.
# This is the SConscript, which is called by the SConstruct.

Import('*')

CORE_SRC = Glob('src/*.c') + Glob('src/vendor/*.c')
JS_SRC = []

env = Environment(
    LIBS=['m'],
    CPPPATH=['include', 'src/vendor'],
    SHLIBPREFIX='',
    LINKFLAGS='-fopenmp ',
    CFLAGS=' -fopenmp -g -O3 -Wall -std=c99 ')

if env['PLATFORM'] == 'darwin':
    env['LINKFLAGS'] = ''

if GetOption('javascript'):
    env['CC'] = 'emcc'
    env['CXX'] = 'emcc'
    env['CFLAGS'] = '-O3 -Wall -std=c99 '
    env['CXXFLAGS'] = '-O3 -Wall --bind -std=c++11 '
    env['LINKFLAGS'] = (
        "-O3 --memory-init-file 0 --bind " +
        "-s 'MODULARIZE=1' " +
        "-s 'EXPORT_NAME=\"CreatePar\"' " +
        "-s 'ALLOW_MEMORY_GROWTH=1' " +
        "-s 'NO_FILESYSTEM=1' ")
    par = env.Program('par.js', source=CORE_SRC + JS_SRC)
else:
    par = env.SharedLibrary('_par.so', source=CORE_SRC)
    env.ParseConfig('pkg-config --cflags --static --libs glfw3')

Alias('lib', par)

env = env.Clone(LIBS=['m', par])

demos, shaders = [], []
for demo in DEMOS:
    name = 'par_' + demo
    cpath = 'demos/' + demo + '.c'
    spath = 'demos/' + demo + '.glsl'
    env.Program(name, source=[cpath])
    shader = Command(demo + '.glsl', spath, Copy("$TARGET", "$SOURCE"))
    shaders.append(shader)
    demos.append(name)

Alias('demos', demos + shaders)
