@echo off
set PATH=%PSPSDK%\bin;%PATH%

if not exist ..\build (
    mkdir ..\build
)

pushd ..\src
make
move /y mschange.prx ..\build\
make clean
popd
