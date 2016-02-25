alias init="cmake . -Bbuild"
alias initdbg="cmake -DCMAKE_BUILD_TYPE=Debug . -Bbuild"
alias initjs="cmake -DEMSCRIPTEN=ON . -Bbuild"
alias build="cmake --build build"
alias clean="rm -rf build"
alias format="
    clang-format -i src/*.c demos/*.c include/parg.h;
    uncrustify -l C -c uncrustify.cfg --no-backup demos/*.c src/*.c include/parg.h"
