// Exercicio 07_28
// Implementa um computador de arquitetura simples chamada Simpletron. O computador tem um unico registrador de uso geral, o acumulador. 
// Na configuração atual seu armazenamento tem espaço para 1000 dados/instruçoes de quatro digitos decimais. 
// Há implementação de algumas verificações simples em tempo execução: Divisão por zero, overflow/underflow aritmético e codigos de operação inválidos.
// Há implementação de entrada via console ou arquivo. Para uma entrada via arquivo o nome do arquivo deve ser 'instructions.txt'

#include <stdio.h>

// prototipos
void simpletron();
void printSimpletron(const int item, const int accumulator, const int instructionCounter, const int instructionRegister, const int operationCode, const int operand, const int memory[]);
void printError(const int errorType, const int error);
void verifyOverflow(const int accumulator, int * const operationCodePtr);

// pre configuraçoes
#define MEMORY_SIZE 1000
#define MEMORY_VALUE_MAX 99999
#define MEMORY_VALUE_MIN -99999
#define SENTINELA -999999
#define NONE 0 // usado para parametros nao utilizados nas funçoes

// codigos de operaçao
#define READ 10
#define WRITE 11
#define LOAD 20
#define STORE 21
#define ADD 30
#define SUBTRACT 31
#define DIVIDE 32
#define MULTIPLY 33
#define MOD 34
#define EXPONENTIAL 35
#define BRANCH 40
#define BRANCHNEG 41
#define BRANCHZERO 42
#define HALT 43
#define ERROR -99999 // codigo para erros em tempo de execuçao

// indice para as impressoes
#define START 0
#define START_EXECUTION 1
#define REGISTERS 2
#define MEMORY 3
#define TYPE_OF_INPUT 4
#define END 5
#define ABNORMAL_END 6

// indice para impressao de erros
#define ERROR_LOAD 0
#define ERROR_EXECUTION 1
// erros em tempo de carga
#define INVALID_VALUE 0
#define INVALID_OPTION 1
// erros em tempo de execução
#define DIVISION_BY_ZERO 0
#define OVERFLOW 1
#define NOEXISTENT_OPCODE 2

int main(void)
{
    simpletron();
}

