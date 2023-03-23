# Assembly Virtual Machine Emulator

Assembly Virtual Machine Emulator for subset of Reduced Instruction Set Computer (RISC) processor.
Syntax for this particular instruction set is simplified but logic and functionality remains the same.

## Usage
Clone the repository on your local machine.
```
git clone https://github.com/b3tameche/asm.git
```
To compile the files, you need to install g++ compiler.
```
sudo apt-get update
sudo apt-get install build-essential
```
Finally, compile the project and run.
```
g++ *.cpp
./a.out
```
## Notes and features
Here are instruction types and their syntax:
- **Load ->** *R1 = M[SP]*, reads bytes from memory to register
- **Store ->** *M[SP] = 15*, moves values from registers back out to memory
- **ALU ->** *R1 = 12 + 15*, Arithmetic Logic Unit (ALU) instructions, instructions which are executed by CPU ALU, such as arithmetic operations.
- **Branching ->** *BNE R1, R2, PC+32*, Branch instructions which compares register values and constants to each other and changes Program Counter accordingly.
- **CALL ->** *CALL \<printf\>* **|** Call instructions, making function calls with given function name or register in "<>" brackets.
- **JMP ->** *JMP PC-36* **|** Jump instructions, sets Program Counter to given value.
- **RET ->** *RET* **|** Return instruction, which returns Program Counter to function caller address to continue execution from there.
- **Func definition ->** *memcpy:* **|** Custom instruction, function definition, every function definition should end with END_DEF.
- **END_DEF ->** *END_DEF* **|** Custom instruction, implies end of a function definition.
- **PRINTF ->** *PRINTF\<register, M[address], or string\>* **|** Custom instruction which prints values of registers and memory addresses if provided, or strings in other cases.

> **Notes:**
> - Assignment operations (i.e. R1 = 16) are classified as ALU instructions by my decision.
> - Test files are provided in ```/tests``` directory with their corresponding functions in ```tests.cpp``` file. If you want to add your own test file, you have to add it to the ```/tests``` directory.
