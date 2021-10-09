@echo off

echo cloning, updating, compiling and installing boost from source..

git submodule init --recursive
git submodule update --recursive
cd boost
bootstrap
.\b2 install --prefix=..\install
