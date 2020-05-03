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

//TYPE 0: IFLESS
void if_less(){

    //Generates internal register
    int *internal_reg[2];
    //Copies data from register forwarder
    memcpy(internal_reg,reg_forwarder,sizeof(internal_reg));

    //IF LESS
    if(*internal_reg[0]<(*internal_reg[1])){

        //Memory offset calculation
        int offset = ((int)(current_instruction[2][2])<<8) + current_instruction[2][3];
        mem_counter += (2*offset);
    }
    //FALSE Statement
    else
        mem_counter += 4;

    //Frees the instruction memory
    free(current_instruction[2]);
    //Frees the register forwarder
    free(reg_forwarder);
    executer = NULL;
    fetch_more = TRUE;
}

//TYPE 1: IFLESSEQUAL
void if_less_equal(){
    

    //Generates internal register
    int *internal_reg[2];
    //Copies data from register forwarder
    memcpy(internal_reg,reg_forwarder,sizeof(internal_reg));

    //IF LESS
    if(*internal_reg[0]<=(*internal_reg[1])){

        //Memory offset calculation
        int offset = ((int)(current_instruction[2][2])<<8) + current_instruction[2][3];
        mem_counter += (2*offset);
    }
    //FALSE Statement
    else
        mem_counter += 4;

    //Frees the instruction memory
    free(current_instruction[2]);
    //Frees the register forwarder
    free(reg_forwarder);
    executer = NULL;
    fetch_more = TRUE;
}

//TYPE 2: IFEQUAL
void if_equal(){


    //Generates internal register
    int *internal_reg[2];
    //Copies data from register forwarder
    memcpy(internal_reg,reg_forwarder,sizeof(internal_reg));

    //IF LESS
    if(*internal_reg[0]==(*internal_reg[1])){

        //Memory offset calculation
        int offset = ((int)(current_instruction[2][2])<<8) + current_instruction[2][3];
        mem_counter += (2*offset);
    }
    //FALSE Statement
    else
        mem_counter += 4;

    //Frees the instruction memory
    free(current_instruction[2]);
    //Frees the register forwarder
    free(reg_forwarder);
    executer = NULL;
    fetch_more = TRUE;
}

//TYPE 4: IFNOTEQUAL
void if_not_equal(){


    //Generates internal register
    int *internal_reg[2];
    //Copies data from register forwarder
    memcpy(internal_reg,reg_forwarder,sizeof(internal_reg));

    //IF LESS
    if(*internal_reg[0]!=(*internal_reg[1])){

        //Memory offset calculation
        int offset = ((int)(current_instruction[2][2])<<8) + current_instruction[2][3];
        mem_counter += (2*offset);
    }
    //FALSE Statement
    else
        mem_counter += 4;

    //Frees the instruction memory
    free(current_instruction[2]);
    //Frees the register forwarder
    free(reg_forwarder);
    executer = NULL;
    fetch_more = TRUE;
}

//TYPE 5: IFGREATER
void if_greater(){


    //Generates internal register
    int *internal_reg[2];
    //Copies data from register forwarder
    memcpy(internal_reg,reg_forwarder,sizeof(internal_reg));

    //IF LESS
    if(*internal_reg[0]>(*internal_reg[1])){

        //Memory offset calculation
        int offset = ((int)(current_instruction[2][2])<<8) + current_instruction[2][3];
        mem_counter += (2*offset);
    }
    //FALSE Statement
    else
        mem_counter += 4;

    //Frees the instruction memory
    free(current_instruction[2]);
    //Frees the register forwarder
    free(reg_forwarder);
    executer = NULL;
    fetch_more = TRUE;
}

//TYPE 6: IFGREATEREQUAL
void if_greater_equal(){

    //Generates internal register
    int *internal_reg[2];
    //Copies data from register forwarder
    memcpy(internal_reg,reg_forwarder,sizeof(internal_reg));

    //IF LESS
    if(*internal_reg[0]>=(*internal_reg[1])){

        //Memory offset calculation
        int offset = ((int)(current_instruction[2][2])<<8) + current_instruction[2][3];
        mem_counter += (2*offset);
    }
    //FALSE Statement
    else
        mem_counter += 4;

    //Frees the instruction memory
    free(current_instruction[2]);
    //Frees the register forwarder
    free(reg_forwarder);
    executer = NULL;
    fetch_more = TRUE;
}

//TYPE 7: CALL
void call_fn(){

    unsigned int address = 0;

    //Addes up bits of address from current register
    for(int i=1; i<=3; i++){
        address+=(current_instruction[2][i])<<(24-8*i);
    }

    //Values to store
    unsigned char values[4];

    //Divide values by bytes
    hadle_val(mem_counter+4,values);

    //Saves data to the buffer
    ditributed_val=(unsigned char *)malloc(sizeof(unsigned char)*4);
    memcpy(ditributed_val,values,sizeof(unsigned char)*4);
    mem_addr=cpu_register[15];

    //Assign proper store action for next cycle
    sotrer_mem=store_mem;    

    //Move R15 to next location
    cpu_register[15]-=4;

    //Multiplies 2 to jump specified location
    address*=2;

    //Indicates next instruction location address
    mem_counter = address;

    //Frees the instruction memory
    free(current_instruction[2]);
    executer = NULL;
    fetch_more = TRUE;
}

//TYPE 7: JUMP
void jump(){


    unsigned int address=0;

    //Addes up bits of address from current register
    for (int i=2; i<4; i++)
        address+=current_instruction[2][i];

    //Multiplies 2 to jump specified location
    address*=2;

    //Indicates next instruction location address
    mem_counter = address;

    //Frees the instruction memory
    free(current_instruction[2]);
    executer = NULL;
    fetch_more = TRUE;

}
//Value devider
void hadle_val(int reg_val, unsigned char *arr){

    arr[0] = reg_val>>24;
    arr[1] = (reg_val>>16)%256;
    arr[2] = (reg_val>>8)%256;
    arr[3] = reg_val%256;
}