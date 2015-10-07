# Type "scons --help" for a list of options.
# This is the SConscript, which is called by the SConstruct.

Import('*')

CORE_SRC = Glob('src/*.c') + Glob('src/vendor/*.c')
JS_EXCLUSIONS = Glob('src/vendor/whereami.c') + Glob('src/vendor/kopen.c')
JS_SRC = list(set(CORE_SRC) - set(JS_EXCLUSIONS))

env = Environment(
    LIBS=['m'],
    CPPPATH=['include', 'src/vendor'],
    SHLIBPREFIX='',
    LINKFLAGS='-fopenmp ',
    CFLAGS=' -fopenmp -g -O3 -Wall -std=c99 ')

if env['PLATFORM'] == 'darwin':
    env['LINKFLAGS'] = ''

if GetOption('javascript'):
    env = env.Clone(
        CC = 'emcc',
        CXX = 'emcc',
        CFLAGS = '-O3 -Wall -std=c99 ',
        CXXFLAGS = '-O3 -Wall --bind -std=c++11 ',
        LINKFLAGS = (
            "-O3 --memory-init-file 0 --bind " +
            "-s 'MODULARIZE=1' " +
            "-s 'EXPORT_NAME=\"CreatePar\"' " +
            "-s 'ALLOW_MEMORY_GROWTH=1' " +
            "-s 'NO_FILESYSTEM=1' "))
else:
    par = env.SharedLibrary('_par.so', source=CORE_SRC)
    env.ParseConfig('pkg-config --cflags --static --libs glfw3')
    env = env.Clone(LIBS=['m', par])
    Alias('lib', par)

demos, shaders = [], []
for demo in DEMOS:
    cpath = 'demos/' + demo + '.c'
    spath = 'demos/' + demo + '.glsl'
    if GetOption('javascript'):
        name = demo + '.js'
        env.Program(name, source=[cpath] + JS_SRC)
    else:
        name = 'par_' + demo
        env.Program(name, source=[cpath])
    shader = Command(demo + '.glsl', spath, Copy("$TARGET", "$SOURCE"))
    shaders.append(shader)
    demos.append(name)

Alias('demos', demos + shaders)
