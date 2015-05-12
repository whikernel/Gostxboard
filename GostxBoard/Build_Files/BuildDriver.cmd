::
:: Cmd de compilation du driver 
::

:: Usage : BuildDriver  [-build|-rebuild|-clean] [-release|-debug] [-x86|-x64] [output]

:: Desactivation du verbose
@echo off 

:: Récupération des arguments
set ARG_COMPIL=%~1
shift
set ARG_TYPE=%~1
shift
set ARG_ARCH=%~1
shift

:: On vérifie qu'il n'y a pas d'espaces dans le répertoire de destination
cd | find " " >NULL:
if %ERRORLEVEL% == 0 (
	echo BuildDriver.cmd: error: MS Build dne supporte pas les espaces dans les chemins de répertoires. >&2
	exit /B 1
)

:: On définit les options de build
set C_DEFINES=-D_WIN32 -DTC_WINDOWS_DRIVER
set WARNING_LEVEL=-W4
set LINKER_FLAG=-nologo
set TEST_SIGNING=0

:: Architectures CPU 
if "%ARG_ARCH%"=="-x64" (
	set BUILD_ARCH=x64 WNET
	set BUILD_ARCH_DIR=amd64
	set ARCH=x64
	set ARCH_SUFFIX=-x64
	set LINKER_FLAG=%LINKER_FLAG% -LTCG
) else (
	set BUILD_ARCH=WXP
	set BUILD_ARCH_DIR=i386
	set ARCH=x86
	ARCH_SUFFIX=
)

:: Type de build
if "%ARG_TYPE%"=="-debug" (
	set BUILD_TYPE=chk
	set C_DEFINES=%C_DEFINES% -DDEBUG -D_DEBUG
	set BUILD_ALT_DIR=_driver_DEBUG
	set COPY_DIR="..\Debug"
) else (
	set BUILD_TYPE=fre
	set BUILD_ALT_DIR=_driver_release
	set BUILD_COPY_DIR="..\Release"
)


:: WDK environment

pushd .
call %WINDDK_ROOT%\bin\setenv %WINDDK_ROOT% %BUILD_TYPE% %BUILD_ARCH% no_oacr || exit /B %errorlevel%
popd


:: Build 

pushd . 
:build_dirs
	:build_dirs

	if "%~1"=="" goto done
	cd /D "%~1" || exit /B %errorlevel%

	if "%ARG_CMD%"=="-clean" (
		rd /s /q obj%BUILD_ALT_DIR%\%BUILD_ARCH_DIR% 2>NUL:
		rd /q obj%BUILD_ALT_DIR% 2>NUL:
	) else (

		set USER_C_FLAGS=%C_FLAGS% %C_DISABLED_WARNINGS% -FAcs -Fa%~1\obj%BUILD_ALT_DIR%\%BUILD_ARCH_DIR%\
		set MSC_WARNING_LEVEL=%C_WARNING_LEVEL%
		set C_DEFINES=%C_DEFINES%
		set RCOPTIONS=/I %MFC_INC_PATH%
		set LIBRARIAN_FLAGS=%LINKER_FLAG%
		set LINKER_FLAGS=%LINKER_FLAG%
		set BUILD_ALT_DIR=%BUILD_ALT_DIR%

		build %BUILD_OPTS% -w -nmake /S -nmake /C 2>build_errors.log 1>&2
		
		if errorlevel 1 (
			type build_errors.log
			type build_errors_asm.log 2>NUL:
			exit /B 1
		)
		del /q build_errors.log build_errors_asm.log build%BUILD_ALT_DIR%.* 2>NUL:
	)

	shift
	
goto build_dirs
:done
popd


if "%ARG_CMD%"=="-clean" exit /B 0

md "%COPY_DIR%\Setup Files" >NUL: 2>NUL:
copy /B /Y obj%BUILD_ALT_DIR%\%BUILD_ARCH_DIR%\GostxBoard.sys "%COPY_DIR%\Setup Files\GostxBoard_%ARCH_SUFFIX%.sys" >NUL:

if errorlevel 1 (
	echo BuildDriver.cmd: error: Cannot copy target. >&2
	exit /B 1
)

exit /B 0
