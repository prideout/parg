alias init="cmake -H. -Bbuild"
alias initdbg="cmake -DCMAKE_BUILD_TYPE=Debug -H. -Bbuild"
alias initjs="cmake -DEMSCRIPTEN=ON -H. -Bbuild"
alias build="cmake --build build"
alias clean="rm -rf build"
alias format="
    clang-format -i src/*.c demos/*.c include/par.h;
    uncrustify -l C -c uncrustify.cfg --no-backup demos/*.c src/*.c include/par.h"
