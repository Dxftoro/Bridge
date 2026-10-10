#include <print>
#include <vector>
#include "igca/virtual_machine.h"

using I = igca::Instruction32;

int main() {
	igca::VirtualMachine* vm = new igca::VirtualMachine(128);
	
	//std::vector<igca::Instruction32> program({
	//	I(igca::Opcode::MOVRV, igca::Register::RAX, 45),
	//	I(igca::Opcode::MOVRV, igca::Register::RDX, 2),
	//	I(igca::Opcode::MOVRR, igca::Register::RBX, igca::Register::RDX),
	//	I(igca::Opcode::MULRR, igca::Register::RAX, igca::Register::RBX),
	//	I(igca::Opcode::PINW, igca::Register::RAX, igca::Register::RAX)
	//	//I(igca::Opcode::JMP, 5)
	//});

	//std::vector<igca::Instruction32> program({
	//	I(igca::Opcode::MOVRV, igca::Register::RAX, 47),
	//	I(igca::Opcode::MOVRV, igca::Register::RBX, 45),
	//	I(igca::Opcode::CMPRR, igca::Register::RAX, igca::Register::RBX),
	//	I(igca::Opcode::JE, 5),
	//	I(igca::Opcode::JMP, 1000),
	//	I(igca::Opcode::PINW, igca::Register::RAX, igca::Register::RAX)
	//});

	std::vector<igca::Instruction32> program({
		I(igca::Opcode::MOVRV, igca::Register::RCX, 0),
		I(igca::Opcode::PINW, igca::Register::RCX, igca::Register::RCX),
		I(igca::Opcode::ADDRV, igca::Register::RCX, 1),
		I(igca::Opcode::CMPRV, igca::Register::RCX, 10),
		I(igca::Opcode::JNE, 1)
	});


	igca::ExecCode code = vm->execute(program.data(), program.size());

	delete vm;

	std::println("Program executed with code: {} ({})", static_cast<int>(code), igca::execCodeToString(code));
	return 0;
}