#!/bin/sh
set -e

mkdir -p "$HOME/deps"

if [ ! -d "$HOME/deps/boost/include/boost" ]; then
    wget -O boost.tar.bz2 "https://sourceforge.net/projects/boost/files/boost/1.56.0/boost_1_56_0.tar.bz2/download"
    tar -xf boost.tar.bz2
    mkdir -p "$HOME/deps/boost/include"
    mv boost_*/boost "$HOME/deps/boost/include"
    rm -rf boost.tar.bz2 boost_*
fi
