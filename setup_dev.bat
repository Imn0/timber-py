@echo off
set SCRIPT_DIR=%~dp0
cd /d "%SCRIPT_DIR%"

if not exist build mkdir build
cd build

cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
