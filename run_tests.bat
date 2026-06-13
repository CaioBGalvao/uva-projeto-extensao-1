@echo off
setlocal enabledelayedexpansion

echo Compilando testes...
gcc -I. tests\test_cliente.c src\persistencia.c -o tests\bin_test_cliente.exe
if %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%

gcc -I. tests\test_produto.c src\persistencia.c -o tests\bin_test_produto.exe
if %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%

gcc -I. tests\test_pedido.c src\persistencia.c -o tests\bin_test_pedido.exe
if %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%

gcc -I. tests\test_devolucao.c src\persistencia.c -o tests\bin_test_devolucao.exe
if %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%

gcc -I. tests\test_relatorio.c src\relatorio.c src\persistencia.c -o tests\bin_test_relatorio.exe
if %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%

gcc -I. tests\test_edge_cases.c tests\test_factory.c src\persistencia.c -o tests\bin_test_edge_cases.exe
if %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%

gcc -I. tests\test_performance.c tests\test_factory.c src\persistencia.c -o tests\bin_test_performance.exe
if %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%

echo Rodando testes...
tests\bin_test_cliente.exe
if %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%

tests\bin_test_produto.exe
if %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%

tests\bin_test_pedido.exe
if %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%

tests\bin_test_devolucao.exe
if %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%

tests\bin_test_relatorio.exe
if %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%

tests\bin_test_edge_cases.exe
if %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%

tests\bin_test_performance.exe
if %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%

echo Todos os testes passaram!
