#pragma once
#include <vector>
using namespace std;

struct instruction {
	uint8_t opcode;
	uint8_t rs;
	uint8_t rt;
	uint8_t rd;
	uint8_t shamt;
	uint8_t funct;
	uint16_t IMM;
	uint32_t address;
};

class CPU
{
public:
	CPU();
	CPU(vector<uint32_t> program);
	instruction decompile(uint32_t);
	~CPU();
private:
	int _pc;
	vector<uint32_t> _program;
	uint32_t _registers[32];
};

