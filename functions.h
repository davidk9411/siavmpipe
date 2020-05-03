/*
Assignment 4 _ PIPED SIAVM
Made by David Kim
Last Modified: 04/21/2020
*/

#include "memory.h"

//Function indicator goes here

//Functions in print.c
void print_red();
void print_yellow();
void print_green();
void print_blue();
void print_reset();

//Functions in init.c
void init();
void print_vm_resource();
int load(char *file_name);

//Functions in run.c
void fetch();
int decode();
void get_registers(int **reg, int handler_type);
int decode_branch(int ** reg);
int decode_stack();

//Functions in basic.c
void halt_SIA();
void move();
void interrupt();
void addr();
void andr();
void divider();
void multiplyr();
void subtractr();
void orr();

//Functions in branch.c
void if_less();
void if_less_equal();
void if_equal();
void if_not_equal();
void if_greater();
void if_greater_equal();
void call_fn();
void jump();
void hadle_val(int reg_val, unsigned char *arr);

//Functions in store.c
void store_reg(int *target_reg, int val);
void store_mem(int addr, unsigned char *value);

//Function in stack.c
void return_R15();
void push_R15();
void pop_R15();
void store_memory_ops();
void load_memory_ops();

//Functions for memory stack, DO NOT MODIFY!!!
//Note that memory related function is prefixed with mem_
void mem_init(Memory *sys_mem);
int mem_is_empty(Memory *sys_mem);
int mem_push(Memory *sys_mem, unsigned char data);
int check_point(Memory *sys_mem);
unsigned char mem_search(Memory *sys_mem, int index);
int mem_wirte(Memory *sys_mem, unsigned char data, int index);