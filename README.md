# SP1ProcessorEmulator

1 - Compile the program 

2 - On runtime specify on the command line the .bin file to be executed.
Example:
./SP1 file.bin



General:

The SP1 is a hypothetical processor for academic means only. It has three registers, each one with 8 bits (1 byte): 
acc (accumulator), stat (status) and pc (program counter). The processor's ISA is made of 16 instructions. 
The instructions format is fixed, in this case 16 bits (8 bits for the opcode and 8 bits for the operand). 
The stat register keeps informations about carry, overflow and acc's register zero.



Registers:

The accumulator (acc) register receives all the results of logical and arithmetic operations. 
Being an essential register in 1-address (1 operand) architectures based on accumulator. 
The register pc (program counter) contains the memory location (address) where the next instruction must be searched.



Instruction Format:

SP1 supports 16-bit fixed instructions as specified below:


        8 bits                 8 bits
        
+--------------------+---------------------+

+--------------------+---------------------+

|------(opcode)------|-------(operand)-----|

+--------------------+---------------------+

+------------------------------------------+

                    16 bits
                    
The operation code or opcode field uniquely identifies the instruction that will be performed. This way, each instruction has an associated opcode. 
The reference field identifies the value or location of the data that will be used in the instruction. 
As SP1 has only 16 instructions, only 4 bits would be needed to represent the operation. For the Reference field we can use data with 8 bits. 
Thus, we can reference (address) up to 28 bytes in memory. The code snippet below shows how the algorithm works to fetch an instruction from memory: 


instrution = program[pc];     // Fetch the instruction to be performed


opcode = instruction.opcode;


operand = instruction.operand;


pc = pc + 1;                  // Increase the program counter



Instructions:


Instruction---Opcode(binary)-------Comment

Load-----------0(00000000)----------Memory

Load-----------1(00000001)----------Value

store----------2(00000010)

add------------3(00000011)

sub------------4(00000100)

mul------------5(00000101)

div------------6(00000110)

inc------------7(00000111)

dec------------8(00001000)

and------------9(00001001)

or------------10(00001010)

not-----------11(00001011)

jmp-----------12(00001100)

jz------------13(00001101)

jnz-----------14(00001110)

hlt-----------15(00001111)-------End of program
