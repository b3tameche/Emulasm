#include <vector>
#include <iostream>
#include <unordered_map>
#include <bitset>
#include "executor.h"
#include "validator.h"

#define INSTR_SIZE (sizeof(int))
#define INCREMENT_PC (Registers["PC"] += INSTR_SIZE)
#define endl ("\n")

using namespace std;

const int STACK_SIZE = 1024;

unordered_map<string, int> Registers;   // <register, its value>
unordered_map<string, int> Functions;   // <function name, its address>
char* M;                                // virtual memory
int max_mem;                            // to track memory usage

void init_executor() {
  Registers.clear();
  Functions.clear();
  M = (char*) malloc(STACK_SIZE);

  Registers["SP"] = STACK_SIZE;         // [S]tack [P]ointer
  Registers["PC"] = 0;                  // [P]rogram [C]ounter
  Registers["RV"] = 0;                  // [R]eturn [V]alue
}

void print_insights(long tstart) {
  cout << "\033[1;32mProgram reached the end, here are the registers and their values so far:" << endl << endl;
  
  for (pair<string, int> reg : Registers) {
    bitset<32> x(reg.second);
    cout << reg.first << ": " << x;
    if (reg.first == "SP" || reg.first == "PC" || reg.first == "RV") cout << " " << reg.second;
    cout << endl;
  } cout << "\033[0m" << endl;

  cout << "\033[1;36mMax memory usage: " << max_mem << " bytes." << endl;
  printf("Time elapsed: %.2fms\033[0m\n\n", (double)(clock() - tstart)/CLOCKS_PER_SEC*1000);
}

void emulate(vector<string>& instructions) {
  clock_t start = clock();
  
  while (Registers["PC"] / 4 < instructions.size()) {
    string instr = instructions[Registers["PC"] / 4];
    execute(instr, instructions);

    if (Registers["SP"] < 0) {
      cout << "STACK OVERFLOW, Instr: " << instr << ", Aborting..." << endl; exit(1);
    }
  }

  print_insights(start);
  free(M);
}

InstructionType resolveType(string& instr) {
  if (valid_function_decl(instr)) return FUNCTION;
  if (valid_printf(instr)) return PRINTF;
  if (valid_alu_instr(instr)) return ALU;
  if (valid_load_instr(instr)) return LOAD;
  if (valid_store_instr(instr)) return STORE;
  if (valid_call_instr(instr)) return CALL;
  if (valid_jmp_instr(instr)) return JMP;
  if (valid_branch_instr(instr)) return BRANCH;
  if (valid_ret_instr(instr)) return RET;
  
  cout << "Illegal instruction type detected: '" << instr << "', Aborting..." << endl;
  exit(1);
}

int evaluate(string& exp) { // 6, R1, R1 + R2, R1, R1 + 6, 6 + R2
  if (Registers.find(exp) != Registers.end()) return Registers[exp];

  int signdex = -1;
  char sign;
  for (int i = 0; i < exp.length(); i++) {
    if (exp[i] == '+' || exp[i] == '-' || exp[i] == '*' || exp[i] == '/') {
      signdex = i; sign = exp[i]; break;
    }
  }

  if (signdex == -1) return stoi(exp);

  string l = exp.substr(0, signdex);
  string r = exp.substr(signdex+1);

  int a, b;
  try {
    a = stoi(l);
  } catch(const invalid_argument& e) {
    a = Registers[l];
  }

  try {
    b = stoi(r);
  } catch(const invalid_argument& e) {
    b = Registers[r];
  }

  switch (sign) {
    case '+': return a+b;
    case '-': return a-b;
    case '*': return a*b;
    case '/': return a/b;
  }

  return -1;
}

