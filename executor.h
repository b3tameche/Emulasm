#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <vector>
#include <string>

enum InstructionType {
  ALU, LOAD, STORE, CALL, JMP, BRANCH, RET, PRINTF, FUNCTION
};

/* clears register and function maps and initializes special registers and virtual memory */
void init_executor();

/* prints insights of the program */
void print_insights(long tstart);

/* starts emulator */
void emulate(std::vector<std::string>& instructions);

/* determines the type of given instruction */
InstructionType resolveType(std::string& instr);

/* executes given instruction according to its type */
void execute(std::string& instr, std::vector<std::string>& instructions);

/* evaluates given expression, which can possibly represent ALU operation on registers and integers, 
   register itself, or integer */
int evaluate(std::string& right);

/* executes given instruction as ALU instruction */
void eALU(std::string& instr);

/* executes given instruction as LOAD instruction */
void eLOAD(std::string& instr);

/* executes given instruction as STORE instruction */
void eSTORE(std::string& instr);

/* executes given instruction as CALL instruction */
void eCALL(std::string& instr);

/* executes given instruction as BRANCH instruction */
void eBRANCH(std::string& instr);

/* executes given instruction as JMP instruction */
void eJMP(std::string& instr);

/* executes given instruction as RET instruction */
void eRET();

/* executes given instruction as FUNCTION declaration */
void eFUNCTION(std::string& instr, std::vector<std::string>& instructions);

/* executes given instruction as PRINTF function */
void ePRINTF(std::string& instr);

#endif