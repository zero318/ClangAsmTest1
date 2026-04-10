@echo off
setlocal EnableDelayedExpansion

SET "out=..\..\out\build\x86-Clang-Release-Debug\"
SET "outD=..\..\out\build\x86-Clang-Debug\"

DEL /Q "%out%st00.ecl"
DEL /Q "%outD%st00.ecl"