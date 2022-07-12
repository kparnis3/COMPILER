#include <iostream>
#include <fstream>
#include <vector>
#include "Parser.hpp"


InterpreterPass::InterpreterPass()= default;

InterpreterPass::~InterpreterPass()= default;

void InterpreterPass::visit(ASTProgram* node){}

void InterpreterPass::visit(ASTForStatement* node)
{
    myTable.push();

    if(node->varD != nullptr)
    {
        node->varD->accept(this); //get actual variables if declaration exist
    }

    node->val->accept(this);

    bool myBool = false;
    // Relational -> convert all to boolean

    if(getType=="string")
    {
        myBool = (stoi(actualString) != 0);
    }
    else if(getType=="int")
    {
        myBool = (actualInt != 0);
    }
    else if(getType=="float")
    {
        myBool = ((int) actualFloat != 0);
    }
    else if(getType=="bool")
    {
        myBool = actualBool;
    }

    while(myBool) //loop while condition is true
    {
        node->forBlock->accept(this); //recheck block

        node->assign->accept(this); //update assignment

        node->val->accept(this); //reget values

        if(getType=="string")
        {
            myBool = (stoi(actualString) != 0);
        }
        else if(getType=="int")
        {
            myBool = (actualInt != 0);
        }
        else if(getType=="float")
        {
            myBool = ((int) actualFloat != 0);
        }
        else if(getType=="bool")
        {
            myBool = actualBool;
        }
       
    }

    myTable.pop();
}

void InterpreterPass::visit(ASTBlock* node)
{
    myTable.push(); //entering new scope
    
    for(const std::shared_ptr<ASTStatement>& s : node->block)
    {   
        s->accept(this);
    }
    
    myTable.pop(); //leaving scope
}

void InterpreterPass::visit(ASTAssign* node)
{
    node->val->accept(this); //check RHS

    String value = node->identify->IdentifyStore;
    // --- Assign the values in the Symbol table with the name of the identifier and the actual values
    if(getType=="float")
    {
        myTable.assignment(value, actualFloat);
    }
    else if(getType == "int")
    {
        myTable.assignment(value, actualInt);
    }
    else if(getType == "bool")
    {
        myTable.assignment(value, actualBool);
    }
    else
    {
        myTable.assignment(value, actualString);
    }


}

void InterpreterPass::visit(ASTIdentify* node) //used to get actual values (these are converted from string to there respective types)
{
    String val = myTable.checkVariableType(node->IdentifyStore)->second.myval;
    String type = myTable.checkVariableType(node->IdentifyStore)->second.type;

    if(type=="float")
    {
        actualFloat = std::stof(val);
    }
    else if(type == "int")
    {
        actualInt = std::stoi(val);
    }
    else if(type == "bool")
    {
        if(val == "1")
        {
            actualBool = true;
        }
        else
        {
            actualBool = false;
        }
    }
    else
    {
        actualString = val;
    }
}

void InterpreterPass::visit(ASTLiteral* node){}

//--- Get each actual value and there respective types ---
void InterpreterPass::visit(ASTLiteralBool* node)
{
    if(node->boolStore == 1)
    {
        actualBool = true;
    }
    else
    {
        actualBool = false;
    }

    getType = "bool";
}

void InterpreterPass::visit(ASTLiteralInt* node)
{
    actualInt = std::stoi(node->intStore);
    getType = "int";
}

void InterpreterPass::visit(ASTLiteralFloat* node)
{
    actualFloat = std::stof(node->floatStore);
    getType = "float";
}

void InterpreterPass::visit(ASTLiteralChar* node)
{
    actualString = node->StringStore;
    getType = "string";
}

void InterpreterPass::visit(ASTReturnStatement* node)
{   
    node->val->accept(this);
}

void InterpreterPass::visit(ASTPrintStatement* node)
{   
    node->val->accept(this); //check what type the value is and get its values

    // print answer to screen
    if(getType=="string")
    {
        std::cout << actualString << std::endl; 
    }
    else if(getType=="int")
    {
        std::cout << actualInt << std::endl;
    }
    else if(getType=="float")
    {
        std::cout << actualFloat << std::endl;
    }
    else if(getType=="bool")
    {
        if(actualBool)
        {
            std::cout << "True" << std::endl;
        }
        else
        {
            std::cout << "False" << std::endl;
        }
    }
}

