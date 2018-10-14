@echo off
set ConfigurationName=Release
set OutDir=..\bin\Release\
set SolutionDir=..\

echo xcopy "..\externals\OgreSDK_vc10_v1-7-2\Bin\%ConfigurationName%\*.dll" "%OutDir%" /s/e/d/i/y
xcopy "..\externals\OgreSDK_vc10_v1-7-2\Bin\%ConfigurationName%\*.dll" "%OutDir%" /s/e/d/i/y

echo xcopy "%SolutionDir%media" "%OutDir%media\" /s/e/d/i/y
xcopy "%SolutionDir%media" "%OutDir%media\" /s/e/d/i/y

echo xcopy "%SolutionDir%configuration" "%OutDir%configuration\" /s/e/d/i/y
xcopy "%SolutionDir%configuration" "%OutDir%configuration\" /s/e/d/i/y

echo xcopy "%SolutionDir%lib\%ConfigurationName%\*.dll" "%OutDir%" /s/e/d/i/y
xcopy "%SolutionDir%lib\%ConfigurationName%\*.dll" "%OutDir%" /s/e/d/i/y

echo if not exist "%OutDir%logs" mkdir "%OutDir%logs"
if not exist "%OutDir%logs" mkdir "%OutDir%logs"

echo if not exist "%OutDir%screenshots" mkdir "%OutDir%screenshots"
if not exist "%OutDir%screenshots" mkdir "%OutDir%screenshots"

pause