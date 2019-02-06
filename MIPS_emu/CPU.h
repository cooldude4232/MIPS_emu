#pragma once
#include <vector>
#include <iostream>
using namespace std;

struct instruction {
	uint8_t opcode;
	uint8_t rs;
	uint8_t rt;
	uint8_t rd;
	int8_t shamt;
	uint8_t funct;
	int16_t IMM;
	uint32_t address;
};

class CPU
{
public:
	CPU();
	CPU(vector<uint32_t> program);
	instruction decompile(uint32_t);
	void printRegisters();
	void step();
	~CPU();
private:
	int _pc = 0;
	vector<uint32_t> _program;
	int32_t _registers[32];
};

