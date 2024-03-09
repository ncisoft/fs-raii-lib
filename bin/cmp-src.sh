#!/bin/bash

if test   $# -lt 2 ; then
  echo "Usage: $(basename $0) target src-dir ..."
  exit 0
fi

target="$1"
shift

fg_bright_yellow='[33m[1m'
fg_reset='[0m'

echo_hl()
{
  echo -e ${fg_bright_yellow}$@ ${fg_reset} >&2
}


new_file_list=""
CC=gcc
#VERBOSE="VERBOSE=1"

run()
{
  true &&                                             \
    echo_hl "found newer files: [$new_file_list]" &&  \
    mkdir -p build &&                                 \
    cd build && CC=$CC cmake .. && CC=$CC cmake .. && \
    cd .. &&                                          \
    make -C ./build $VERBOSE -j$(nproc)
  exit 0
}

LF='\n'
! test -f $target && run && exit 0
while [ $# -gt 0 ]; do
  dir="$1"
  shift
  for file in $(find "$dir" -name "*.[hc]" -o -name 'CMakeLists.txt')
  do
    test "$file" -nt "$target" && new_file_list+=" $file"
  done
done


if test -n "$new_file_list"; then
  echo "$new_file_list"
  echo  -e "${fg_bright_yellow}found newer files:\n["
  for new_file in $new_file_list; do
    echo  "  $new_file"
  done
  echo -e  "\n]${fg_reset}"

  touch $target &&                                    \
    mkdir -p build &&                                 \
    cd build && CC=$CC cmake .. && CC=$CC cmake .. && \
    cd .. && make -C ./build -j$(nproc) $VERBOSE &&   \
    exit 0

fi

echo_hl "every thing is clean"
