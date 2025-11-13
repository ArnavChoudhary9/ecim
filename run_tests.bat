@echo off
REM Build and run tests for ecim project

echo ================================
echo Building ECIM Test Suite
echo ================================

cd /d "%~dp0"

REM Build the test project using the test Makefile
echo.
echo Building tests...
cd build
make ecim_tests
if errorlevel 1 (
    echo Error: Build failed
    cd ..
    exit /b 1
)
cd ..

REM Run the tests
echo.
echo ================================
echo Running Tests
echo ================================
echo.
bin\Debug\ecim_tests.exe
set TEST_RESULT=%errorlevel%

echo.
if %TEST_RESULT% equ 0 (
    echo ================================
    echo All tests passed!
    echo ================================
) else (
    echo ================================
    echo Some tests failed!
    echo ================================
)

exit /b %TEST_RESULT%
