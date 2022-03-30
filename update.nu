def main [mode: string = 'debug'] {
    xmake f -m $mode --cc=clang-cl --cxx=clang-cl
    xmake project -k compile_commands
}
