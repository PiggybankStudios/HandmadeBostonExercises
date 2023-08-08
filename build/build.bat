@echo off

set ProjectName=Handmade Boston Exercises
set ProjectNameSafe=HmBostonEx

set DebugBuild=0
set DeveloperBuild=1
set AssertionsEnabled=1
set RunProgram=1
set FuzzTesting=1

set LibFolder=..
set RunFolder=..
set SourceFolder=..\src
set MainSourceFilePath=%SourceFolder%\main.cpp
set LlvmInstallDir=C:\Program Files\LLVM
set OutputExeName=%ProjectNameSafe%.exe

set CompilerFlags=-DWINDOWS_COMPILATION -DFUZZ_TESTING=%FuzzTesting% -DPROJECT_NAME="\"%ProjectName%\"" -DPROJECT_NAME_SAFE=\"%ProjectNameSafe%\" -DDEBUG_BUILD=%DebugBuild% -DDEVELOPER_BUILD=%DeveloperBuild% -DDEMO_BUILD=0 -DSTEAM_BUILD=0 -DPROCMON_SUPPORTED=0 -DBOX2D_SUPPORTED=0 -DOPENGL_SUPPORTED=0 -DVULKAN_SUPPORTED=0 -DDIRECTX_SUPPORTED=0 -DSLUG_SUPPORTED=0 -DJSON_SUPPORTED=0 -DASSERTIONS_ENABLED=%AssertionsEnabled%
rem /FC = Full path for error messages
rem /EHsc = Exception Handling Model: Standard C++ Stack Unwinding. Functions declared as extern "C" can't throw exceptions
rem /EHa- = TODO: Do we really need this?? It seems like this option should be off if we specify s and c earlier
rem /nologo = Suppress the startup banner
rem /GS- = Buffer overrun protection is turned off
rem /Gm- = Minimal rebuild is enabled [deprecated]
rem /GR- = Run-Time Type Information is Disabled [_CPPRTTI macro doesn't work]
rem /Fm = Enables map file output [with default name, same as /MAP linker option]
rem /Oi = Enable intrinsic generation
rem /WX = Treat warnings as errors
rem /W4 = Warning level 4 [just below /Wall]
set CompilerFlags=%CompilerFlags% /FC /EHsc /EHa- /nologo /GS- /Gm- /GR- /Fm /Oi /WX /W4
rem /wd4130 = Logical operation on address of string constant [W4] TODO: Should we re-enable this one? Don't know any scenarios where I want to do this
rem /wd4201 = Nonstandard extension used: nameless struct/union [W4] TODO: Should we re-enable this restriction for ANSI compatibility?
rem /wd4324 = Structure was padded due to __declspec[align[]] [W4]
rem /wd4458 = Declaration of 'identifier' hides class member [W4]
rem /wd4505 = Unreferenced local function has been removed [W4]
rem /wd4996 = Usage of deprecated function, class member, variable, or typedef [W3]
rem /wd4127 = Conditional expression is constant [W4]
rem /wd4706 = assignment within conditional expression [W?]
set CompilerFlags=%CompilerFlags% /wd4130 /wd4201 /wd4324 /wd4458 /wd4505 /wd4996 /wd4127 /wd4706
set Libraries=gdi32.lib User32.lib Shell32.lib kernel32.lib winmm.lib Winhttp.lib Shlwapi.lib Ole32.lib
set LinkerFlags=-incremental:no
set IncludeDirectories=/I"%SourceFolder%" /I"%LibFolder%"
set LibraryDirectories=

if "%DebugBuild%"=="1" (
	rem /Od = Optimization level: Debug
	rem /Zi = Generate complete debugging information
	rem /MTd = Statically link the standard library [not as a DLL, Debug version]
	rem /wd4065 = Switch statement contains 'default' but no 'case' labels
	rem /wd4100 = Unreferenced formal parameter [W4]
	rem /wd4101 = Unreferenced local variable [W3]
	rem /wd4127 = Conditional expression is constant [W4]
	rem /wd4189 = Local variable is initialized but not referenced [W4]
	rem /wd4702 = Unreachable code [W4]
	set CompilerFlags=%CompilerFlags% /Od /Zi /MTd /wd4065 /wd4100 /wd4101 /wd4127 /wd4189 /wd4702
	rem Dbghelp.lib = ?
	set Libraries=%Libraries% Dbghelp.lib
) else (
	rem /Ot = Favors fast code over small code
	rem /Oy = Omit frame pointer [x86 only]
	rem /O2 = Optimization level 2: Creates fast code
	rem /MT = Statically link the standard library [not as a DLL]
	rem /Zi = Generate complete debugging information [optional]
	rem /fsanitize=address = Enable Address Sanitizer [optional]
	set CompilerFlags=%CompilerFlags% /Ot /Oy /O2 /MT /Zi
)

if "%FuzzTesting%"=="1" (
	set CompilerFlags=%CompilerFlags% /fsanitize=address /fsanitize=fuzzer
	set LibraryDirectories=%LibraryDirectories% /LIBPATH:"%LlvmInstallDir%\lib\clang\13.0.1\lib\windows"
	set OutputExeName=%ProjectNameSafe%_Fuzz.exe
)

rem call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64
rem call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" amd64 -no_logo
call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64

cl /Fe%OutputExeName% %CompilerFlags% %IncludeDirectories% "%MainSourceFilePath%" /link %LibraryDirectories% %LinkerFlags% %Libraries%

echo [Build Finished!]

if "%RunProgram%"=="1" (
	echo [Running Program...]
	pushd %RunFolder%
	build\%OutputExeName%
	popd
	echo [Program Finished!]
)
