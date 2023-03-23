#include "validator.h"
#include <iostream>
#include <regex>

using namespace std;

bool valid_register(string& expr) { // Rn, RV, PC, SP
  if (expr == "PC" || expr == "SP" || expr == "RV") return true;
  if (expr[0] != 'R') return false;
  
  try {
    stoi(expr.substr(1));
  } catch(const invalid_argument& e) {
    return false;
  }

  return true;
}

bool valid_load_instr(string& instr) { // R1 = M[SP], R1 = M[SP+12], R1 = .2 M[SP]
  int eqIndex = instr.find('=');

  if (eqIndex == string::npos) return false;

  string lside = instr.substr(0, eqIndex);
  string rside = instr.substr(eqIndex+1);
  if (lside == "SP" || !valid_register(lside)) return false;

  if (rside[0] == '.') {
    if (rside[1] != '1' && rside[1] != '2' && rside[1] != '4') return false;
    string mem_addr = rside.substr(2);
    return valid_mem_addr(mem_addr);
  }

  return valid_mem_addr(rside);
}

bool valid_mem_addr(string& str) { // M[Register | Constant | valid_alu_operation]
  if (str[0] == 'M') {
    int l = str.find('['),  r = str.find(']');
    if (l != 1 || r != str.length()-1) return false;

    string inside = str.substr(l+1, r-l-1);
    if (valid_register(inside) || valid_alu_operation(inside)) return true;

    try {
      stoi(inside);
    } catch(const invalid_argument& e) {
      return false;
    }

    return true;
  }

  return false;
}

bool valid_alu_operation(string& str) { // (Register | Constant) [+-*/] (Register | Constant)
  int signdex = -1;
  for (int i = 0; i < str.length(); i++) {
    if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/') {
      signdex = i;
      break;
    }
  }

  if (signdex == -1) return false;

  string left = str.substr(0, signdex);
  string right = str.substr(signdex+1);

  try {
    stoi(left);
  } catch(const invalid_argument& e) {
    if (!valid_register(left)) return false;
  }
  
  try {
    stoi(right);
  } catch(const invalid_argument& e) {
    if (!valid_register(right)) return false;
  }

  return true;
}

bool valid_alu_instr(string& instr) { // Register = valid_alu_operation | Constant | casted Constant (.1 1025) | Register
  int eqIndex = instr.find('=');
  if (eqIndex == string::npos) return false;

  string left = instr.substr(0, eqIndex);
  string right = instr.substr(eqIndex+1);

  if (!valid_register(left)) return false;
  if (valid_register(right)) return true;

  if (right[0] == '.') {
    if (right[1] != '1' && right[1] != '2' && right[1] != '4') return false;
    
    string rexpr = right.substr(2);
    try {
      stoi(rexpr);
    } catch(const invalid_argument& e) {
      if (!valid_register(rexpr)) return false;
    }

    return true;
  }

  try {
    stoi(right);
  } catch(const invalid_argument& e) {
    return valid_alu_operation(right);
  }

  return true;
}

bool valid_store_instr(string& instr) { // M[Register | Constant | valid_alu_operation] = (Register | Constant | casted (Constant | Register))
  int eqIndex = instr.find('=');

  if (eqIndex == string::npos) return false;

  string left = instr.substr(0, eqIndex);
  string right = instr.substr(eqIndex+1);

  if (!valid_mem_addr(left)) return false;

  if (right[0] == '.') {
    if (right[1] != '1' && right[1] != '2' && right[1] != '4') return false;

    string rexpr = right.substr(2);
    try {
      stoi(rexpr);
    } catch(const invalid_argument& e) {
      if (!valid_register(rexpr)) return false;
    }

    return true;
  }

  try {
    stoi(right);
  } catch(const invalid_argument& e) {
    if (!valid_register(right)) return false;
  }
  
  return true;
}

bool valid_call_instr(string& instr) { // CALL (<function_name> | Register)
  int call_index = instr.find("CALL");
  
  if (call_index != 0) return false;

  int l = instr.find('<'), r = instr.find('>');

  if (l == string::npos && r == string::npos) {
    string reg = instr.substr(4);
    return (valid_register(reg));
  }

  return (l == 4 && r == instr.length()-1);
}

bool valid_jmp_instr(string& instr) { // JMP (valid_alu_operation | Constant)
  if (instr.find("JMP") != 0) return false;
  if (instr.length() <= 3) return false;

  string dest = instr.substr(3);

  try {
    if (stoi(dest) % 4 != 0) return false;
  } catch(const invalid_argument& e) {
    if (stoi(dest.substr(3)) % 4 != 0) return false;
    return valid_alu_operation(dest);
  }
  
  return true;
}

bool valid_branch_instr(string& instr) { // (Branch String), (Register | Constant),(Register | Constant), (valid_alu_operation | Constant)
  string fthree = instr.substr(0, 3);

  if (fthree != "BNE" && fthree != "BLT" && fthree != "BEQ" && fthree != "BGE" && fthree != "BGT" && fthree != "BLE") {
    return false;
  }

  int fcomma = instr.find(',');
  int scomma = instr.find(',', fcomma+1);

  string fexp = instr.substr(3, fcomma-3);
  string sexp = instr.substr(fcomma+1, scomma-fcomma-1);

  try {
    stoi(fexp);
  } catch(const invalid_argument& e) {
    if (!valid_register(fexp)) return false;
  }

  try {
    stoi(sexp);
  } catch(const invalid_argument& e) {
    if (!valid_register(sexp)) return false;
  }
  
  string rest = instr.substr(scomma+1);
  try {
    if (stoi(rest) % 4 != 0) return false;
  } catch(const invalid_argument& e) {
    if (stoi(rest.substr(3)) % 4 != 0) return false;
    return valid_alu_operation(rest);
  }

  return true;
}

bool valid_ret_instr(string& instr) { // RET
  return (instr == "RET");
}

bool valid_function_decl(string& instr) { // function_name:
  regex reg("([a-zA-Z_][a-zA-Z0-9_]*):");
  return regex_match(instr, reg);
}

bool valid_printf(string& instr) { // PRINTF<string which can also be identified as valid_mem_addr or Register>
  return instr.find("PRINTF<") == 0 && instr[instr.length() - 1] == '>';
}

bool valid_end_def(string& instr) { // END_DEF
  return instr == "END_DEF";
}
