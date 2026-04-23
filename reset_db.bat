@echo off
echo Resetando o Banco de Dados...
echo Deletando arquivos binários e índices...

if exist steam.bin del steam.bin
if exist steam_index.bin del steam_index.bin
if exist steam_ordenado.bin del steam_ordenado.bin
if exist reviews.bin del reviews.bin
if exist review_index.bin del review_index.bin

if exist temp rmdir /s /q temp

echo.
echo Banco de dados resetado com sucesso!
echo Na próxima vez que você rodar a aplicação, ela re-importará os dados do steam.csv.
pause
