@echo off

setlocal enabledelayedexpansion

set "errorFlag=0"

for /r %%G in (*.c *.cpp) do (
    pushd "%%~dpG"
    echo Deleting Previously Generated Files...
    del "%%~dpnG.obj" 2>nul
    del "%%~dpnG.exe" 2>nul

    echo Compiling : "%%~nxG"
    cl.exe "%%~nxG"

    if exist "%%~dpnG.exe" (
        echo Running: "%%~dpnG.exe"
        "%%~dpnG.exe"
    )

    if not exist "%%~dpnG.exe" (
        echo Error compiling: "%%~nxG"
        set "errorFlag=1"
    )
    popd
)

if %errorFlag% equ 1 (
    echo Compilation completed with errors.
) else (
    echo All C files compiled successfully.
)
pause