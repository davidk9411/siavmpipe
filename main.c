/*
Assignment 4 _ PIPED SIAVM
Made by David Kim
Last Modified: 04/21/2020
*/

#include <stdio.h>
#include <stdlib.h>

//Predefined header
#include "functions.h"
#include "constants.h"

//Global Variables(System resources)
Memory sys_memory;
int cpu_register[NUM_REG];
int halt;
int fetch_more;
int mem_counter;
unsigned char *current_instruction[3];
//Global Variables for store parameter
int *target_addr;
int immediate_val; 
int mem_addr;
unsigned char *ditributed_val;
//Global variable for internal variable handler
int **reg_forwarder;
//Pointer for executing functions
void (*executer)();
void (*storer)(int*,int);
void (*sotrer_mem)(int,unsigned char*);

int main(int argc, char *argv[]){

    //input argument chekcer
	if(argc!=2){
		print_red();
		printf("ERROR: NO INPUT FILE DETECTED\n");
		print_reset();
		printf("siavm filename\n");
		return 1;
	}

    //initializes system resource
	init();
	//prints system rexource in yellow color
	print_vm_resource();
    
   	//loads input file into system memory with error handler
	if(	load(argv[1]) != 0 )
		return 1;

	int cycle_counter = 0;

	do{
		//Store values if possible
		//Note that storer is pointer
		if(storer!=NULL)
			storer(target_addr,immediate_val);
		else if(sotrer_mem!=NULL){
			sotrer_mem(mem_addr,ditributed_val);
			free(ditributed_val);
		}
		//Execute instruction if possible
		//Note that executer is pointer
		if(executer!=NULL)
			executer();
		//Decodes instruction if possible
		decode();
		//Fetches instruction if possible
		fetch();
	}while(halt==FALSE);

	puts("DONE");

    exit(0);
}