#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include "Parser.hpp"

SymTable::SymTable() = default;

SymTable::~SymTable()= default;

void SymTable::push()
{
    Scopes myScope;
    myStack.push_back(myScope);
}

void SymTable::pop()
{
    myStack.pop_back();
}

bool SymTable::checkDeclaration(const String& var)
{
    auto check = myStack.rbegin();

    while(check != myStack.rend()) //check through each scope, return true if variable has already been declared false otherwise.
    {
        if(check->find(var) != check->end())
        {
            return true;
        }
        check++;
    }
    return false;
}

bool SymTable::checkDeclaration(const String& var , std::vector<String>* type)
{
    auto check = myStack.rbegin();

    while(check != myStack.rend()) //check through each scope, return true if variable has already been declared false otherwise.
    {
        if(check->find(var) != check->end())
        {
            Scope function = (check->find(var))->second;
            
            for (ASTFunctionDecl* decl :function.myFuncs)
            {
                bool paramsMatch = true;

                int j = 0;

                for(const std::shared_ptr<ASTFormalParam>& p: decl->param) //checking if params match.
                {
                    if(p ->type != (*type)[j])
                    {
                        paramsMatch = false;
                        break;
                    }

                    j++;
                }

                if(paramsMatch)
                {
                    return paramsMatch; //params match by type therefore true
                }
            }

            return false;
        }
        check++;
    }
    return false;
}

Scopes* SymTable::top()
{
    return &*(myStack.end()-1); //get the top of the current scope
}

void SymTable::declaration(ASTVariableDecl* node)
{
    String variableName = node->identify->IdentifyStore; //get var name

    if(checkDeclaration(variableName))
    {
        throw std::runtime_error("Declaration error: "+variableName+" has already been declared");
    }

    Scope newScope = {.myval="",.myFuncs={},.type=node->type}; //We only care about the type of the variable here
    top()->insert({variableName, newScope});
   
}

void SymTable::declaration(ASTFormalParam* node)
{
    String variableName = node->identify->IdentifyStore;

    if(checkDeclaration(variableName))
    {
        throw std::runtime_error("Declaration error: "+variableName+" has already been declared");
    }

    Scope newScope = {.myval="",.myFuncs={},.type=node->type}; //We only care about the type of the varable here
    top()->insert({variableName, newScope});

}

void SymTable::declaration(const std::shared_ptr<ASTFormalParam>& node)
{
    String variableName = node->identify->IdentifyStore;

    if(checkDeclaration(variableName))
    {
        throw std::runtime_error("Declaration error: "+variableName+" has already been declared");
    }

    Scope newScope = {.myval="",.myFuncs={},.type=node->type}; //We only care about the type of the varable here
    top()->insert({variableName, newScope});

}

void SymTable::declaration(ASTFunctionDecl* node)
{
    String variableName = node->identify->IdentifyStore;

    if(checkDeclaration(variableName)) //first validate declaration
    {
           throw std::runtime_error("Declaration error: "+variableName+" has already been declared");
    }
    else
    {
        std::vector<ASTFunctionDecl*> functionName;
        functionName.push_back(node);
        Scope newScope = {.myval="" ,.myFuncs=functionName,.type=node->type};

        top()->insert({variableName, newScope}); //insert new scope to top of stack
    }

}

std::map<String, Scope>::iterator SymTable::checkVariableType(const String& var)
{
    auto check = myStack.rbegin();

    while(check != myStack.rend()) //check through each scope, return map if variable is found return random map otherwise (random map evaluates to error).
    {
        auto type = check->find(var);
        if(type != check->end())
        {
            return type;
        }
        check++;
    }

    return check->end();
}


//Store as a string within a scope, the different types of values

void SymTable::assignment(const String& var, const String& actualValue)
{
    checkVariableType(var)->second.myval = actualValue;
}
        
void  SymTable::assignment(const String& var, float actualValue)
{
    checkVariableType(var)->second.myval = std::to_string(actualValue);
}

void  SymTable::assignment(const String& var, int actualValue)
{
    checkVariableType(var)->second.myval = std::to_string(actualValue);
}

void  SymTable::assignment(const String& var, bool actualValue)
{
    checkVariableType(var)->second.myval = std::to_string(actualValue);
}
