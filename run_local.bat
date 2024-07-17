@echo off

REM Compile the C program
gcc app\*.c -o shell.exe

REM Check if compilation was successful
if %errorlevel% neq 0 (
    echo Compilation failed
    exit /b %errorlevel%
)

REM Run the compiled program
shell.exe %*

REM Store the exit code of the program
set last_error=%errorlevel%

REM Delete the executable
del shell.exe

REM Exit with the same code as the program
exit /b %last_error%