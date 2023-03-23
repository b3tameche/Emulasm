#include <bits/stdc++.h>
#include <sys/stat.h>
#include <cstdlib>
#include <unistd.h>
#include "executor.h"

#define pb push_back

#define BASE_PATH ("./tests/")

using namespace std;

const int STACK_SIZE = 1024;

vector<string> Instructions;            // <one line containing single instruction>

void obtain_file(ifstream& f) {
  string filename;
  cout << "File Name Containing Instructions: ";
  getline(cin, filename); cout << endl;

  string path = BASE_PATH + filename;

  struct stat buff;
  if (stat(path.c_str(), &buff) == 0) f = ifstream(path);
  else {
    cout << "File with given path doesn't exist (make sure a file is located in 'tests' directory), Aborting...";
    exit(1);
  }
}

void process_file(ifstream& f, vector<string>& instructions) {
  string instr;
  while(getline(f, instr)) {
    if (instr.find("PRINTF") != string::npos) {
      string whitespaces = " \t\n\r\v\f";
      instr.erase(0, instr.find_first_not_of(whitespaces)); // ltrim
      int start_inside = instr.find_first_of("F")+1;
      instr.erase(start_inside, instr.find_first_of("<")-start_inside); // "squeeze"
      instr.erase(instr.find_last_not_of(whitespaces)+1); // rtrim
    } else instr.erase(remove_if(instr.begin(), instr.end(), ::isspace), instr.end());
    
    if (instr.length() > 0) instructions.pb(instr);
  }

  f.close();
}

int main() {
  ifstream f;
  obtain_file(f);
  process_file(f, Instructions);

  init_executor();

  emulate(Instructions);
}