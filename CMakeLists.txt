cmake_minimum_required(VERSION 3.20)
project(bridge)

set(SOURCES
	src/main.cpp
	"src/igca/util/memory.cpp"
	"src/igca/flag_bank.cpp" "src/igca/virtual_machine.cpp" )

add_executable(bridge ${SOURCES})
set_target_properties(bridge PROPERTIES CXX_STANDARD 23)