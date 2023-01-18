#!/bin/sh

set -e
git_path=$(git rev-parse --show-toplevel 2>/dev/null)
xopt_path=$(echo $git_path/.xopt)
pwd_dir=$(pwd)
echo $git_path
if ! test -f $git_path/contrib/logger/logger.h; then
  git submodule update --init --recursive
  echo git submodule update --init --recursive .. done
else
  echo git submodule update --init --recursive .. clean
fi

if test -d $xopt_path/include; then
  echo make contrib/* .. clean
  exit 0
fi
if ! test -f $git_path/.xopt/include/logger.h; then
  cd $git_path && cd ./contrib/logger &&                            \
    ./autogen.sh &&                                                 \
    ./configure --prefix=$(echo $git_path/.xopt) &&                 \
    make && make install &&                                         \
    mkdir -p $git_path/.xopt/include &&                             \
    cp $git_path/contrib/logger/logger.h $git_path/.xopt/include && \
    cd $pwd_dir &&                                                  \
    pwd

  #  patch -p0 < patches/logger.patches

else
  echo make contrib/logger .. clean
fi


