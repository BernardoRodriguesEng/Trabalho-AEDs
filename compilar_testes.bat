@echo off
echo Compilando os testes...

g++ -std=c++17 ^
tests/TestesMain.cpp ^
tests/TestesJogo.cpp ^
tests/TestesCompressao.cpp ^
tests/TestesHash.cpp ^
tests/TestesArvoreBMais.cpp ^
tests/TestesOrdenacao.cpp ^
tests/TestesGameDAO.cpp ^
tests/TestesServidor.cpp ^
tests/TestesCriptografia.cpp ^
src/Game.cpp ^
src/CSVConverter.cpp ^
src/GameDAO.cpp ^
src/GameController.cpp ^
src/User.cpp ^
src/Review.cpp ^
src/ReviewDAO.cpp ^
src/Hash.cpp ^
src/HashExtensivel.cpp ^
src/UserDAO.cpp ^
src/LibraryDAO.cpp ^
src/BPlusTree.cpp ^
src/OrdenacaoExterna.cpp ^
src/Compressao/GerenciadorArquivo.cpp ^
src/Compressao/ArranjosDinamicos.cpp ^
src/Compressao/TrieLZW.cpp ^
src/Compressao/LZW.cpp ^
src/Compressao/Huffman.cpp ^
src/Compressao/Saida.cpp ^
src/Criptografia/XOR.cpp ^
-o run_tests.exe -lws2_32

if %errorlevel% neq 0 (
    echo A compilacao dos testes falhou!
    exit /b %errorlevel%
)

echo Compilacao bem-sucedida! Executando bateria de testes...
echo.
.\run_tests.exe
