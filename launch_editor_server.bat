@echo off
call set_engine_path.bat
START "" "%editor_exe_path%" "%~dp0PotatoGame.uproject" ?listen -server -log