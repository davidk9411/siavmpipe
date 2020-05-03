/*
Assignment 4 _ PIPED SIAVM
Made by David Kim
Last Modified: 04/21/2020
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <libgen.h>

//Personally defined headers
#include "constants.h"
#include "functions.h"

//External Global Variables
extern Memory sys_memory;
extern int cpu_register[NUM_REG];
extern int halt;
extern int mem_counter;
extern int fetch_more;
extern void (*executer)();
extern void (*storer)(int,int);
extern void (*sotrer_mem)(int,unsigned char*);

//Initializes system resource
void init(){

	printf("Initializing SIA Virtual Machine\n");
	printf("%s\t", "Initializing System Memory..." );
	//Checks initial memory condition
	while(mem_is_empty(&sys_memory)==FALSE){

		//Initializes memory
		mem_init(&sys_memory);
	}
	printf("OK!!\n");

	printf("%s\t", "Initializing System Registers..." );
	for(int index=0; index<NUM_REG-1; index++){
		/*
		Note that I am initializing each register to smallest value of integer
		in order to avoid error resulted from possible value that each register can hold
		MOVE ops can only assign signed 8 bit value (-127 to 128) 
		*/
		cpu_register[index]=INT_MAX;
	}

	//sets R15 to default value
	cpu_register[15] = 1020;

	//Initializes function pointers
	executer = NULL;
	storer = NULL;
	sotrer_mem = NULL;

	printf("OK!!\n");


}

//Prints system specification
void print_vm_resource(){

	//const int mem_size = sizeof(memory)/sizeof(unsigned char);
	print_yellow();
	printf("SIA Virtual Machine System Specification\n");
	printf("Memory: %lu Bytes\n", sizeof(sys_memory.address)/sizeof(unsigned char));
	printf("Registers: %lu registers & 1 register = %lu bytes \n", sizeof(cpu_register)/sizeof(int), sizeof(int));
	print_reset();

	return;
}

//Loads from input file
int load(char *file_name){

	printf("Opening input file...\t");

	//Opens the file read-only binary mode
	FILE *input = fopen(file_name,"rb");
	
	//File open error checking and return error if not opened
	if(input == NULL){
		print_red();
		printf("\nERROR: UNABLE TO OPEN INPUT FILE\n");
		print_reset();
		return 1;
	}

	//File size check
	fseek(input,0L, SEEK_END);
	int file_size = ftell(input);

	printf("OK!!\n");
	print_yellow();
	printf("Input file: %s (%d Bytes)\n", basename(file_name), file_size);
	print_reset();
	printf("Loading input file to system memory...\t");



	fseek(input, 0, SEEK_SET);

	while(!feof(input)){
		/*
		Raw input data array.
		Note that typecast to unsigned char is required to avoid bitoverflow
		when handling hex values
		*/
		char raw[2];
		//Read data byte by byte
		fread(raw,1,1,input);
		
		//Test printf and typecast to unsigned char for bit overflow
		//printf("Converted: %02X\n", (unsigned char)(raw[0]));
		if(mem_push(&sys_memory,(unsigned char)raw[0])!=0){
			print_red();
			printf("\nERROR: UNABLE TO WRITE DATA ON MEMORY\n");
			print_reset();
			return 1;
		}

		//Stop if end of file
		if(check_point(&sys_memory)+1==file_size)
			break;
	}

	fclose(input);

	printf("OK!!\n");

	//while loop controller
	halt = FALSE;

	//fetching portion controller
	fetch_more=TRUE;

	//memory address counter for instruction decoding
	mem_counter=0;

	return 0;
}