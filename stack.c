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
extern int fetch_more;
extern int mem_addr;
extern unsigned char *ditributed_val;
extern void (*sotrer_mem)(int,unsigned char*);

// OPERATION 0: RETURN
void return_R15(){

    //move R15
    cpu_register[15]+=4;

    //mem_counter=0;
    mem_counter=0;
    //pop process for return
    for(int i=0; i<sizeof(unsigned int); i++){
        mem_counter += mem_search(&sys_memory,cpu_register[15]+i) << (24-(8*i));
        mem_wirte(&sys_memory,0xff, cpu_register[15]+i);
    }

    //Frees the instruction memory
    free(current_instruction[2]);
    executer=NULL;
    //Buffer unlock
    fetch_more=TRUE;
}

// OPERATION 1: PUSH
void push_R15(){

    //Obtains register's value
    int register_val = cpu_register[current_instruction[2][0]%16];

    //Values to store
    unsigned char values[4];

    //Divide values by bytes
    hadle_val(register_val,values);

    //Saves data to buffer
    ditributed_val=(unsigned char *)malloc(sizeof(unsigned char)*4);
    memcpy(ditributed_val,values,sizeof(unsigned char)*4);
    mem_addr=cpu_register[15];

    //Assigns proper store action for next cycle
    sotrer_mem=store_mem;

    cpu_register[15]-=4;

    //Frees the instruction memory
    free(current_instruction[2]);
    executer=NULL;
    
}

// OPERATION 2: POP
void pop_R15(){

    //Saves data to buffer for store ops
    target_addr=&cpu_register[current_instruction[2][0]%16];

    //Move R15
    cpu_register[15]+=4;

    immediate_val=0;

    //Save data to buffer for store ops
    for(int i=0; i<sizeof(immediate_val); i++){
        immediate_val=mem_search(&sys_memory,cpu_register[15]+i) << (24-(8*i));
        mem_wirte(&sys_memory,0xff,cpu_register[15]+i);
    }

    //Assigns proper store ops for next cycle
    storer=store_reg;

   //Frees the instruction memory
    free(current_instruction[2]);
    executer=NULL;
}

// OPCODE 8: LOAD
void load_memory_ops(){

    //Internal register
    int *internal_reg[2];

    unsigned char value[4];

    //Saves data from buffer
    memcpy(internal_reg,reg_forwarder,sizeof(internal_reg));

    //calculate memory address
    unsigned int address = *internal_reg[1]+2*(current_instruction[2][1]%16);

    //read from memory
    for(int i=0; i<sizeof(value)/sizeof(unsigned char); i++)
        value[i] = mem_search(&sys_memory,address+i);

    //Saves data to buffer for store ops
    immediate_val = (value[0]<<24) + (value[1]<<16) + (value[2]<<8) + value[3];
    target_addr = internal_reg[0];
    //Assigns proper store ops for next cycle
    storer=store_reg;
}

//OPCODE 9: STORE
void store_memory_ops(){

    /*Internal registers
    0: register to store
    1: address register*/
    int *internal_reg[2];
    memcpy(internal_reg,reg_forwarder,sizeof(internal_reg));

    unsigned char values[4];

    //Calculates proper memory address
    unsigned int address = *internal_reg[1] + 2*(current_instruction[2][1]%16);

    //Obtains proper values
    hadle_val(*internal_reg[0], values);
    ditributed_val=(unsigned char *)malloc(sizeof(unsigned char)*4);
    //Copies data to buffer
    memcpy(ditributed_val,values,sizeof(unsigned char)*4);
    mem_addr = address;
    
    //Assigns proper store action for next cycle
    sotrer_mem=store_mem;

   //Frees the instruction memory
    free(current_instruction[2]);
    executer=NULL;
}