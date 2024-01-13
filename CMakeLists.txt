cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# fixes clang errors

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(parser)


set(_src_files
	main.cpp
	lexer.cpp
	)

set(_hdr_files
	lexer.hpp
	token.hpp
	)

add_executable(main ${_src_files} ${_hdr_files})
