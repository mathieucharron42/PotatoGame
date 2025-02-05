@echo off
call set_cooked_path.bat
START %cooked_potato_exe_path% 127.0.0.1 -log -windowed -ResX=800 -ResY=450