void simpletron()
{
    // simula a memoria do Simpletron
    int memory[MEMORY_SIZE] = {0};
    
    // registradores
    int accumulator = 0; // uso geral
    int instructionCounter = 0; // armazena o local da memoria que contem a instruçao sendo executada
    int operationCode = 0; // armazena os dois digitos da esquerda de uma palavra de instruçao
    int operand = 0; // armazena os dois digitos da direita de uma palavra de instruçao
    int instructionRegister = 0; // armazena a proxima instruçao a ser executada da memoria
    
    // logica while
    int instruction = 0; // armazena as instrucoes que serao de entrada
    int numInstructions = 0; // armazena o numero de instruçoes de entrada
    int sentinelaExecucao = 0; // verifica se a execuçao deve ou nao continuar  
    
    // escolha de entrada do usuario
    FILE *fPtr; // ponteiro para arquivo    
    enum {TERMINAL = 1, FILE = 2} choice;
    
    // printa mensagem de boas vindas
    printSimpletron(START, NONE, NONE, NONE, NONE, NONE, NONE);
    
    printSimpletron(TYPE_OF_INPUT, NONE, NONE, NONE, NONE, NONE, NONE);
    scanf("%u", &choice);

    switch(choice)
    {
    // adiciona instruçoes à memoria do Simpletron via console
        case TERMINAL:
            while(instruction != SENTINELA)
            {
                printf("%02d ? ", numInstructions);
                scanf("%d", &instruction);
                
                if(instruction != SENTINELA && instruction >= MEMORY_VALUE_MIN && instruction <= MEMORY_VALUE_MAX)
                {
                    memory[numInstructions] = instruction;
                    numInstructions++;
                }
                // avalia se as instruçoes estao dentro do limite valido
                else if(instruction != SENTINELA && (instruction <= MEMORY_VALUE_MIN || instruction >= MEMORY_VALUE_MAX))
                {
                    printError(ERROR_LOAD, INVALID_VALUE);
                }
            }   
            break;
        
        case FILE:
            
            if((fPtr = fopen("instruction.txt", "r")) == NULL)
                printf("Erro ao arquivo arquivo"); 
            else
            {
                // adiciona instruçoes à memoria do Simpletron via arquivo 
            
                while(instruction != SENTINELA && fscanf(fPtr, "%d", &instruction) != EOF)
                {   
                    if(instruction != SENTINELA && instruction >= MEMORY_VALUE_MIN && instruction <= MEMORY_VALUE_MAX)
                    {
                        memory[numInstructions] = instruction;
                        numInstructions++;
                    }
                    // avalia se as instruçoes estao dentro do limite valido
                    else if(instruction != SENTINELA && (instruction <= MEMORY_VALUE_MIN || instruction >= MEMORY_VALUE_MAX))
                    {
                        printError(ERROR_LOAD, INVALID_VALUE);
                        return;
                    }
                }    
                
                fclose(fPtr); // fecha o arquivo
            }
            
            break;
            
        default:
            printError(ERROR_LOAD, INVALID_OPTION);
            printSimpletron(END, NONE, NONE, NONE, NONE, NONE, NONE);
            return;
    }
    
    printSimpletron(START_EXECUTION, NONE, NONE, NONE, NONE, NONE, NONE);  
    
    // começa a execução
    while(sentinelaExecucao != SENTINELA)
    {
        
        // avalia se ha erro em tempo de execuçao
        if(operationCode != ERROR)
        {
            // atualiza os registradores        
            instructionRegister = memory[instructionCounter];
            operationCode = instructionRegister / MEMORY_SIZE;
            operand = instructionRegister % MEMORY_SIZE;
        
        // imprime o estado atual
        printSimpletron(REGISTERS, accumulator, instructionCounter, instructionRegister, operationCode, operand, NONE); 
        }
        
        instructionCounter++;        
        // seleciona a operaçao
        switch(operationCode)
        {
            case READ:
                printf("INPUT: ");            
                scanf("%d", &memory[operand]);
                break;
                
            case WRITE:
                printf("OUTPUT: %d\n", memory[operand]);
                break;  
                
            case LOAD:
                accumulator = memory[operand];
                break;
                
            case STORE:
                memory[operand] = accumulator;
                break;  

            case ADD:
                accumulator += memory[operand];
                verifyOverflow(accumulator, &operationCode);
                break;
                
            case SUBTRACT:
                accumulator -= memory[operand];
                verifyOverflow(accumulator, &operationCode);
                break;  
                
            case DIVIDE:
            
                // avalia divisao por zero
                if(memory[operand] == 0)
                {
                    printError(ERROR_EXECUTION, DIVISION_BY_ZERO);
                    operationCode = ERROR;
                    break;
                }
                       
                accumulator /= memory[operand];
                verifyOverflow(accumulator, &operationCode);
                
                break;
                
            case MULTIPLY:
                accumulator *= memory[operand];
                verifyOverflow(accumulator, &operationCode);
                break;  
                
            case MOD:
                if (accumulator < 0)
                  accumulator *= -1;
                
                break;

            case EXPONENTIAL:
            
                if(accumulator == 1)
                    break;
                    
                if (memory[operand] == 0)
                    accumulator = 1;
                    
                else if(memory[operand] > 1)
                {
                    int total = 1;
                    for(int i = 0; i < memory[operand]; i++)
                        total *= accumulator;  
                    
                    accumulator = total;
                    verifyOverflow(accumulator, &operationCode);             
                }
                
                else if(memory[operand] < 0)
                {
                // ERRO DE EXECUÇAO: RACIONALIZAÇAO AINDA NAO SUPORTADA
                }
                
                break;    

            case BRANCH:     
                instructionCounter = operand;
                break;
                
            case BRANCHNEG:
                if(accumulator < 0)
                    instructionCounter = operand;
                break;
                
            case BRANCHZERO:
                if(accumulator == 0)
                    instructionCounter = operand;
                break;
                
            case HALT:
                printSimpletron(END, NONE, NONE, NONE, NONE, NONE, NONE);
                printSimpletron(MEMORY, NONE, NONE, NONE, NONE, NONE, memory);
                sentinelaExecucao = SENTINELA;
                break;  
                
            case ERROR:
                printSimpletron(END, NONE, NONE, NONE, NONE, NONE, NONE);
                printSimpletron(MEMORY, NONE, NONE, NONE, NONE, NONE, memory);
                sentinelaExecucao = SENTINELA;
                break;    
                
            default:
                operationCode = ERROR;
                printError(ERROR_EXECUTION, NOEXISTENT_OPCODE);
        }
        
    }
}

