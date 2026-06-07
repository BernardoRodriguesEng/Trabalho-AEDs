@echo off
echo Resetando o Banco de Dados...
echo Deletando arquivos binarios, indices e compressoes...

if exist steam.bin del steam.bin
if exist steam_index.bin del steam_index.bin
if exist steam_ordenado.bin del steam_ordenado.bin
if exist reviews.bin del reviews.bin
if exist review_index.bin del review_index.bin
if exist price_index.bin del price_index.bin
if exist users.bin del users.bin
if exist library.bin del library.bin
if exist steam.bin.lzw del steam.bin.lzw
if exist steam.bin.huffman del steam.bin.huffman

if exist temp rmdir /s /q temp

echo.
echo Banco de dados resetado!
echo Na proxima vez que voce rodar a aplicacao, ela re-importara os dados do steam.csv.
pause