void InterpreterPass::visit(ASTWhileStatement* node)
{
    myTable.push();

    node->val->accept(this);

    bool myBool = false;

    if(getType=="string")
    {
        myBool = (stoi(actualString) != 0);
    }
    else if(getType=="int")
    {
        myBool = (actualInt != 0);
    }
    else if(getType=="float")
    {
        myBool = ((int) actualFloat != 0);
    }
    else if(getType=="bool")
    {
        myBool = actualBool;
    }

    while(myBool)
    {
        node->whileBlock->accept(this);

        node->val->accept(this);

        if(getType=="string")
        {
            myBool = (stoi(actualString) != 0);
        }
        else if(getType=="int")
        {
            myBool = (actualInt != 0);
        }
        else if(getType=="float")
        {
            myBool = ((int) actualFloat != 0);
        }
        else if(getType=="bool")
        {
            myBool = actualBool;
        }
       
    }

    myTable.pop();
}

void InterpreterPass::visit(ASTIfStatement* node)
{
    myTable.push();
    bool myBool = false;
    node->val->accept(this);

    if(getType=="string")
    {
        myBool = (stoi(actualString) != 0);
    }
    else if(getType=="int")
    {
        myBool = (actualInt != 0);
    }
    else if(getType=="float")
    {
        myBool = ((int) actualFloat != 0);
    }
    else if(getType=="bool")
    {
        myBool = actualBool;
    }

    if(myBool)
    {
        node->ifBlock->accept(this);
    }
    else {
        if(node->elseBlock != nullptr)
        {
            node->elseBlock->accept(this);
        }
        
    }


    myTable.pop();
    
}

void InterpreterPass::visit(ASTVariableDecl* node)
{
    myTable.declaration(node);
    node->val->accept(this);

    String value = node->identify->IdentifyStore;
    if (getType == "int")
    {
        myTable.assignment(value, actualInt);
    }
    else if(getType == "float")
    {
        myTable.assignment(value, actualFloat);
    }
    else if(getType == "bool")
    {
        myTable.assignment(value, actualBool);
    }
    else if(getType == "string")
    {
        myTable.assignment(value, actualString);
    }
   
}

void InterpreterPass::visit(ASTUnary* node) // using - or not evaluates to negate the respective types
{
    node->val->accept(this);

    if (getType == "int")
    {
        actualInt = -actualInt;
    }
    else if(getType == "float")
    {
        actualFloat = -actualFloat;   
    }
    else if(getType == "bool")
    {
        actualBool = !actualBool;
    }   
}

void InterpreterPass::visit(ASTRelationalOP* node) //compute relation and store them accrdingly
{
    String s_valL;
    float f_valL;
    int i_valL;
    bool b_valL;

    node->valOne->accept(this);

    if (getType == "int")
    {
        i_valL = actualInt;
    }
    else if(getType == "float")
    {
        f_valL = actualFloat;
    }
    else if(getType == "bool")
    {
        b_valL = actualBool;
    }
    else if(getType == "string")
    {
        s_valL = actualString;
    }

    node->valTwo->accept(this);

    if (getType == "int")
    {
      if(node->checkOperator(node->operatorStore) == "<")
      {
          actualInt = i_valL < actualInt;
      }
      else if(node->checkOperator(node->operatorStore) == ">")
      {
          actualInt = i_valL > actualInt;
      }
      else if(node->checkOperator(node->operatorStore) == "==")
      {
          actualInt = i_valL == actualInt;
      } 

    }
    else if(getType == "float")
    {
      if(node->checkOperator(node->operatorStore) == "<")
      {
          actualFloat = f_valL < actualFloat;
      }
      else if(node->checkOperator(node->operatorStore) == ">")
      {
          actualFloat = f_valL > actualFloat;
      }
      else if(node->checkOperator(node->operatorStore) == "==")
      {
          actualFloat = f_valL == actualFloat;
      } 
    }
    else if(getType == "bool")
    {
      if(node->checkOperator(node->operatorStore) == "<")
      {
          actualBool = b_valL > actualBool;
      }
      else if(node->checkOperator(node->operatorStore) == ">")
      {
          actualBool = b_valL < actualBool;
      }
      else if(node->checkOperator(node->operatorStore) == "==")
      {
          actualBool = b_valL == actualBool;
      } 
    }
}

