@echo off
rem CoA Custom: run with the repack's own Python (the package is extracted inside the repack folder).
setlocal
set PY="%~dp0..\Runtime\python\python.exe"
if not exist %PY% set PY=python
%PY% -B "%~dp0install.py" %*
echo.
pause
