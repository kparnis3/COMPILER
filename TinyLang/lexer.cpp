#include <iostream>
#include <fstream>
#include <vector>
#include "Parser.hpp"

Lexer::Lexer(const std::vector<String>& input)
{
    setReserved(); // Initilizing reserved words
    stringPtr = 0; // Pointer to current word being tokenized
    currentLine = 0;

    for (const String& i : input) //Each line of code being inserted to myData
        myData.push_back(i);

    std::cout << "Lexer input:" << std::endl;
    std::cout << " " << std::endl;
    for (const String& i : myData)
        std::cout << i << std::endl;
    std::cout << " " << std::endl;
}

Token::Token() = default; // Token constructor

String Token::getTypeName(MyTokens type) //used to get the string version of token names
{
    switch(type)
    {
        case tLET: return "tLET";
        case tIDENTIFY: return "tIDENTIFY";
        case tSPACE: return "tSPACE";
        case tLCURLY: return "tLCURLY";
        case tRCURLY: return "tRCURLY";
        case tLBRACKET: return "tLBRACKET";
        case tRBRACKET: return "tRBRACKET";
        case tDIGIT: return "tDIGIT";
        case tFLOAT: return "tFLOAT";
        case tLESSTHAN: return "tLESSTHAN";
        case tLORE: return "tLORE";
        case tGREATER: return "tGREATER";
        case tGORE: return "tGORE";
        case tEQUAL: return "tEQUAL";
        case tRELA: return "tRELA";
        case tNOTE: return "tNOTE";
        case tCOMMA: return "tCOMMA";
        case tCOLON: return "tCOLON";
        case tSCOLON: return "tSCOLON";
        case tADD: return "tADD";
        case tSUBTRACT: return "tSUBTRACT";
        case tARROW: return "tARROW";
        case tSLit: return "tSlit";
        case tINT: return "tINT";
        case tPRINT: return "tPRINT";
        case tBOOLEAN: return "tBOOL";
        case tN: return "tNOT";
        case tIF: return "tIF";
        case tELSE: return "tELSE";
        case tFOR: return "tFOR";
        case tCHAR: return "tCHAR";
        case tFUNCTION: return "tFUNCTION";
        case tRETURN: return "tRETURN";

        default: return "Not yet implemented";
    }
}

void Token::printResult() //print the current line, name and token type
{   
    if(type[0]!=tSPACE)
        std::cout << "<" << currLine <<": " << word[0] << " :" << getTypeName(type[0]) << ">" << std::endl;
}



MyTokens Lexer::checkReserved(const String word) //check the reserved to see if identifier is actually a keyword
{
    auto find = Reserved.find(word);
    if(find == Reserved.end())
    {
        return tIDENTIFY;
    }
    else
    {
        return find->second;
    }
}

void Lexer::setReserved() //initilise all tokens as a map of strings and tokens
{
    Reserved.insert({"bool", tBOOLEAN});
    Reserved.insert({"int", tINT});
    Reserved.insert({"float", tFLOAT});
    Reserved.insert({"char", tCHAR});
    Reserved.insert({"true", tTRUE});
    Reserved.insert({"false", tFALSE});
    Reserved.insert({"let", tLET});
    Reserved.insert({"print", tPRINT});
    Reserved.insert({"return", tRETURN});
    Reserved.insert({"if", tIF});
    Reserved.insert({"else", tELSE});
    Reserved.insert({"for", tFOR});
    Reserved.insert({"while", tWHILE});
    Reserved.insert({"fn", tFUNCTION});
    Reserved.insert({"not", tN});
    Reserved.insert({"or", tOR});
    Reserved.insert({"and", tAND});
}

Lexer::~Lexer()
{
    //std::cout << "Lexer has been destroyed" << std::endl;
}

Token::~Token()
{
    //std::cout << "Token has been destroyed" << std::endl;
}

