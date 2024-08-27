@echo off
chcp 65001 > nul


set TOTAL_COMMANDS=19
set /A COUNTER=%TOTAL_COMMANDS%

cd /d "%~dp0..\src\db"

set /A current=%TOTAL_COMMANDS%-%COUNTER%
echo Loading…  (%current%/%TOTAL_COMMANDS%)
set /A COUNTER-=1
javac -h . DBManager.java

rem Compile all .cpp files to object files

cls
set /A current=%TOTAL_COMMANDS%-%COUNTER%
echo Loading…  (%current%/%TOTAL_COMMANDS%)

set /A COUNTER-=1
g++ -std=c++17 -c -I"%JAVA_HOME%\include" -I"%JAVA_HOME%\include\win32" Create.cpp -o Create.o -m64

cls
set /A current=%TOTAL_COMMANDS%-%COUNTER%
echo Loading…  (%current%/%TOTAL_COMMANDS%)

set /A COUNTER-=1
g++ -std=c++17 -c -I"%JAVA_HOME%\include" -I"%JAVA_HOME%\include\win32" CustomFormat.cpp -o CustomFormat.o -m64

cls
set /A current=%TOTAL_COMMANDS%-%COUNTER%
echo Loading…  (%current%/%TOTAL_COMMANDS%)



set /A COUNTER-=1
g++ -std=c++17 -c -I"%JAVA_HOME%\include" -I"%JAVA_HOME%\include\win32" Database.cpp -o Database.o -m64

cls
set /A current=%TOTAL_COMMANDS%-%COUNTER%
echo Loading…  (%current%/%TOTAL_COMMANDS%)



set /A COUNTER-=1
g++ -std=c++17 -c -I"%JAVA_HOME%\include" -I"%JAVA_HOME%\include\win32" DBManager.cpp -o DBManager.o -m64

cls
set /A current=%TOTAL_COMMANDS%-%COUNTER%
echo Loading…  (%current%/%TOTAL_COMMANDS%)



set /A COUNTER-=1
g++ -std=c++17 -c -I"%JAVA_HOME%\include" -I"%JAVA_HOME%\include\win32" Delete.cpp -o Delete.o -m64

cls
set /A current=%TOTAL_COMMANDS%-%COUNTER%
echo Loading…  (%current%/%TOTAL_COMMANDS%)



set /A COUNTER-=1
g++ -std=c++17 -c -I"%JAVA_HOME%\include" -I"%JAVA_HOME%\include\win32" Drop.cpp -o Drop.o -m64

cls
set /A current=%TOTAL_COMMANDS%-%COUNTER%
echo Loading…  (%current%/%TOTAL_COMMANDS%)



set /A COUNTER-=1
g++ -std=c++17 -c -I"%JAVA_HOME%\include" -I"%JAVA_HOME%\include\win32" Equal.cpp -o Equal.o -m64

cls
set /A current=%TOTAL_COMMANDS%-%COUNTER%
echo Loading…  (%current%/%TOTAL_COMMANDS%)



set /A COUNTER-=1
g++ -std=c++17 -c -I"%JAVA_HOME%\include" -I"%JAVA_HOME%\include\win32" Filter.cpp -o Filter.o -m64

cls
set /A current=%TOTAL_COMMANDS%-%COUNTER%
echo Loading…  (%current%/%TOTAL_COMMANDS%)



set /A COUNTER-=1
g++ -std=c++17 -c -I"%JAVA_HOME%\include" -I"%JAVA_HOME%\include\win32" Insert.cpp -o Insert.o -m64

cls
set /A current=%TOTAL_COMMANDS%-%COUNTER%
echo Loading…  (%current%/%TOTAL_COMMANDS%)



set /A COUNTER-=1
g++ -std=c++17 -c -I"%JAVA_HOME%\include" -I"%JAVA_HOME%\include\win32" NotEqual.cpp -o NotEqual.o -m64

cls
set /A current=%TOTAL_COMMANDS%-%COUNTER%
echo Loading…  (%current%/%TOTAL_COMMANDS%)



set /A COUNTER-=1
g++ -std=c++17 -c -I"%JAVA_HOME%\include" -I"%JAVA_HOME%\include\win32" QueryEditor.cpp -o QueryEditor.o -m64

cls
set /A current=%TOTAL_COMMANDS%-%COUNTER%
echo Loading…  (%current%/%TOTAL_COMMANDS%)



set /A COUNTER-=1
g++ -std=c++17 -c -I"%JAVA_HOME%\include" -I"%JAVA_HOME%\include\win32" Record.cpp -o Record.o -m64

cls
set /A current=%TOTAL_COMMANDS%-%COUNTER%
echo Loading…  (%current%/%TOTAL_COMMANDS%)



set /A COUNTER-=1
g++ -std=c++17 -c -I"%JAVA_HOME%\include" -I"%JAVA_HOME%\include\win32" Select.cpp -o Select.o -m64

cls
set /A current=%TOTAL_COMMANDS%-%COUNTER%
echo Loading…  (%current%/%TOTAL_COMMANDS%)



set /A COUNTER-=1
g++ -std=c++17 -c -I"%JAVA_HOME%\include" -I"%JAVA_HOME%\include\win32" SQLFormat.cpp -o SQLFormat.o -m64

cls
set /A current=%TOTAL_COMMANDS%-%COUNTER%
echo Loading…  (%current%/%TOTAL_COMMANDS%)



set /A COUNTER-=1
g++ -std=c++17 -c -I"%JAVA_HOME%\include" -I"%JAVA_HOME%\include\win32" StringManipulator.cpp -o StringManipulator.o -m64

cls
set /A current=%TOTAL_COMMANDS%-%COUNTER%
echo Loading…  (%current%/%TOTAL_COMMANDS%)



set /A COUNTER-=1
g++ -std=c++17 -c -I"%JAVA_HOME%\include" -I"%JAVA_HOME%\include\win32" Table.cpp -o Table.o -m64

cls
set /A current=%TOTAL_COMMANDS%-%COUNTER%
echo Loading…  (%current%/%TOTAL_COMMANDS%)



set /A COUNTER-=1
g++ -std=c++17 -c -I"%JAVA_HOME%\include" -I"%JAVA_HOME%\include\win32" Update.cpp -o Update.o -m64

rem Link all object files into a single DLL

cls
set /A current=%TOTAL_COMMANDS%-%COUNTER%
echo Loading…  (%current%/%TOTAL_COMMANDS%)



set /A COUNTER-=1
g++ -shared -o native.dll -m64 Create.o CustomFormat.o Database.o DBManager.o Delete.o Drop.o Equal.o Filter.o Insert.o NotEqual.o QueryEditor.o Record.o Select.o SQLFormat.o StringManipulator.o Table.o Update.o

cls
set /A current=%TOTAL_COMMANDS%-%COUNTER%
echo Loading…  (%current%/%TOTAL_COMMANDS%)

rem Move the DLL to the specified directory
set /A COUNTER-=1
move native.dll "%~dp0..\dll\native.dll"


rem Delete object files

for %%f in (*.o) do del "%%f"

cls
echo Done!


