#include <iostream>
#include <fstream>
#include <vector>
#include "Parser.hpp"

std::vector<String> readToString(const String& pathName);

int main() {

    String myfile = ("../miniLang.txt");
    std::vector<String> filecontents = readToString(myfile);

    std::shared_ptr<Parser> myParse = std::make_shared<Parser>(filecontents);
    //myParse->Testlexer(); //Used to test Lexer implementation.

    myParse->BeginParse(); //Parsing phase
    //myParse->XMLPass(); //XML phase
    myParse->SemanticPass(); //Semantic Analysis phase
    myParse->InterpretPass(); //Interpreter phase

    return 0;
}

std::vector<String> readToString(const String& pathName) //file input
{   
    std::vector<String> result;
    String input;
    std::fstream file;
    file.open(pathName);

    if(file.fail())
    {
        std::cout << "Error: File could not open" <<std::endl;
        exit(-1);
    }

    while(getline(file, input))
    {
        result.push_back(input);
    }
    
    file.close();
    return result;
}