myCharacters getType(char c) // using ASCII table for characters
{
    if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
        return xLETTER;
    else if (c >= 48 && c <= 57)
        return xDIGIT;
    else if (c == 42)
        return xMULTIPLY;
    else if (c == 47)
        return xDIVIDE;
    else if (c == 45)
        return xSUBTRACT;
    else if (c == 43)
        return xADD;
    else if (c == 62)
        return xGREATER;
    else if (c == 60)
        return xLESS;
    else if (c == 61)
        return xEQUAL;
    else if (c == 33)
        return xNOT;

    else if (c == 95)
        return xUNDERSCORE;
    else if (c == 44)
        return xCOMMA;
    else if (c == 46)
        return xDOT;
    else if (c == 40)
        return xLBRACKET;
    else if (c == 41)
        return xRBRACKET;
    else if (c == 59)
        return xSCOLON;
    else if (c == 58)
        return xCOLON;
    else if (c == 32)
        return xSPACE;
    else if (c == 123)
        return xLCURLY;
    else if (c == 125)
        return xRCURLY;
    else if (c == 34)
        return xQUOTE;
    else if (c == '\n')
        return xNL;

    return xOTHER;
}

Token Lexer::getNextToken() //handles tokenisation, ignores any spaces found.
{ //template used in class slides used to create the getNextToken() function
    Token temp;
    bool Space = false;
    String lexeme;
    int preState;

    do{ 
    //Initialisation stage
    lexeme = ""; 
    std::stack<int> stack;
    stack.push(-1);
    myCharacters currentChar;
    int currentState = tSTART;
    String line;

    line = myData[currentLine];    

    if (stringPtr == line.length()+1) //check if we are in a new line
    {
        currentLine++;
        if (currentLine == myData.size())
        {
            temp.end = true;
            return temp;
        }
        stringPtr = 0;
        line = myData[currentLine];
    }

    if (line.length()==0) //if line is empty
    {
        currentLine++;
        if (currentLine == myData.size())
        {
            temp.end = true;
            return temp;
        }
        stringPtr = 0;
        line = myData[currentLine];
    }
    
    //Scanning Loop
    int i = 0;
    for (i = stringPtr; i < line.length()+1; i++)
    {
        if (currentState == ERROR)
        {
            stack.push(currentState);
            break;
        }
        
        lexeme += (line[i]);

        if (isAccepting(currentState))
        {
            while (!stack.empty())
            {
                stack.pop();
            }
        }
        stack.push(currentState);

        preState = currentState;
        currentChar = getType(line[i]);
        currentState = stateTable[currentState][currentChar];

    }
    
    stringPtr = i;

    //Rollback loop stage
    String eLexeme = lexeme; //store lexeme incase of error
    while ((stack.top() != -1) && !isAccepting(stack.top()))
    {
        stack.pop();
        lexeme = lexeme.substr(0, lexeme.size() - 1);
        stringPtr--;
    }
    // Result reporting stage
    if (stack.top() == -1)
    {
        std::cout << "Error at line: "+std::to_string(currentLine+=1)+", "+eLexeme+" is not a valid lexeme" << std::endl;
        exit(-3);
        // report error
    }
    else if (isAccepting(stack.top()))
    {
        if(preState==tIDENTIFY)
        {
            preState = checkReserved(lexeme);
            Space=false;
        }
        else if(preState==tSPACE)
        {
            Space=true;
        }
        else
        {
            Space =false;
        }
        
    }
    else
    {
        std::cout << "Error at line: "+std::to_string(currentLine+=1)+", "+eLexeme+" is not a valid lexeme" << std::endl;
        exit(-3);
        // report error
    }
    }
    while(Space); //keep looping while a space is present

    temp.type.push_back(static_cast<MyTokens>(preState));
    temp.word.push_back(lexeme);
    temp.currLine = currentLine+1;


    return temp;
}

bool Lexer::isAccepting(int state) //All possible accepting states
{
    switch (static_cast<MyTokens>(state))
    {
    case tIDENTIFY:
    case tSPACE:
    case tLCURLY:
    case tRCURLY:
    case tLBRACKET:
    case tRBRACKET:
    case tDIGIT:
    case tFLOAT:
    case tLESSTHAN:
    case tLORE:
    case tGREATER:
    case tGORE:
    case tEQUAL:
    case tRELA:
    case tNOTE:
    case tCOMMA:
    case tCOLON:
    case tSCOLON:
    case tADD:
    case tSUBTRACT:
    case tSTAR:
    case tSLASH:
    case tARROW:
    case tSLit:
        return true;

    default:
        return false;
    }
}