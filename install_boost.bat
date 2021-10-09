@echo off

echo cloning, updating, compiling and installing boost from source..

git submodule init 
git submodule update --recursive
cd boost
git submodule init
git submodule update --recursive
bootstrap
.\b2 install --prefix=..\install
