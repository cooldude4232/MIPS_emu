// MIPS_emu.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "CPU.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
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

	cout << "successfully read file!";

	vCPU.step();

	vCPU.printRegisters();

	system("PAUSE");
	return 0;
}

vector<uint32_t> convertHex(ifstream &file)
{
	vector<uint32_t> program;

	uint8_t opcode;
	char type;
	uint8_t funct;

	string t;
	while (getline(file, t))
	{
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
		}
		else if (field == "blez")
		{
			type = 'I';
			opcode = 0x06;
			funct = 0x00;
		}
		else if (field == "bne")
		{
			type = 'I';
			opcode = 0x05;
			funct = 0x00;
		}
		else if (field == "bgtz")
		{
			type = 'I';
			opcode = 0x07;
			funct = 0x00;
		}
		else if (field == "div")
		{
			type = 'R';
			opcode = 0x00;
			funct = 0x1A;
		}
		//assume anything else is a function definition
		else
		{

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
				uint8_t shamt = stoi(field);
				instruction += (shamt << 6);
			}

			instruction += funct;

			//add the instruction to our program
			program.push_back(instruction);
		}
		else if (type == 'I')
		{
			inst >> field;
			uint8_t rt = convertRegister(field);
			inst >> field;
			uint8_t rs = convertRegister(field);
			inst >> field;
			uint16_t IMM = stoi(field);


			uint32_t instruction = 0;

			instruction += (opcode << 26);

			instruction += (rs << 21);

			instruction += (rt << 16);

			instruction += IMM;

			//add the instruction to our program
			program.push_back(instruction);
		}
		else if (type == 'J')
		{

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
