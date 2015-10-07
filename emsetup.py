import fileinput, os

PREFIX = 'LLVM_ROOT = '

os.system('emcc')
inifile = os.path.expanduser('~/.emscripten')
for line in fileinput.input(inifile, inplace=True):
    if line.startswith(PREFIX):
        print PREFIX + "'/usr/local/opt/emscripten/libexec/llvm/bin'"
    else:
        print line,
