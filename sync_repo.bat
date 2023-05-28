@echo off

REM Change to the script's directory
cd /d "%~dp0"

REM Fetch the latest changes from the origin
git fetch origin

REM Merge the latest changes from the origin's main branch
git pull origin main

REM Add all files to the staging area
git add .

REM Get the commit message from the command-line argument
set COMMIT_MESSAGE=%1

REM If no commit message is provided, use a default commit message
if "%COMMIT_MESSAGE%"=="" (
  set COMMIT_MESSAGE=Sync code with origin
)

REM Commit the changes with the specified commit message
git commit -m "%COMMIT_MESSAGE%"

REM Push the changes to the main branch of the origin
git push origin main

REM Pause the script (optional, for viewing the output)
pause
