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
extern void (*storer)(int,int);
extern void (*sotrer_mem)(int,unsigned char*);



//Store function for register
void store_reg(int *target_reg, int val){

    *target_reg = val;
    storer = NULL;
}

//Store function for memory
void store_mem(int addr, unsigned char *value){

    //Write to memroy (PUSH)
    for(int i=0; i<4; i++)
        mem_wirte(&sys_memory,value[i],addr+i);

    sotrer_mem = NULL;
}