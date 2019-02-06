// MIPS_emu.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "CPU.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <time.h>
#define $v0 2
#define $v1 3
using namespace std;

vector<uint32_t> convertHex(ifstream &file);
uint8_t convertRegister(string reg);

int main()
{
	ifstream file;
	file.open("code.txt");
	vector<uint32_t> program;
	//convert our input to hex format
	program = convertHex(file);
	//create a cpu object with our program
	CPU vCPU(program);
	clock_t tStart = clock();
	while (vCPU.step());

	cout << "end of program reached" << endl;
	printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);

	system("PAUSE");
	return 0;
}

vector<uint32_t> convertHex(ifstream &file)
{
	vector<uint32_t> program;
	//vectors to store functions and the corresponding locations
	vector<string> functions;
	vector<uint32_t> functionLoc;
	//for if instruction is branch or not

	//remember what instructions are branches to link up later
	vector<string> branches;
	vector<uint32_t> branchLoc;

	//track where we are when reading the program to link up jumps
	uint32_t pc = 0;
	uint8_t opcode;
	char type;
	uint8_t funct;

	string t;

	while (getline(file, t))
	{
		bool offset = false;
		bool isBranch = false;
		//replace commas with spaces
		for (int i = 0; i < t.length(); i++)
		{
			if (t[i] == ',')
			{
				t[i] = ' ';
			}
			if((t[i] == '(') || (t[i] == ')'))
			{
				offset = true;
				t[i] = ' ';
			}
		}

		istringstream inst(t);

		string field;

		inst >> field;

		if (field == "add")
		{
			type = 'R';
			opcode = 0x00;
			funct = 0x20;
		}
		else if (field == "addi")
		{
			type = 'I';
			opcode = 0x08;
			funct = 0x00;
		}
		else if (field == "addiu")
		{
			type = 'I';
			opcode = 0x09;
			funct = 0x00;
		}
		else if (field == "addu")
		{
			type = 'R';
			opcode = 0x00;
			funct = 0x21;
		}
		else if (field == "and")
		{
			type = 'R';
			opcode = 0x00;
			funct = 0x24;
		}
		else if (field == "andi")
		{
			type = 'I';
			opcode = 0x0C;
			funct = 0x00;
		}
		else if (field == "beq")
		{
			type = 'I';
			opcode = 0x04;
			funct = 0x00;
			isBranch = true;
		}
		else if (field == "blez")
		{
			type = 'I';
			opcode = 0x06;
			funct = 0x00;
			isBranch = true;
		}
		else if (field == "bne")
		{
			type = 'I';
			opcode = 0x05;
			funct = 0x00;
			isBranch = true;
		}
		else if (field == "bgtz")
		{
			type = 'I';
			opcode = 0x07;
			funct = 0x00;
			isBranch = true;
		}
		else if (field == "div")
		{
			type = 'R';
			opcode = 0x00;
			funct = 0x1A;
		}
		else if (field == "divu")
		{
			type = 'R';
			opcode = 0x00;
			funct = 0x1B;
		}
		else if (field == "j")
		{
			type = 'J';
			opcode = 0x00;
			funct = 0x1B;
		}
		else if (field == "syscall")
		{
			//syscall is special
			type = 'N';
			funct = 0x0C;
			uint32_t instruction = 0;
			instruction += funct;
			program.push_back(instruction);
		}
		else if (field == "ori")
		{
			type = 'I';
			opcode = 0x0D;
			funct = 0x00;
		}
		else if (field == "or")
		{
			type = 'R';
			opcode = 0x00;
			funct = 0x25;
		}
		else if (field == "sw")
		{
		type = 'I';
		opcode = 0x2B;
		funct = 0x00;
		}
		else if (field == "lw")
		{
		type = 'I';
		opcode = 0x23;
		funct = 0x00;
		}
		else if (field == "sub")
		{
		type = 'R';
		opcode = 0x00;
		funct = 0x22;
		}
		//assume anything else is a function definition
		else
		{
			//set type to none
			type = 'N';
			//if not comment
			if (field[0] != '#') {
				//if there is a colon
				if (field.back() == ':')
				{
					field.pop_back();
				}
				//store in an array to link up later
				functions.push_back(field);
				functionLoc.push_back(pc);
				//subtract from the counter and don't count as an instruction
			}
			pc--;
		}

		//switch for the type
		if (type == 'R')
		{

			inst >> field;
			uint8_t rd = convertRegister(field);
			inst >> field;
			uint8_t rs = convertRegister(field);
			inst >> field;
			uint8_t rt = convertRegister(field);


			uint32_t instruction = 0;

			instruction += (opcode << 26);

			instruction += (rs << 21);

			instruction += (rt << 16);

			instruction += (rd << 11);


			//optional fields
			if (inst >> field)
			{
				//check if comment
				if ((field[0] != ';') || (field[0] != '#'))
				{
					uint8_t shamt = stoi(field);
					instruction += (shamt << 6);
				}
			}

			instruction += funct;

			//add the instruction to our program
			program.push_back(instruction);
		}
		else if (type == 'I')
		{
			uint8_t rs;
			uint16_t IMM;
			inst >> field;
			uint8_t rt = convertRegister(field);
			if (offset)
			{
				inst >> field;
				IMM = stoi(field);
			}
			inst >> field;
			rs = convertRegister(field);
			
			inst >> field;
			
			if (!isBranch && !offset)
			{
				IMM = stoi(field);
			}
			uint32_t instruction = 0;

			instruction += (opcode << 26);

			instruction += (rs << 21);

			instruction += (rt << 16);

			if (offset)
			{
				instruction += IMM;
			}

			if (isBranch)
			{
				branches.push_back(field);
				branchLoc.push_back(pc);
			}
			else
			{
				instruction += IMM;
			}

			//add the instruction to our program
			program.push_back(instruction);
		}
		else if (type == 'J')
		{
			uint32_t instruction = 0;
			instruction += (opcode << 26);
			inst >> field;
			uint32_t address;
			address = stoi(field);
			instruction += address;
		}

		//advance the counter
		pc++;
	}
	//link up function branches
	for (int i = 0; i < branches.size(); i++)
	{
		//go through each branch location and replace with our function locations
		for (int j = 0; j < functions.size(); j++)
		{
			//if the branch function matches the function list
			if (branches[i] == functions[j])
			{
				//if it matches replace with relaative jump
				program[branchLoc[i]] += static_cast<uint16_t>(functionLoc[j] - branchLoc[i]);
			}
		}
	}

	return program;
}

uint8_t convertRegister(string reg)
{
	//remove commas
	if (reg.back() == ',')
	{
		reg.pop_back();
	}

	if (reg == "$zero")
	{
		return 0;
	}
	else if (reg == "$at")
	{
		return 1;
	}
	else if (reg == "$gp")
	{
		return 28;
	}
	else if (reg == "$sp")
	{
		return 29;
	}
	else if ((reg == "$fp") || (reg == "$s8"))
	{
		return 30;
	}
	else if (reg == "$ra")
	{
		return 31;
	}
	//if it doesn't match the other special registers
	else
	{
		switch (reg[1])
		{
			case 'v':
				return (reg[2] - '0' + 2);
				break;

			case 'a':
				return (reg[2] - '0' + 4);
				break;

			case 't':
				if((reg[2] - '0') < 8)
					return (reg[2] - '0' + 8);
				else
					return (reg[2] - '0' + 16);
				break;

			case 's':
				return (reg[2] - '0' + 16);
				break;

			case 'k':
				return (reg[2] - '0' + 26);
				break;
		}
	}
}
