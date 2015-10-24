alias init="cmake -H. -Bbuild"
alias initjs="cmake -DEMSCRIPTEN=ON -H. -Bbuild"
alias build="cmake --build build"
alias clean="rm -rf build"
alias format="
    clang-format -i src/*.c demos/*.c;
    uncrustify -l C -c uncrustify.cfg --no-backup demos/*.c src/*.c"