void InterpreterPass::visit(ASTAdditiveOP* node) //compute additive and store accordingly
{
    String s_valL;
    float f_valL;
    int i_valL;
    bool b_valL;

    node->valOne->accept(this);

    if (getType == "int")
    {
        i_valL = actualInt;
    }
    else if(getType == "float")
    {
        f_valL = actualFloat;
    }
    else if(getType == "bool")
    {
        b_valL = actualBool;
    }
    else if(getType == "string")
    {
        s_valL = actualString;
    }

    node->valTwo->accept(this);

    if (getType == "int")
    {
      if(node->checkOperator(node->operatorStore) == "+")
      {
          actualInt = i_valL + actualInt;
      }
      else if(node->checkOperator(node->operatorStore) == "-")
      {
          actualInt = i_valL - actualInt;
      }
      else if(node->checkOperator(node->operatorStore) == "or")
      {
          actualInt = i_valL || actualInt;
      } 
    }
    else if(getType == "float")
    {
      if(node->checkOperator(node->operatorStore) == "+")
      {
          actualFloat = f_valL + actualFloat;
      }
      else if(node->checkOperator(node->operatorStore) == "-")
      {
          actualFloat = f_valL - actualFloat;
      }
      else if(node->checkOperator(node->operatorStore) == "or")
      {
          actualFloat = f_valL || actualFloat;
      } 
    }
    else if(getType == "bool")
    {
      if(node->checkOperator(node->operatorStore) == "+")
      {
          actualBool = b_valL + actualBool;
      }
      else if(node->checkOperator(node->operatorStore) == "-")
      {
          actualBool = b_valL - actualBool;
      }
      else if(node->checkOperator(node->operatorStore) == "or")
      {
          actualBool = b_valL || actualBool;
      } 
    }   
}

void InterpreterPass::visit(ASTFormalParam* node){} 

void InterpreterPass::visit(ASTFunctionCall* node)
{
    std::vector<String> vals; //store all values as strings

    for(const std::shared_ptr<ASTExpression>& p : node->param) //get params to be kept as strings
    {
        p->accept(this);

        if(getType == "bool")
        {
            vals.push_back(std::to_string(actualBool));
        }
        else if(getType == "int")
        {
            vals.push_back(std::to_string(actualInt));
        }
        else if(getType == "float")
        {
            vals.push_back(std::to_string(actualFloat));
        }
        else if(getType == "string")
        {
            vals.push_back(actualString);
        }

    }

    String val = node->identify->IdentifyStore;

    ASTFunctionDecl* function = myTable.checkVariableType(val)->second.myFuncs[0]; //get function details

    myTable.push();

    int count = 0;

    for(const std::shared_ptr<ASTFormalParam>& f : function->param)
    {
        myTable.declaration(f); //declare the params

        if(f->type == "bool")
        {
            bool aBool;
            String boolString = vals[count];
            if(boolString == "true")
            {
                aBool = true;
            }
            else
            {
                aBool = false;
            }
            myTable.assignment(f->identify->IdentifyStore, aBool);
        }
        else if(f->type == "float")
        {
            float floatVal = std::stof(vals[count]);
            myTable.assignment(f->identify->IdentifyStore, floatVal);
        }
        else if(f->type == "int")
        {
            int intVal = std::stoi(vals[count]);
            myTable.assignment(f->identify->IdentifyStore, intVal);
        }
        else if(f->type == "string")
        {
            String StringVal = vals[count];
            myTable.assignment(f->identify->IdentifyStore, StringVal);
        }

        getType = function->type;

        count++;
    }

    function->functionBlock->accept(this); //execute function

    myTable.pop(); 

}

void InterpreterPass::visit(ASTFunctionDecl* node) //just declare the function
{
    myTable.declaration(node);
}

void InterpreterPass::visit(ASTMultiplicativeOP* node) //compute multiplicative and store accordingly
{
    String s_valL;
    float f_valL;
    int i_valL;
    bool b_valL;

    node->valOne->accept(this);

    if (getType == "int")
    {
        i_valL = actualInt;
    }
    else if(getType == "float")
    {
        f_valL = actualFloat;
    }
    else if(getType == "bool")
    {
        b_valL = actualBool;
    }
    else if(getType == "string")
    {
        s_valL = actualString;
    }

    node->valTwo->accept(this);

    if (getType == "int")
    {
      if(node->checkOperator(node->operatorStore) == "/")
      {
          actualInt = i_valL / actualInt;
      }
      else if(node->checkOperator(node->operatorStore) == "*")
      {
          actualInt = i_valL * actualInt;
      }
      else if(node->checkOperator(node->operatorStore) == "and")
      {
          actualInt = i_valL && actualInt;
      } 
    }
    else if(getType == "float")
    {
      if(node->checkOperator(node->operatorStore) == "/")
      {
          actualFloat = f_valL / actualFloat;
      }
      else if(node->checkOperator(node->operatorStore) == "*")
      {
          actualFloat = f_valL * actualFloat;
      }
      else if(node->checkOperator(node->operatorStore) == "and")
      {
          actualFloat = f_valL && actualFloat;
      } 
    }
    else if(getType == "bool")
    {
      if(node->checkOperator(node->operatorStore) == "/")
      {
          actualBool = b_valL / actualBool;
      }
      else if(node->checkOperator(node->operatorStore) == "*")
      {
          actualBool = b_valL * actualBool;
      }
      else if(node->checkOperator(node->operatorStore) == "and")
      {
          actualBool = b_valL && actualBool;
      } 
    }
   
}





