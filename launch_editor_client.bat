@echo off
call set_engine_path.bat
START %unreal_engine_path%/Engine/Binaries/Win64/UnrealEditor.exe %~dp0PotatoGame.uproject 127.0.0.1 -game -log -windowed -ResX=800 -ResY=450