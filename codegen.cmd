setlocal
set arch=%1%
if "%arch%" == "" (
  echo.
  echo Error: Arch is not set.

)
if "%STAFF_HOME%" == "" (
  echo.
  echo Error: STAFF_HOME is not set.

)
set PATH=%STAFF_HOME%\lib;%STAFF_HOME%\bin;%PATH%
%STAFF_HOME%\bin\staff_codegen -u -tcomponent -csrc wsOCR.h
%STAFF_HOME%\bin\staff_codegen -u -twsdl -csrc wsOCR.h
endlocal
