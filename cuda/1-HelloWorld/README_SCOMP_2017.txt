Credito:
Mark Ebersole - NVIDIA Corporation

Descrição:
Código de Hello World muito simples que tem o objetivo de auxiliar na demonstração de alto paralelismo existente em GPUs modernas.

Código fonte:

    hello.cu - exemplo de Hello World

Compilação:

  nvcc -o hello hello.cu

Exercícios:
1. Compile e submeta na fila de execução. 

Use o script padrão denominado "scomp-template-gpu.slurm" (disponível no diretório raiz), para submeter na fila.
Este script usa o gerenciador de tarefas denominado "Slurm", para direcionar uma tarefa para a fila denominada "gpushort", requisitando um único nó de computação (host) e uma única GPU (device). O script deve ser alterado para trocar o nome do programa executável compilado. O padrão do script chama-se "A.out", mas neste exemplo teremos "hello"

2. Observe os parâmetros entre <<< e >>> na main. Como veremos a seguir, estes são os parâmetros de lançamento do kernel e indicam a quantidade de Blocos e Threads que serão executados. Experimente as seguintes alterações e observe o que acontece:
a) altere apenas o primeiro parâmetro para 10: <<<10, 1>>>
b) altere apenas o segundo parâmetro para 10: <<<1, 10>>>
c) altere os dois: <<<10, 10>>>

