# Laboratório 1 - Introdução a biblioteca Pthread

## Atividade 1 - Hello.c

> Objetivo: Mostrar como criar um programa concorrente em C usando a lib pthread.

### Há mudança na ordem de execução das threads? Por que isso ocorre?

Sim, pois o tempo de finalização das threads e a ordem de comandos dados pelo programa sequencial muda a cada nova inicialização.
Por exemplo, na primeira feita em meu computador, foram necessários 2 loops do for para imprimir o primeiro "Hello World" (ou seja, fim da primeira thread). Já na segunda, foram necessários 3 loops do for para que o primeiro "Hello World" acontecesse. Além disso, em algumas execuções, a thread principal termina antes das threads secundárias.
Essa ordem altera, pois o fluxo sequencial não espera o fim da thread iniciada.


## Atividade 2 - Hello_arg.c


> Objetivo: Mostrar como passar UM argumento para uma thread.

### Qual foi a diferença em relação ao programa anterior?

A diferença não está na execução e sim que, agora, podemos saber qual thread está finalizando ao imprimir o "Hello World" tendo em vista que estamos passando o valor da variável auxiliar _thread_ por meio do último argumento da função _int pthread\_create_ que é responsável por passar para a nossa rotina o valor do argumento.


## Atividade 3 - Hello_args.c


> Objetivo: Mostrar como passar MAIS DE UM argumento para uma thread.

### O programa funcionou como esperado?

Sim. Aloca um espaço na memória, armazena 2 valores nesse espaço, passa os endereços para a thread e, a partir daí, a thread consegue realizar a rotina por meio dos ponteiros criados.



## Atividade 4 - Hello_join.c


> Objetivo: Mostrar como fazer a thread principal (main) aguardar as outras threads terminarem.

### O que aconteceu de diferente em relação às versões anteriores?

Dessa vez, a thread principal só terminou, ou seja, imprimiu o "--Thread principal terminou", depois que todas as threads secundárias terminaram seus respectivos processos.




