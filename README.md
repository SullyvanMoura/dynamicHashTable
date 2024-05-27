# Hash de Dimensão Dinâmica em C

Implementação de hash de dimensão dinâmica em C com valores de tamanho da hash (m) e fator de carga máximo parametrizáveis

## Como executar

O programa conta com 3 testes prontos. Para compilar e executar o programa é necessário apenas executar um dos seguintes comandos:

``make test1`` , ``make test2`` ou ``make test3``

*Observação: Executar um teste limpa os dados anteriores dos arquivos para que não haja conflito com os metadados de testes anteriores!!!*

## Testes

### Teste 1
O teste 1 é uma hash de tamanho inicial 7 e fator de carga máximo de 1.0
Os seguintes registros são inseridos nesta ordem:

**[49 | JOAO] -> [51 | CARLA] -> [59 | MARIA] -> [3 | JOSE] -> [87 | BIA] -> [103 | ANA] -> [7 | CARLOS] -> [8 | JOAO] -> [14 | CAROL] -> [15 | PEDRO]**

### Teste 2

O teste 2 é uma hash de tamanho inicial 4 e fator de carga máximo de 1.2
Os seguintes registros são inseridos nesta ordem:

**[5 | JOAO] -> [6 | PEDRO] -> [3 | MARIA] -> [15 | ANTONIO] -> [4 | ANA] -> [12 | JOSE] -> [10 | BIA] -> [11 | CARLA] -> [8 | CARLOS] -> [23 | RAFAEL] -> [23 | RAFAEL] -> [36 | FATIMA]**

### Teste 3

O teste 3 é uma hash de tamanho inicial 2 e fator de carga máximo de 1.0
Os seguintes registros são inseridos nesta ordem:

**[0 | JOAO] -> [2 | PEDRO] -> [4 | MARIA] -> [6 | ANTONIO] -> [8 | ANA] -> [10 | JOSE] -> [12 | BIA] -> [14 | CARLA] -> [200 | CARLOS]**