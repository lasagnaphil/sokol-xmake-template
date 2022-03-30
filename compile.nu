def main [mode: string = 'debug'] {
    xmake f -m $mode --cc=clang-cl --cxx=clang-cl
    xmake project -k compile_commands
    do -i { xmake }
    if $env.LAST_EXIT_CODE != -1 {
        xmake run renderer_test
    }
}
