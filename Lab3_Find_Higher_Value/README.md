
## Tempo de Execução


|                              | Execução de Dimensão 10⁶ | Execução de Dimensão 10⁷ | Execução de Dimensão 10⁸ |
|------------------------------|--------------------------|--------------------------|--------------------------|
| Tempo de Execução Sequencial | 0.000220                 | 0.002429                 | 0.022784                 |
| Tempo de Execução 1 thread   | 0.000597                 | 0.002783                 | 0.032380                 |
| Tempo de Execução 2 threads  | 0.000432                 | 0.001444                 | 0.013211                 |
| Tempo de Execução 4 threads  | 0.000321                 | 0.001321                 | 0.012246                 |


## Ganhos na Execução


|                 | Execução de Dimensão 10⁶ | Execução de Dimensão 10⁷ | Execução de Dimensão 10⁸ |
|-----------------|--------------------------|--------------------------|--------------------------|
| Ganho 1 thread  | 0.368509                 | 0.872799                 | 0.703644                 |
| Ganho 2 threads | 0.509259                 | 1.682132                 | 1.724623                 |
| Ganho 4 threads | 0.685358                 | 1.838758                 | 1.860525                 |



O computador utilizado para a execução contém um total de 8 processadores.

O resultado não foi o que eu esperava por um ponto específico: o tempo de execução sequencial ser MENOR do que o tempo de execução de uma única thread.