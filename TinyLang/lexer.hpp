#include <string>
#include <map>
#include <stack>
#include <vector>
#include "lexerTable.hpp"
using String = std::string;

class Token{
    public:
        Token();
        ~Token();
        std::vector<MyTokens> type;
        std::vector<String> word;
        bool end = false; //check if we are at EOF
        //int printCounter;
        void printResult(); //helper function to print tokens
        int currLine{}; //save current line
        static String getTypeName(MyTokens type); //get the actual name of tokens
        
};

class Lexer{
    public:
        Lexer(const std:: vector<String>& input); //file input
        ~Lexer();
        std::map<String, MyTokens> Reserved; //used to store all reserved keywords in the form of a map
        Token getNextToken(); //lexers get next token
        static bool isAccepting(int state);
        
    private:
        std::vector<String> myData; //store file input
        void setReserved();
        MyTokens checkReserved(String word); //check if an identifier is actually a keyword
        int currentLine;
        int stringPtr; //points to current character in getNextToken()
};