// imprime mensagens e 'telas' para o usuario
void printSimpletron(const int item, const int accumulator, const int instructionCounter, const int instructionRegister, const int operationCode, const int operand, const int memory[])
{
    switch(item)
    {
        case START:
            printf("\n\n*** Bem vindo ao Simpletron! ***\n");
            printf("*** Favor digitar seu programa, uma instrução  ***\n");
            printf("*** (ou palavra de dados) por vez. Mostrarei   ***\n");
            printf("*** o número do local e uma interrogação (?)   ***\n");
            printf("*** Você, então deverá digitar a palavra para  ***\n");
            printf("*** esse local. Digite a sentinela [%d]   ***\n", SENTINELA);
            printf("*** para encerrar a entrada do seu programa.   ***\n"); 
            printf("*** Alternativamente voce pode digitar a       ***\n");
            printf("*** entrada em um arquivo com nome de          ***\n");                         
            printf("*** 'instructions.txt' colocando uma           ***\n"); 
            printf("*** instruçao por linha.                       ***\n\n");             
            break;
            
       case START_EXECUTION:
           printf("*** Carga do programa concluída ***\n");
           printf("*** Iniciando execução do programa ***\n");  
            
        case REGISTERS:
            printf("\nREGISTERS:\n");
            printf("accumulator               %05d\n", accumulator);
            printf("instructionCounter          %03d\n", instructionCounter);
            printf("instructionRegister       %05d\n", instructionRegister);    
            printf("operationCode               %03d\n", operationCode);    
            printf("operand                     %03d\n\n", operand);          
            break;
            
        case MEMORY:      
            printf("MEMORY:\n\n");
            printf("     %5d   %5d   %5d   %5d   %5d   %5d   %5d   %5d   %5d   %5d\n", 0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
            
            for(int i = 0; i < MEMORY_SIZE; i++)
            {
                if(i % 10 == 0)
                    printf("\n%4d  ", i);
                    
                printf("%+06d  ", memory[i]);
            }
            
            printf("\n\n");     
            break;
            
        case END:
            printf("*** Execução do Simpletron encerrada ***\n\n"); 
            break;
            
        case ABNORMAL_END:
            printf("*** Execução do Simpletron encerrada de forma anormal ***\n\n");
            break;
        
        case TYPE_OF_INPUT:
            printf("[1] Entrada via console.\n[2] Entrada via arquivo.\n? ");
            break;
            
        default:
            printf("\n\nIMPRESSAO NAO DEFINIDA\n\n");
            break;
    }
}


// imprime os erros para o usuario
void printError(const int errorType, const int error)
{
    if(errorType == ERROR_LOAD)
    {
        switch(error)
        {
            case INVALID_VALUE:
                printf("Insira um valor válido. Intervalo [%d, %d]\n", MEMORY_VALUE_MIN, MEMORY_VALUE_MAX);
                break;
            case INVALID_OPTION:
                printf("Opção Invalida!\n");            
        }
    }
    else if(errorType == ERROR_EXECUTION)
    {
        switch(error)
        {
            case DIVISION_BY_ZERO:
                printf("*** Tentativa de divisão por zero ***\n");
                break;
            case OVERFLOW:
                printf("*** Overflow/Underflow aritmético ***\n");    
                break;
                
            case NOEXISTENT_OPCODE:      
                printf("*** Código de operação inválido ***\n");
                break;   
        }
    }
}

// avalia overflow e underflow aritmetico                
void verifyOverflow(const int accumulator, int * const  operationCodePtr)
{
    if(accumulator > MEMORY_VALUE_MAX || accumulator < MEMORY_VALUE_MIN)
    {
        printError(ERROR_EXECUTION, OVERFLOW);
        *operationCodePtr = ERROR;
    }    
}
