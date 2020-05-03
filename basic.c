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
extern int **reg_forwarder;
extern int *target_addr;
extern int immediate_val; 
extern void (*executer)();
extern void (*storer)(int*,int);

//OPCODE 0: HALT
void halt_SIA(){

    halt = TRUE;
    //reset buffers for next cycle
    free(current_instruction[2]);
    executer = NULL;
}

//OPCODE 1: ADD
void addr(){
     
    //Generates internal register
    int *internal_reg[3];
    //Copies data from register forwarder
    memcpy(internal_reg,reg_forwarder,sizeof(internal_reg));
    
    //Assigns target data for store action
    immediate_val = *internal_reg[0] + *internal_reg[1];
    target_addr = internal_reg[2];
    //Assign proper store action for next cycle
    storer=store_reg;
    
    //reset buffers for next cycle
    free(current_instruction[2]);
    free(reg_forwarder);
    executer = NULL;
    
    return;
}

//OPCODE 2: AND
void andr(){

    //Generates internal register
    int *internal_reg[3];
    //Copies data from register forwarder
    memcpy(internal_reg,reg_forwarder,sizeof(internal_reg));
    
    //Assigns target data for store action
    immediate_val = *internal_reg[0] & *internal_reg[1];
    target_addr = internal_reg[2];
    //Assign proper store action for next cycle
    storer=store_reg;

    //reset buffers for next cycle
    free(current_instruction[2]);
    free(reg_forwarder);
    executer = NULL;

    return;
}

//OPCODE 3: DIVIDE
void divider(){

    //Generates internal register
    int *internal_reg[3];
    //Copies data from register forwarder
    memcpy(internal_reg,reg_forwarder,sizeof(internal_reg));
    
    //Assigns target data for store action
    immediate_val = *internal_reg[0] / *internal_reg[1];
    target_addr = internal_reg[2];
    //Assign proper store action for next cycle
    storer=store_reg;

    //reset buffers for next cycle
    free(current_instruction[2]);
    free(reg_forwarder);
    executer = NULL;

    return;
}

//OPCODE 4: MULTIPLY
void multiplyr(){


    //Generates internal register
    int *internal_reg[3];
    //Copies data from register forwarder
    memcpy(internal_reg,reg_forwarder,sizeof(internal_reg));
    
    //Assigns target data for store action
    immediate_val = *internal_reg[0] * (*internal_reg[1]);
    target_addr = internal_reg[2];
    //Assign proper store action for next cycle
    storer=store_reg;

    //reset buffers for next cycle
    free(current_instruction[2]);
    free(reg_forwarder);
    executer = NULL;
    return;
}

//OPCODE 5: SUBTRACT
void subtractr(){

    //Generates internal register
    int *internal_reg[3];
    //Copies data from register forwarder
    memcpy(internal_reg,reg_forwarder,sizeof(internal_reg));
    
    //Assigns target data for store action
    immediate_val = *internal_reg[0] - (*internal_reg[1]);
    target_addr = internal_reg[2];
    //Assign proper store action for next cycle
    storer=store_reg;

    //reset buffers for next cycle
    free(current_instruction[2]);
    free(reg_forwarder);
    executer = NULL;
    return;
}

//OPCODE 6: OR
void orr(){

    //Generates internal register
    int *internal_reg[3];
    //Copies data from register forwarder
    memcpy(internal_reg,reg_forwarder,sizeof(internal_reg));
    
    //Assigns target data for store action
    immediate_val = *internal_reg[0] | (*internal_reg[1]);
    target_addr = internal_reg[2];    
    //Assign proper store action for next cycle
    storer=store_reg;

    //reset buffers for next cycle
    free(current_instruction[2]);
    free(reg_forwarder);
    executer = NULL;
    return;
}

//OPCODE 11: MOVE
void move(){

    //Isolates register number from first byte of instruction
    int reg_indicator = *(current_instruction[2]+0) % 16;

    /*
    Obtains immediate value,
    note that type of register is unsigned char
    in order to use signed value, typecast to char is required
    */
   int immediate_value = (char)(*(current_instruction[2]+1));
   //Assigns immediate value to variable to store ops
   target_addr = &cpu_register[reg_indicator];
   immediate_val = immediate_value;
   
    //reset buffers for next cycle
    free(current_instruction[2]);
    executer = NULL;

   //Assign proper operation to storer pointer
   storer = store_reg;
}

//OPCODE 12: INTERRUPT
void interrupt(){

    //Condition check
    if(*(current_instruction[2]+1)==0){
        //Prints registers
        print_green();
        puts("REGISTER INFORMATION: DECIMAL(HEX)");
        for(int i=0; i<sizeof(cpu_register)/sizeof(int); i++){
            cpu_register[i] == DEFAULT_VAL ? printf("R%d:\t%s\n",i,"NULL") : printf("R%d:\t%010d(%04X)\n",i,cpu_register[i],cpu_register[i]);
        }
        print_reset();
    }
    else{
        print_blue();
        puts("MEMORY INFORMATION: DECIMAL(HEX)");
        for(int i=0; i<mem_SIZE; i++){
            unsigned char mem_val = mem_search(&sys_memory,i);
            mem_val == 0xff ? printf("M%d:\t%s\n",i,"NULL") : printf("M%d:\t%03d(%02X)\n",i,mem_val,mem_val);
        }
        print_reset();
    }
    
    //reset buffers for next cycle
    free(current_instruction[2]);
    executer = NULL;
}

