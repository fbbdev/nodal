#!/bin/sh
set -e

mkdir -p "$HOME/deps"

if [ ! -d "$HOME/deps/cmake/bin" ]; then
  wget --no-check-certificate -O cmake.tar.gz "https://cmake.org/files/v3.4/cmake-3.4.0-Linux-x86_64.tar.gz"
  tar -xf cmake.tar.gz
  mv cmake-* "$HOME/deps/cmake"
  rm -rf cmake.tar.gz cmake-*
fi

if [ ! -d "$HOME/deps/boost/include/boost" ]; then
  wget -O boost.tar.bz2 "http://downloads.sourceforge.net/project/boost/boost/1.59.0/boost_1_59_0.tar.bz2?r=http%3A%2F%2Fwww.boost.org%2Fusers%2Fhistory%2Fversion_1_59_0.html&ts=1447581105&use_mirror=skylink"
  tar -xf boost.tar.bz2
  mkdir -p "$HOME/deps/boost/include"
  mv boost_*/boost "$HOME/deps/boost/include"
  rm -rf boost.tar.bz2 boost_*
fi
