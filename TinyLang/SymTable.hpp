#include "lexer.hpp"
#include "ASTClasses.hpp"
#include <memory>
#include <map>
#include <vector>

struct Scope
{
    String myval; //store variable
    std::vector<ASTFunctionDecl*> myFuncs; //store function name
    String type; 
};


typedef std::map<String, Scope> Scopes; //Map storing name of object with its information which needed to be typecast to create instances to be pushed on stack

class SymTable
{
    public:
        SymTable();
        ~SymTable();

        void assignment(const String& var, const String& actualValue); //Polymorphed depending on the actual value passed
        void assignment(const String& var, float actualValue);
        void assignment(const String& var, int actualValue);
        void assignment(const String& var, bool actualValue);

        void push();
        void pop();
        Scopes* top();

        void declaration(ASTFunctionDecl* node); // Polymorphed depending on the declaration
        void declaration(ASTVariableDecl* node);
        void declaration(ASTFormalParam* node);
        void declaration(const std::shared_ptr<ASTFormalParam>& node);

        bool checkDeclaration(const String& var); // Check if variables have been declared
        bool checkDeclaration(const String& var, std::vector<String>* type);

        std::map<String, Scope>::iterator checkVariableType(const String& var);
        bool hasReturn{}; //used to check if a function has a return statement

    private:
         std::vector<Scopes> myStack; //ini stack of scopes -> map (String, Scope).
         
         
       
};