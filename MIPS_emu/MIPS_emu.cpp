// MIPS_emu.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;
vector<uint32_t> convertHex(ifstream &file);

int main()
{
	ifstream file;
	file.open("code.txt");
	convertHex(file);
	cout << "successfully read file!";
	system("PAUSE");
	return 0;
}

vector<uint32_t> convertHex(ifstream &file)
{
	uint8_t opcode;
	char type;
	uint8_t funct;

	string t;
	getline(file, t);
	istringstream inst(t);
	string field;
	while (inst >> field) {
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
		//assume anything else is a function definition
		else
		{

		}
	}
}