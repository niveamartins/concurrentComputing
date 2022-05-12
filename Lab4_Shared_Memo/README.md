
## Tempo de Execução


|                              | Execução de Dimensão 10⁶ | Execução de Dimensão 10⁷ | Execução de Dimensão 10⁸ |
|------------------------------|--------------------------|--------------------------|--------------------------|
| Tempo de Execução Sequencial | 0.019901                 | 0.197137                 | 1.976923                 |
| Tempo de Execução 1 thread   | 0.027963                 | 0.275450                 | 3.044194                 |
| Tempo de Execução 2 threads  | 0.081579                 | 0.696235                 | 7.541972                 |
| Tempo de Execução 4 threads  | 0.093294                 | 0.789101                 | 10.229925                 |


## Ganhos na Execução


|                 | Execução de Dimensão 10⁶ | Execução de Dimensão 10⁷ | Execução de Dimensão 10⁸ |
|-----------------|--------------------------|--------------------------|--------------------------|
| Ganho 1 thread  | 0.711604                 | 0.715690                 | 0.649407                 |
| Ganho 2 threads | 0.243947                 | 0.283147                 | 0,262122                 |
| Ganho 4 threads | 0.213314                 | 0.249824                 | 0.193249                 |



O computador utilizado para a execução contém um total de 8 processadores.

O resultado não foi o que eu esperava por um ponto específico: o tempo de execução sequencial ser MENOR do que o tempo de execução de uma única thread.