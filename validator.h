#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>

/* validates given expression as valid register */
bool valid_register(std::string& expr);

/* validates given string as memory address */
bool valid_mem_addr(std::string& str);

/* validates given string as arithmetic operation on registers or integers */
bool valid_alu_operation(std::string& str);

/* validates given string as ALU instruction */
bool valid_alu_instr(std::string& instr);

/* validates given string as LOAD instruction */
bool valid_load_instr(std::string& instr);

/* validates given string as STORE instruction */
bool valid_store_instr(std::string& instr);

/* validates given string as CALL instruction */
bool valid_call_instr(std::string& instr);

/* validates given string as JUMP instruction */
bool valid_jmp_instr(std::string& instr);

/* validates given string as BRANCH instruction */
bool valid_branch_instr(std::string& instr);

/* validates given string as RET instruction */
bool valid_ret_instr(std::string& instr);

/* validates given string as function declaration */
bool valid_function_decl(std::string& instr);

/* validates given string as printf call */
bool valid_printf(std::string& instr);

/* validates given string as 'END_DEF' expression */
bool valid_end_def(std::string& instr);

#endif