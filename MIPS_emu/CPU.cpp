
#include "CPU.h"


CPU::CPU()
{
}

CPU::CPU(vector<uint32_t> program)
{
}

//take in instruction and decompile into easy to use instruction struct
instruction CPU::decompile(uint32_t inst)
{
	instruction out;
	//isolate left 6 bits
	out.opcode == inst >> 26;
	//isolate left 6 bits
	out.opcode == inst >> 26;

	//opcode of zero signifies R type
	if (out.opcode == 0)
	{

	}
	//opcode of 3 or 4 signifies J type
	else if ((out.opcode == 0x02) || (out.opcode == 0x03))
	{

	}
	//all others signify I type
	else
	{

	}
}

CPU::~CPU()
{
}
