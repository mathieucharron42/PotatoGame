@echo off
call set_engine_path.bat
START "" "%editor_exe_path%" "%~dp0PotatoGame.uproject" 127.0.0.1 -game -log -windowed -ResX=800 -ResY=450