void execute(string& instr, vector<string>& instructions) {
  InstructionType type = resolveType(instr);

  cout << "Instr: " << instr << ", PC = " << Registers["PC"] << ", SP = " << Registers["SP"] << endl;
  switch (type) {
    case FUNCTION: eFUNCTION(instr, instructions); break;
    case PRINTF: ePRINTF(instr); break;
    case ALU: eALU(instr); break;
    case LOAD: eLOAD(instr); break;
    case STORE: eSTORE(instr); break;
    case BRANCH: eBRANCH(instr); break;
    case CALL: eCALL(instr); break;
    case JMP: eJMP(instr); break;
    case RET: eRET(); break;
  }

  int current_usage = STACK_SIZE - Registers["SP"];
  // cout << "\033[1;36mCurrent memory usage: " << current_usage << "\033[0m" << endl;
  if (current_usage > max_mem) max_mem = current_usage;
}

void eALU(string& instr) {
  int eqIndex = instr.find("=");
  
  string left = instr.substr(0, eqIndex);
  string right = instr.substr(eqIndex + 1);

  int value;
  if (right[0] == '.') {
    string exp = right.substr(2);
    value = evaluate(exp);
    int castSize = right[1] - '0';

    cout << "Executing ALU operation, writing " << castSize << " bytes of " << value << " in register '" << left << "'." << endl << endl;;

    INCREMENT_PC;

    switch (castSize) {
      case 1: Registers[left] = (unsigned char) value; return;
      case 2: Registers[left] = (unsigned short) value; return;
      case 4: Registers[left] = (unsigned int) value; return;
    }
  }

  value = evaluate(right);

  cout << "Executing ALU operation, writing " << value << " in register '" << left << "'." << endl << endl;
  Registers[left] = value;

  INCREMENT_PC;
}

void eLOAD(string& instr) {
  int eqIndex = instr.find("=");

  string left = instr.substr(0, eqIndex);
  string right = instr.substr(eqIndex + 1);

  int fbr = right.find('['), sbr = right.find(']');
  string inside = right.substr(fbr+1, sbr-fbr-1);
  int offset = evaluate(inside);

  if (right[0] == '.') {
    int castSize = right[1] - '0';

    cout << "Executing LOAD operation, loading " << castSize << " bytes from memory with offset of '" << offset << "' into " << left << endl << endl;

    INCREMENT_PC;

    switch (castSize) {
      case 1: Registers[left] = *((u_char*)(&M[offset])); return;
      case 2: Registers[left] = *((ushort*)(&M[offset])); return;
      case 4: Registers[left] = *((uint*)(&M[offset])); return;
    }
  }

  cout << "Executing LOAD operation, loading value from memory with offset of '" << offset << "' into " << left << endl << endl;

  Registers[left] = *((uint*)(&M[offset]));

  INCREMENT_PC;
}

void eSTORE(string& instr) {
  int eqIndex = instr.find("=");
  string left = instr.substr(0, eqIndex);
  string right = instr.substr(eqIndex + 1);

  int fbr = left.find('['), sbr = left.find(']');
  string inside = left.substr(fbr+1, sbr-fbr-1);

  int offset = evaluate(inside);
  int value;

  if (right[0] == '.') {
    string expr = right.substr(2);
    value = evaluate(expr);
    int castSize = right[1] - '0';

    cout << "Executing STORE operation, storing " << castSize << " bytes of " << value << " in memory with offset of '" << offset << "'." << endl << endl;

    INCREMENT_PC;

    switch (castSize) {
      case 1: *((u_char*)(&M[offset])) = value; return;
      case 2: *((ushort*)(&M[offset])) = value; return;
      case 4: *((uint*)(&M[offset])) = value; return;
    }
  }

  value = evaluate(right);

  cout << "Executing STORE operation, storing " << value << " in memory with offset of '" << offset << "'." << endl << endl;

  *((uint*)(&M[offset])) = value;

  INCREMENT_PC;
}

