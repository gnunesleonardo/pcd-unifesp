Credito:
Mark Ebersole - NVIDIA Corporation

Descrição:
Código de Hello World muito simples que tem o objetivo de auxiliar na demonstração de alto paralelismo existente em GPUs modernas.

Código fonte:

    hello.cu - exemplo de Hello World

Compilação:

  nvcc -o hello hello.cu

Exercícios:
1. Compile e execute o código. 

2. Observe os parâmetros entre <<< e >>> na main. Como veremos a seguir, estes são os parâmetros de lançamento do kernel e indicam a quantidade de Blocos e Threads que serão executados. Experimente as seguintes alterações e observe o que acontece:
a) altere apenas o primeiro parâmetro para 10: <<<10, 1>>>
b) altere apenas o segundo parâmetro para 10: <<<1, 10>>>
c) altere os dois: <<<10, 10>>>

