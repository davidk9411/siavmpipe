/*
Assignment 4 _ PIPED SIAVM
Made by David Kim
Last Modified: 04/21/2020
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

//Personally defined headers
#include "functions.h"
#include "constants.h"

//External Global Variables(System resources)
extern Memory sys_memory;
extern int cpu_register[NUM_REG];
extern int halt;
extern int mem_counter;
extern unsigned char *current_instruction[3];
extern int fetch_more;
extern int **reg_forwarder;
extern void (*executer)();

//Fetches instruction from memory to current instruction
void fetch(){

    unsigned char opcode = mem_search(&sys_memory,mem_counter)>>4;
    
    if(fetch_more!=TRUE){
        return;
    }

    if(opcode == 7){ 
        current_instruction[0] = (unsigned char *)malloc(sizeof(unsigned char)*4);
        for(int i=0; i<4; i++)
            *(current_instruction[0]+i) = mem_search(&sys_memory,mem_counter+i);
    }
    
    else {
        current_instruction[0] = (unsigned char *)malloc(sizeof(unsigned char)*2);
        *(current_instruction[0]+0) = mem_search(&sys_memory,mem_counter);
        *(current_instruction[0]+1) = mem_search(&sys_memory,mem_counter+1);
    }

    current_instruction[1] = current_instruction[0];
    current_instruction[0] = NULL;
}

//Decodes instruction
//Return type:
//0 - Success
//1 - Unsupported opcode
//2 - Unsupported branch type
//3 - No Instructions Available
//4 - Unsupported stack ops
int decode(){


    if(current_instruction[1] == NULL){
       return 3;
    }


    int opcode = *(current_instruction[1]+0) >> 4;
    //Default value is 1
    int internal_reg_size=1;
    
    //Calculates internal reigster size
    if(opcode>=1 && opcode<=6)
        internal_reg_size = 3;
    else if(opcode>=7&&opcode<=9)
        internal_reg_size = 2;

    //If size is 1, internal register is not utilized
    //Generates internal register
    int *internal_reg[internal_reg_size];

    switch(opcode){
        //OPCODE 0: HALT
        case 0:
            //Assigns halt ops to function pointer    
            executer=halt_SIA;
            //Prohibits fetching any further instructions
            fetch_more=FALSE;
            break;
        //OPCODE 1: ADD
        case 1:
            //Process proper internal register
            get_registers(internal_reg,1);
            //Assigns memory for internal reigster forwarding purpose
            reg_forwarder = (int **)malloc(sizeof(internal_reg));
            for(int i=0; i<3; i++)
                reg_forwarder[i] = (int*)malloc(sizeof(int));
            //Copies memory from internal register to forwarder
            memcpy(reg_forwarder,internal_reg,sizeof(internal_reg));
            //Assign add execute operation to executer pointer
            executer=addr;
            mem_counter+=2;
            break;
        //OPCODE 2: AND
        case 2:
            //Process proper internal register
            get_registers(internal_reg,1);
            //Assigns memory for internal reigster forwarding purpose
            reg_forwarder = (int **)malloc(sizeof(internal_reg));
            for(int i=0; i<3; i++)
                reg_forwarder[i] = (int*)malloc(sizeof(int));
            //Copies memory from internal register to forwarder
            memcpy(reg_forwarder,internal_reg,sizeof(internal_reg));
            //Assign add execute operation to executer pointer
            executer=andr;
            mem_counter+=2;
            break;
        //OPCCDE 3: DIVIDE
        case 3:
            //Process proper internal register
            get_registers(internal_reg,1);
            //Assigns memory for internal reigster forwarding purpose
            reg_forwarder = (int **)malloc(sizeof(internal_reg));
            for(int i=0; i<3; i++)
                reg_forwarder[i] = (int*)malloc(sizeof(int));
            //Copies memory from internal register to forwarder
            memcpy(reg_forwarder,internal_reg,sizeof(internal_reg));
            //Assign add execute operation to executer pointer
            executer=divider;
            mem_counter+=2;
            break;
        //OPCODE 4: MULTIPLY
        case 4:
            //Process proper internal register
            get_registers(internal_reg,1);
            //Assigns memory for internal reigster forwarding purpose
            reg_forwarder = (int **)malloc(sizeof(internal_reg));
            for(int i=0; i<3; i++)
                reg_forwarder[i] = (int*)malloc(sizeof(int));
            //Copies memory from internal register to forwarder
            memcpy(reg_forwarder,internal_reg,sizeof(internal_reg));
            //Assign add execute operation to executer pointer
            executer=multiplyr;
            mem_counter+=2;
            break;
        //OPCODE 5: SUBTRACT
        case 5:
            //Process proper internal register
            get_registers(internal_reg,1);
            //Assigns memory for internal reigster forwarding purpose
            reg_forwarder = (int **)malloc(sizeof(internal_reg));
            for(int i=0; i<3; i++)
                reg_forwarder[i] = (int*)malloc(sizeof(int));
            //Copies memory from internal register to forwarder
            memcpy(reg_forwarder,internal_reg,sizeof(internal_reg));
            //Assign add execute operation to executer pointer
            executer=subtractr;
            mem_counter+=2;
            break;
        //OPCODE 6: OR
        case 6:
            //Process proper internal register
            get_registers(internal_reg,1);
            //Assigns memory for internal reigster forwarding purpose
            reg_forwarder = (int **)malloc(sizeof(internal_reg));
            for(int i=0; i<3; i++)
                reg_forwarder[i] = (int*)malloc(sizeof(int));
            //Copies memory from internal register to forwarder
            memcpy(reg_forwarder,internal_reg,sizeof(internal_reg));
            //Assign add execute operation to executer pointer
            executer=orr;
            mem_counter+=2;
            break;
        //OPCODE 7: BRANCH
        case 7:
            if(decode_branch(internal_reg)!=0){
                return 2;
            }
            break;
        //OPCODE 8: LOAD
        case 8:
            //Process proper internal register
            get_registers(internal_reg,3);
            //Assigns memory for internal reigster forwarding purpose
            reg_forwarder = (int **)malloc(sizeof(internal_reg));
            for(int i=0; i<internal_reg_size; i++)
                reg_forwarder[i] = (int*)malloc(sizeof(int));
            //Copies memory from internal register to forwarder
            memcpy(reg_forwarder,internal_reg,sizeof(internal_reg));
            mem_counter+=2;
            //Assign add execute operation to executer pointer
            executer=load_memory_ops;
            break;
        //OPCODE 9: STORE
        case 9:
            //Process proper internal register
            get_registers(internal_reg,3);
            //Assigns memory for internal reigster forwarding purpose
            reg_forwarder = (int **)malloc(sizeof(internal_reg));
            for(int i=0; i<internal_reg_size; i++)
                reg_forwarder[i] = (int*)malloc(sizeof(int));
            //Copies memory from internal register to forwarder
            memcpy(reg_forwarder,internal_reg,sizeof(internal_reg));
            mem_counter+=2;
            //Assign add execute operation to executer pointer
            executer=store_memory_ops;
            break;
        //OPCODE A: STACK
        case 10:
            //decode stack
            if(decode_stack()!=0)
                return 4;
            break;
        //OPCODE B: MOVE
        case 11:
            //Assigns halt ops to function pointer    
            executer=move;
            mem_counter+=2;
            break;
        //OPCODE C: INTERRUPT
        case 12:
            executer=interrupt;
            mem_counter+=2;
            break;
        //DEFAULT: ERROR
        default:
            return 1; 
    }

    //Forwards instruction to next array portion
    current_instruction[2] = current_instruction[1];
    //Resets the pointer
    current_instruction[1] = NULL;

    return 0;
}

//OPCODE 7: BRANCH SERIES DECODER
int decode_branch(int **reg){

    //Stall set up, halts fetching further instructions
    fetch_more=FALSE;

    //Calculates branch type
    int type = *(current_instruction[1]+0) % 16;

    //If branch type is not CALL or JUMP
    if(type>=0 && type<=5){
        //Obtains internal registers
        get_registers(reg,2);
        //Set up buffer to forward internal registers
        reg_forwarder = (int **)malloc(sizeof(reg)*2);
        for(int i=0; i<2; i++)
            reg_forwarder[i] = (int*)malloc(sizeof(int));
        //Copies internal register to buffer
        memcpy(reg_forwarder,reg,sizeof(reg)*2);
    }

    switch (type){

    // TYPE 0: BRANCHIFLESS
    case 0:
        //Calls if less execution on next cycle
        executer=if_less;
        return 0;
    // TYPE 1: BRANCHIFLESSEQUAL
    case 1:
        //Calls if less equal execution on next cycle
        executer=if_less_equal;
        return 0;   
    // TYPE 2: BRANCHIFEQUAL
    case 2:
        //Calls if equal execution on next cycle
        executer=if_equal;
        return 0;
    // TYPE 3: BRANCHIFENOTQUAL
    case 3:
        //Calls if not equal execution on next cycle
        executer=if_not_equal;
        return 0;      
    // TYPE 4: BRANCHIFGREATER
    case 4:
        //Calls if greater execution on next cycle
        executer=if_greater;
        return 0;
    // TYPE 5: BRANCHIFGREATEREQUAL
    case 5:
        //Calls if greater equal execution on next cycle
        executer=if_greater_equal;
        return 0;
    // TYPE 6: CALL
    case 6:
        //Calls call ops on next cycle
        executer=call_fn;
        return 0;
    // TYPE 7: JUMP
    case 7:
        //Calls jump ops on next cycle
        executer=jump;
        return 0;    
    default:
        return 1;
    }
}

int decode_stack(){

    //Get type of stack operation from current instruction register
    int ops = (int)current_instruction[1][1]>>6;

    switch (ops)
    {
    //RETURN
    case 0:
        //Execute return on next cycle
        executer=return_R15;
        //Buffer lock
        fetch_more = FALSE;
        return 0;
    //PUSH
    case 1:
        executer=push_R15;
        mem_counter+=2;
        return 0;
    //POP
    case 2:
        executer=pop_R15;
        mem_counter+=2;
        return 0;
    default:
        return 1;
    }
}

//Internal register handler
void get_registers(int **reg, int handler_type){

    //Handler for 3R insturctions
    if(handler_type==1){
        int index[3];

        //Obtains cpu_register number for OP1
        index[0] = *(current_instruction[1]+0) % 16;
        //Obtains cpu_register number for OP2
        index[1] = *(current_instruction[1]+1) >> 4;
        //Obtains cpu_register number for result/target
        index[2] = *(current_instruction[1]+1) % 16;

        //Assigns proper cpu_register address to internal registers
        for(int i=0; i<sizeof(index)/sizeof(int); i++)
            reg[i] = &cpu_register[index[i]];
    }

    //Handler for branch instructions
    else if(handler_type==2){
        int index[2];

        //Obtains cpu_register number for OP1
        index[0] = *(current_instruction[1]+1) >> 4;
        //Obtains cpu_register number for OP2
        index[1] = *(current_instruction[1]+1) % 16;

        //Assigns proper cpu_register address to internal registers
        reg[0] = &cpu_register[index[0]];
        reg[1] = &cpu_register[index[1]];
    }

    //Handler for ls instructions
    else if(handler_type==3){
        int index[2];

        //Obtains cpu_register for ls
        index[0] = current_instruction[1][0] % 16;
        //Obtains cpu_register for address register
        index[1] = current_instruction[1][1] >> 4;

        //Assigns proper cpu_register to internal registers
        reg[0] = &cpu_register[index[0]];
        reg[1] = &cpu_register[index[1]];
    }
}