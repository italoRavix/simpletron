# Simpletron
Simpletron é uma simulação de um computador simples de terminal com um unico registrador (acumulador) de propósito geral.\
Atualmente suas capacidades incluem: as quatro operaçoes aritméticas, exponenciaçao e modulo de um inteiro; entrada e saida de dados e instruções via console ou arquivo.\
Tudo deve ser escrito usando os opcodes.
## Opcodes
**READ 10**: Obtém um dado da entrada padrão para o endereço de memória indicado pelo operando.\
**WRITE 11**: Escreve um dado na saída padrão de um endereço de memória indicado pelo operando.\
**LOAD 20**: Carrega o conteúdo de um endereço de memória indicado pelo operando para o acumulador.\
**STORE 21**: Armazena o conteúdo do acumulador em um endereço de memória indicado pelo operando.\
**ADD 30**: Soma o conteúdo de um endereço de memória indicado pelo operando ao conteúdo no acumulador.\
**SUBTRACT 31**: Subtrai o conteúdo de um endereço de memória indicado pelo operando do conteúdo no acumulador.\
**DIVIDE 32**: Divide pelo conteúdo de um endereço de memória indicado pelo operando o conteúdo no acumulador.\
**MULTIPLY 33**: Multiplica pelo conteúdo de um endereço de memória indicado pelo operando o conteúdo no acumulador.\
**MOD 34**: O conteúdo do acumulador recebe seu valor em módulo.\
**EXPONENTIAL 35**: Eleva pelo conteúdo de um endereço indicado pelo operando o conteúdo no acumulador.\
**BRANCH 40**: Transfere a execução para o endereço indicado pelo conteúdo do operando.\
**BRANCHNEG 41**: Transfere a execução para o endereço indicado pelo conteúdo do operando se o valor no acumulador for negativo.\
**BRANCHZERO 42**: Transfere a execução para o endereço indicado pelo conteúdo do operando se o valor no acumulador for zero.\
**HALT 43**: Indica o fim da execução do programa.\
