set UNISIM_SHIP=C:\data\QDev\UniSim2\ship
set QT_PLUGINS="C:\Qt\5.4\mingw491_32\plugins"

rem Clean target bin and set target bin variable
rd /Q /S bin
md bin
md bin\end-user
md bin\end-user\plugins
md bin\developer
md bin\developer\plugins
set UNISIM_BIN=%UNISIM_SHIP%\bin

rem Copy icon file
copy *.ico bin

rem Copy MS files
pushd \Windows\System32
copy atl.dll %UNISIM_BIN%\end-user
copy comctl32.dll %UNISIM_BIN%\end-user
copy mfc42u.dll %UNISIM_BIN%\end-user
copy msvcrt.dll %UNISIM_BIN%\end-user
copy oleaut32.dll %UNISIM_BIN%\end-user
popd

rem Copy exe and DLL files
copy ..\bin\*_r.exe bin\end-user
copy ..\bin\*_r.dll bin\end-user
copy ..\bin\plugins\*_r.dll bin\end-user\plugins

copy ..\bin\*_d.exe bin\developer
copy ..\bin\*_d.dll bin\developer
copy ..\bin\plugins\*_d.dll bin\developer\plugins

rem copy MinGW libraries
pushd %QT_ROOT%
copy icu*.dll %UNISIM_BIN%\end-user
copy libwinpthread-1.dll %UNISIM_BIN%\end-user
copy libgcc_s_dw2-1.dll %UNISIM_BIN%\end-user
copy libstd*.dll %UNISIM_BIN%\end-user

rem copy Qt libraries
copy qt5core.dll %UNISIM_BIN%\end-user
copy qt5gui.dll %UNISIM_BIN%\end-user
copy Qt5Network.dll %UNISIM_BIN%\end-user
copy Qt5PrintSupport.dll %UNISIM_BIN%\end-user
copy qt5test.dll %UNISIM_BIN%\end-user
copy Qt5Widgets.dll %UNISIM_BIN%\end-user
copy qt5xmlpatterns.dll %UNISIM_BIN%\end-user
popd

rem create Qt plugin folders
pushd %UNISIM_BIN%
md end-user\imageformats
md end-user\platforms
popd

rem copy Qt plugin folders
pushd %QT_PLUGINS%
copy imageformats %UNISIM_BIN%\end-user\imageformats
copy platforms %UNISIM_BIN%\end-user\platforms
del %UNISIM_BIN%\end-user\imageformats\*d.dll
del %UNISIM_BIN%\end-user\platforms\*d.dll
popd

rem Remove test modules
pushd %UNISIM_BIN%
del /Q end-user\test*.*
del /Q end-user\plugins\test*.*
popd

pause