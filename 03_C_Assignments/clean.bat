@echo off
setlocal enabledelayedexpansion

for /r %%G in (*.obj *.exe) do (
    echo Deleting: "%%G"
    del "%%G"
)
echo Clean completed.
pause