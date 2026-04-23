@echo off
echo Compilando o projeto...
g++ main.cpp -o a.exe -lws2_32
if %errorlevel% neq 0 (
    echo A compilação falhou!
    exit /b %errorlevel%
)
echo Compilação bem-sucedida! Você já pode executar o a.exe