void eCALL(string& instr) {
  int l = instr.find('<'), r = instr.find('>');
  
  if (l != string::npos) {
    int r = instr.find('>');
    string func = instr.substr(l+1, r-l-1);
    
    if (Functions.find(func) == Functions.end()) {
      cout << "Invalid function name: '" << instr << "', Aborting..." << endl;
      exit(1);
    }

    Registers["SP"] -= INSTR_SIZE; // to remember return address
    *(uint*) &M[Registers["SP"]] = Registers["PC"]; // remembering return address

    int addr = Functions[func]; // load function address
    Registers["PC"] = addr + INSTR_SIZE; // set PC register to function address to continue execution from there

    cout << "Calling function '" << func << "', setting PC register to '" << Registers["PC"] << "'." << endl << endl;
    return;
  }

  string reg = instr.substr(4);
  int func_addr = evaluate(reg);

  Registers["SP"] -= INSTR_SIZE; // to remember return address
  *(uint*) &M[Registers["SP"]] = Registers["PC"]; // remembering return address
  Registers["PC"] = func_addr + INSTR_SIZE; // set PC register to function address to continue execution from there
}

void eRET() {
  Registers["PC"] = *((uint*)(&M[Registers["SP"]])); // load saved PC address inside PC register
  Registers["SP"] += INSTR_SIZE; // restore SP register

  cout << "Returning to " << Registers["PC"] << " with 'RV' register set to " << Registers["RV"] << endl << endl;

  INCREMENT_PC;
}

void eBRANCH(string& instr) {
  int fcomma = instr.find(',');
  int scomma = instr.find(',', fcomma+1);

  string f = instr.substr(3, fcomma-3);
  string s = instr.substr(fcomma+1, scomma-fcomma-1);

  int fvalue = evaluate(f);
  int svalue = evaluate(s);

  string fthree = instr.substr(0, 3);

  bool check = (fthree == "BLT" && fvalue < svalue) || (fthree == "BLE" && fvalue <= svalue)
            || (fthree == "BGT" && fvalue > svalue) || (fthree == "BGE" && fvalue >= svalue)
            || (fthree == "BNE" && fvalue != svalue) || (fthree == "BEQ" && fvalue == svalue);

  string jumpstring = "JMP" + instr.substr(scomma+1);
  if (check) eJMP(jumpstring);
  else { cout << "Branch failed..." << endl << endl; INCREMENT_PC; }
}

void eJMP(string& instr) {
  if (instr.find('-') == string::npos && instr.find('+') == string::npos) {
    int value = stoi(instr.substr(3));
    Registers["PC"] = value;
  } else {
    int sign = instr[5] == '-' ? -1 : 1;
    int size = sign * stoi(instr.substr(6));
    Registers["PC"] += size;
  }

  cout << "Jumping to address with value of " << Registers["PC"] << endl << endl;
}

void eFUNCTION(string& instr, vector<string>& instructions) {
  string func = instr.substr(0, instr.length() - 1); // without last ':' indicator
  Functions[func] = Registers["PC"];

  cout << "Function '" << func << "' was successfully stored." << endl << endl;

  while (!valid_end_def(instructions[Registers["PC"] / INSTR_SIZE])) {
    INCREMENT_PC;
  }

  INCREMENT_PC;
}

void ePRINTF(string& instr) {
  int l = instr.find('<'), r = instr.length() - 1;

  string inside = instr.substr(l+1, r-l-1);

  if (Registers.find(inside) != Registers.end()) {
    cout << "\033[1;33mPRINTF: " << inside << " = " << Registers[inside] << ";\033[0m" << endl << endl;
    INCREMENT_PC;
    return;
  }

  int constant;
  try {
    constant = stoi(inside);
  } catch(const invalid_argument& e) {
    if (valid_mem_addr(inside)) {
      string addr = inside.substr(2, inside.length() - 3);
      int offset = evaluate(addr);
      unsigned int value = *(unsigned int*) &M[offset];
      cout << "\033[1;33mPRINTF: " << inside << " = " << value << "\033[0m" << endl << endl;
      INCREMENT_PC;
      return;
    }

    cout << "\033[1;33mPRINTF: " << inside << "\033[0m" << endl << endl;
    INCREMENT_PC;
    return;
  }

  cout << "\033[1;33mPRINTF: " << constant << "\033[0m" << endl << endl;

  INCREMENT_PC;
}
