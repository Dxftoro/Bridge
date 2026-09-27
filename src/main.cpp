#include <print>
#include <vector>
#include "igca/virtual_machine.h"

using I = igca::Instruction32;

int main() {
	igca::VirtualMachine* vm = new igca::VirtualMachine(128);
	
	std::vector<igca::Instruction32> program({
		I(igca::Opcode::ADDRV, igca::Register::RAX, 45),
		I(igca::Opcode::ADDRV, igca::Register::RAX, 45),
		I(igca::Opcode::PINW, igca::Register::RAX, igca::Register::RAX)
	});

	vm->execute(program.data(), program.size());

	delete vm;
	return 0;
}