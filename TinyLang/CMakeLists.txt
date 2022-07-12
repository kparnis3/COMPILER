cmake_minimum_required(VERSION 3.18)

project(COMPILER CXX)

add_executable(Tiny Main.cpp lexer.hpp lexer.cpp lexerTable.hpp Parser.hpp Parser.cpp ASTClasses.hpp VisitorDesign.hpp XMLVisitorDesign.cpp SemanticVisitorDesign.cpp SymTable.hpp SymTable.cpp InterpreterPass.cpp)