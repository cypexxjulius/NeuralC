@echo off
setlocal enabledelayedexpansion

set CC =cl.exe
set OperatingDirectory=%~dp0

:: Setting settings Variables

set CompilerFlags=/W3 /Z7 /wd4201 /wd5045 /D "_CRT_SECURE_NO_WARNINGS" /c /MP /nologo

set NeuralOutputDir=!%~d0%cd%\NeuralEngine\bin\!

set NeuralLibPath=%OperatingDirectory%/NeuralEngine/lib

set NeuralIncludePath=-I%NeuralLibPath%/stb/ -I%NeuralLibPath%/cglm/include/ -I%NeuralLibPath%/glad/include/  -I%NeuralLibPath%/glfw/include/ -I%OperatingDirectory%/NeuralEngine/


call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64 >nul 2> nul
echo --- Environment Configured

IF NOT EXIST "NeuralEngine\lib\glfw\build\src\Release" (
    pushd "NeuralEngine/lib/glfw" 
    
        cmake .  -B build 
        cd build && msbuild ALL_BUILD.vcxproj -property:Platform=x64 -property:Configuration=Release

    popd

    echo --- glfw3.lib build
)

IF NOT EXIST "NeuralEngine/lib/cglm/build/Release/cglm.lib" (
    pushd "NeuralEngine/lib/cglm" 
        
        cmake . -DCGLM_STATIC=ON -B build 
        cd build && msbuild ALL_BUILD.vcxproj -property:Platform=x64 -property:Configuration=Release
    
    popd
    
    echo --- cglm.lib build
)

IF NOT EXIST "NeuralEngine/lib/glad/glad.obj" (
    pushd "NeuralEngine/lib/glad" 
        call cl /Foglad.obj !CompilerFlags! src/glad.c /Iinclude/ /c 

    popd
    echo --- glad.obj build
)

echo --- Building NeuralEngine.lib

:: If bin folder does not exist create it
if not EXIST "%NeuralOutputDir%" (
    mkdir %NeuralOutputDir%
)

:: Delete every leftover .obj file in the bin folder
pushd %NeuralOutputDir%
del /F /Q *.*
popd


pushd "NeuralEngine/src"

for /R %%f IN ("*.c") DO (
    set file=%%~dpnxf

    set outfile=%%~fsnxf
    set outfile=!outfile:.c=.obj!
    set outfile=!outfile:%cd%=!
    set outfile=!outfile:\=-!
    set outfile=!!NeuralOutputDir!!outfile!!

    set NeuralCompiledFiles=!!NeuralCompiledFiles! !outfile!!

    call cl /Fo!outfile! !CompilerFlags! !file! !NeuralIncludePath! 2> nul || (
    
        echo --- Failed to compile %%~fsnxf aborting..
        goto end
    )
)
popd

set VendorLibs=NeuralEngine/lib/glfw/build/src/Release/glfw3.lib NeuralEngine/lib/cglm/build/Release/cglm.lib NeuralEngine/lib/glad/glad.obj

call lib /nologo /out:NeuralEngine/bin/NeuralEngine.lib %VendorLibs% %NeuralCompiledFiles% 2> nul

set PlatformLibs=kernel32.lib user32.lib gdi32.lib shell32.lib

echo -- Linking

call cl /Z7 /FeSandbox/program.exe /MD -I NeuralEngine/src/ %NeuralIncludePath% Sandbox/main.c /link /debug:fastlink /nologo /NODEFAULTLIB:LIBCMT NeuralEngine/bin/NeuralEngine.lib %PlatformLibs% 2> nul && (
    echo --- Compiled Succesfully
    goto end
) || (
    echo --- Compilation failed
    goto end
)


:end