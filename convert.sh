# convert all the Chinese encoding to UTF-8 for C and C++ source files
# You have to install enca
# https://github.com/nijel/enca
# and libenca
# https://packages.ubuntu.com/focal/libdevel/libenca-dev

find ./ -name "*.c" -o -name "*.cpp" -o -name "*.h" -type f | xargs enca -L chinese -x utf-8
