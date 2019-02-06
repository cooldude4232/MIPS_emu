
#include "CPU.h"


CPU::CPU()
{
}

CPU::CPU(vector<uint32_t> program)
{
	_program = program;
}

void CPU::step()
{
	instruction inst;
	inst = decompile(_program[_pc]);
	switch (inst.opcode)
	{
	case 0x00:
		switch (inst.funct)
		{
		//add signed
		case 0x20:
			_registers[inst.rd] = _registers[inst.rt] + _registers[inst.rs];
			break;
		//add unsigned
		case 0x21:
			_registers[inst.rd] = static_cast<unsigned>(_registers[inst.rt]) + static_cast<unsigned>(_registers[inst.rs]);
			break;
		}
		break;
	//add immediate signed
	case 0x08:
		_registers[inst.rt] = _registers[inst.rs] + inst.IMM;
	}
}

//take in instruction and decompile into easy to use instruction struct
instruction CPU::decompile(uint32_t inst)
{
	instruction out;
	//isolate left 6 bits
	out.opcode = inst >> 26;

	//opcode of zero signifies R type
	if (out.opcode == 0)
	{
		//get rs bits
		out.rs = (inst << 6) >> 27;
		//get rt bits
		out.rt = (inst << 11) >> 27;
		//get rd bits
		out.rd = (inst << 16) >> 27;
		//get shamt bits
		out.shamt = (inst << 21) >> 27;
		//get funct bits
		out.funct = (inst << 26) >> 26;
	}
	//opcode of 2 or 3 signifies J type
	else if ((out.opcode == 0x02) || (out.opcode == 0x03))
	{

	}
	//all others signify I type
	else
	{
		//get rs bits
		out.rs = (inst << 6) >> 27;
		//get rt bits
		out.rt = (inst << 11) >> 27;
		//get rd bits
		out.IMM = (inst << 16) >> 16;
	}

	return out;
}

void CPU::printRegisters()
{
	
	cout << endl << "$v0: " << _registers[2] << "$v0: " << _registers[3] << endl;
	for (int i = 0; i < 4; i++)
	{
		cout << "$a" << i << ": " << _registers[i+4] << " ";
	}

	cout << endl;

	for (int i = 0; i < 8; i++)
	{
		cout << "$t" << i << ": " <<  _registers[i + 8] << " ";
	}

	cout << endl;

	for (int i = 0; i < 8; i++)
	{
		cout << "$s" << i << ": " << _registers[i + 16] << " ";
	}
}

CPU::~CPU()
{
}
