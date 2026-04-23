@echo off
echo Compilando o projeto...
g++ main.cpp -o a.exe -lws2_32
if %errorlevel% neq 0 (
    echo A compilacao falhou!
    exit /b %errorlevel%
)
echo Compilacao bem-sucedida! Voce ja pode executar o a.exe