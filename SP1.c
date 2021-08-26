//SP1Emulator
//Author: Denis Rezende aka PromptMaster
//Date: Aug-2021

#include <stdio.h>

typedef unsigned char byte;

typedef struct Instruction_format{
    byte opcode;
    byte operand;
}instruction_format;

//SP1 INSTRUCTIONS
#define LOAD_MEMORY 0
#define LOAD_VALUE  1
#define STORE       2
#define ADD         3
#define SUB         4
#define MUL         5
#define DIV         6
#define INC         7
#define DEC         8
#define AND         9
#define OR          10
#define NOT         11
#define JMP         12
#define JZ          13
#define JNZ         14
#define HLT         15

//STAT BITS
#define OVERFLOW    4     
#define CARRY       2
#define ZEROACC     1
#define STAT_CLEAR  0

//REGISTERS
byte stat = 0;
byte acc  = 0;
byte pc   = 0;

//MEMORY
byte memory[256];

//PROGRAM ARRAY
instruction_format instruction[256];

void check_stat(byte test, int operand);
int is_stat_bit_on(int bit_position);


int main(int argc, char const *argv[])
{
    FILE *BIN_FILE = fopen(argv[1], "rb");
    if (BIN_FILE == NULL)
    {
        printf("error: no file specified\n");
        return 1;
    }

    byte buffer[2];

    while(fread(buffer, sizeof(byte), 2, BIN_FILE))
    {
        instruction[pc].opcode  = buffer[0];
        instruction[pc].operand = buffer[1];
        ++pc;
        //printf("debug %d  opcode:%-3u  operand:%-3u\n", pc-1, instruction[ pc-1 ].opcode, instruction[ pc-1 ].operand);
    }
    fclose(BIN_FILE);

    pc = 0;

    while(instruction[pc].opcode != HLT)
    {
        byte opcode = instruction[pc].opcode;
        byte operand = instruction[pc].operand;

        switch (opcode)
        {
        case LOAD_MEMORY:
            acc = memory[operand];
            break;
        
        case LOAD_VALUE:
            acc = operand;
            break;

        case STORE:
            memory[operand] = acc;
            break;
        
        case ADD:
            check_stat(CARRY, acc + memory[operand]);
            acc += memory[operand];
            break;
        
        case SUB:
            check_stat(CARRY, acc - memory[operand]);
            acc -= memory[operand];
            break;

        case MUL:
            check_stat(OVERFLOW, acc * memory[operand]);
            acc *= memory[operand];
            break;
        
        case DIV:
            if (memory[operand] != 0)
                acc /= memory[operand];

            break;

        case INC:
            check_stat(CARRY, acc + 1);
            ++acc;
            break;

        case DEC:
            check_stat(CARRY, acc - 1);
            --acc;
            break;

        case AND:
            acc = acc & memory[operand];
            break;

        case OR:
            acc = acc | memory[operand];
            break;
        
        case NOT:
            acc = ~acc;
            break;

        case JMP:
            pc = operand - 1;
            break;
        
        case JZ:
            if (!acc)
                pc = operand - 1;            
            break;

        case JNZ:
            if (acc)
                pc = operand - 1;            
            break;
        
        case HLT:
            break;

        default:
            printf("error: instruction %X on line %d is a invalid instruction\n", opcode, pc);
            break;
        }
        check_stat(ZEROACC, acc);
        
        ++pc;
    }

    printf("  acc: %d\n" , acc);
    printf(" stat: xxxxx%c", stat & OVERFLOW ? '1' : '0');
    printf("%c"            , stat & CARRY    ? '1' : '0');
    printf("%c\n"          , stat & ZEROACC  ? '1' : '0');
    
    return 0;
}

void check_stat(byte test, int value)
{
    switch (test)
    {
    case ZEROACC:
        if (value == 0 && !is_stat_bit_on(ZEROACC))
        {
            stat += ZEROACC;
        }else if (value != 0 && is_stat_bit_on(ZEROACC))
        {
            stat -= ZEROACC;
        }        
        break;

    case CARRY:
        if ((value < 0 || value > 255) && !is_stat_bit_on(CARRY))
        {    
            stat += CARRY;
        }else if (is_stat_bit_on(CARRY))
        {
            stat -= CARRY;
        }

        break;

    case OVERFLOW:
        if ((value < 0 || value > 255) && !is_stat_bit_on(OVERFLOW))
        {
            stat += OVERFLOW;
        }else if (is_stat_bit_on(OVERFLOW))
        {
            stat -= OVERFLOW;
        }    
        break;    
    }
    
}

int is_stat_bit_on(int bit_position)
{
    return (stat & bit_position);
}
