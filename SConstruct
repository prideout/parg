# Type "scons --help" for a list of options.
# This is the SConstruct, which calls into the SConscript.

import os.path
import re

DEMOS = [re.split('\W+', str(s))[-2] for s in Glob('demos/*.c')]
DEFAULT_DEMO = 'sierpinski'

DEMO_HELP = '\n'.join(
    '- "scons {0}" to build and run the {0} demo.'.format(x) for x in DEMOS)

Help('''
Hello, friend!  You can type...

- "scons" to build and run the {0} demo.
- "scons demos" to build every demo.
- "scons lib" to build only the core library.
- "scons lib --javascript" to build the core library using emcc.
- "scons format" to autoformat the source code.
- "scons -c" to clean.
- "rm -rf build" to clobber.
{1}
'''.format(DEFAULT_DEMO, DEMO_HELP))

BUILD_DIR = 'build'

AddOption('--javascript',
          dest='javascript',
          action='store_true',
          help='build with emcc')

# Targets that build code.

Export('DEMOS')
SConscript('SConscript', variant_dir=BUILD_DIR, src_dir='.', duplicate=0)

# Targets that run code.

for demo in DEMOS:
    filename = 'par_' + demo
    binpath = os.path.join(BUILD_DIR, filename)
    spath = os.path.join(BUILD_DIR, demo + '.glsl')
    Depends(demo, spath)
    Command(demo, binpath, binpath)
    AlwaysBuild(demo)
    if demo == DEFAULT_DEMO:
        Default(demo)

# Targets that format code.

additions = ['include/par.h'] + Glob('demos/*.c')
exclusions = []
cfiles = Glob('src/*.c') + Glob('src/*.h') + additions
cfiles = list(set(cfiles) - set(exclusions))
Command('format', cfiles, 'clang-format -i $SOURCES && ' +
        'uncrustify -l C -c uncrustify.cfg --no-backup $SOURCES')
AlwaysBuild('format')
