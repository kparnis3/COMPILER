#include <iostream>
#include <fstream>
#include <vector>
#include "Parser.hpp"


SemanticVisitorDesign::SemanticVisitorDesign()= default;

SemanticVisitorDesign::~SemanticVisitorDesign()= default;

void SemanticVisitorDesign::visit(ASTProgram* node){}

void SemanticVisitorDesign::visit(ASTForStatement* node)
{
    myTable.push(); //entering new scope

    if(node->varD != nullptr) //check any declarations made
    {
        node->varD->accept(this);
    }

    node->val->accept(this);
    if(getOpType != "Relational")
    {
        throw std::runtime_error("Relational error: expected Relation, " +getType+ " given.");
    }

    if(node->assign != nullptr) //check any declarations made
    {
        node->assign->accept(this);
    }

    node->forBlock->accept(this); //check the block


    myTable.pop(); //leaving scope
}

void SemanticVisitorDesign::visit(ASTBlock* node)
{
    myTable.push(); //entering new scope
    
    for(const std::shared_ptr<ASTStatement>& s : node->block) //go through all the statements in the block
    {   
        s->accept(this);
    }
    
    myTable.pop(); //leaving scope
}

void SemanticVisitorDesign::visit(ASTAssign* node)
{
    String variableName = node->identify->IdentifyStore; //get var name

    if(!myTable.checkDeclaration(variableName)) //first check if variable has been declared
    {
        throw std::runtime_error("Declaration error: "+variableName+" has not been declared");
    }

    node->val->accept(this);

    String type = myTable.checkVariableType(variableName)->second.type; //secondly check if type matches variable type

    if(getType != type)
    {
       throw std::runtime_error("Declaration error: unable to declare "+type+ " as "+getType);
    } 
}

void SemanticVisitorDesign::visit(ASTIdentify* node)
{
    String variableName = node->IdentifyStore; //get var name

    if(!myTable.checkDeclaration(variableName)) //first check if variable has been declared
    {
        throw std::runtime_error("Declaration error: "+variableName+" has not been declared");
    }

    String type = myTable.checkVariableType(variableName)->second.type; //secondly check if type matches variable type
    getType = type;

}

void SemanticVisitorDesign::visit(ASTLiteral* node){}

// --- Get the types saved as Strings ---

void SemanticVisitorDesign::visit(ASTLiteralBool* node)
{
    getType = "bool";
}

void SemanticVisitorDesign::visit(ASTLiteralInt* node)
{
    getType = "int";
}

void SemanticVisitorDesign::visit(ASTLiteralFloat* node)
{
    getType = "float";
}

void SemanticVisitorDesign::visit(ASTLiteralChar* node)
{
    getType = "string";
}

void SemanticVisitorDesign::visit(ASTReturnStatement* node) //validate whats being returned and change flag to true
{   
    node->val->accept(this);
    myTable.hasReturn = true; 
}

void SemanticVisitorDesign::visit(ASTPrintStatement* node) //simply validate whats being printed
{   
    node->val->accept(this);
}

void SemanticVisitorDesign::visit(ASTWhileStatement* node) //validate the expressions and check if they form a relation
{
    node->val->accept(this);
    if(getOpType != "Relational")
    {
        throw std::runtime_error("Relational error: expected Relation, " +getType+ " given.");
    }

    node->whileBlock->accept(this); //check block
}

void SemanticVisitorDesign::visit(ASTIfStatement* node)
{
    node->val->accept(this);
    if(getOpType != "Relational")
    {
        throw std::runtime_error("Relational error: expected Relation, " +getType+ " given.");
    }

    node->ifBlock->accept(this);

    if(node->elseBlock != nullptr)
    {
        node->elseBlock->accept(this); //check else block if found
    }

}

void SemanticVisitorDesign::visit(ASTVariableDecl* node)
{
    node->val->accept(this);

    if(getType != node->type) //check if the type returned is the same type as the declaration
    {
       throw std::runtime_error("Declaration error: unable to declare "+node->type+ " as "+getType);
    }

    myTable.declaration(node);
}

void SemanticVisitorDesign::visit(ASTUnary* node) //simply validate
{
   node->val->accept(this);
}

void SemanticVisitorDesign::visit(ASTRelationalOP* node) //get the left and right types and validate that they are the same.
{
   node->valOne->accept(this);
   String leftType = getType;

   node->valTwo->accept(this);
   String rightType = getType;

   getOpType = "Relational"; //set op type accordingly

   if (leftType!=rightType) 
   {
       throw std::runtime_error("TypeMatch error: "+leftType+ " and "+rightType+" do not match");
   }
}

void SemanticVisitorDesign::visit(ASTAdditiveOP* node) //get the left and right types and validate that they are the same.
{
   node->valOne->accept(this);
   String leftType = getType;

   node->valTwo->accept(this);
   String rightType = getType;

   getOpType = "Additive"; //set op type accordingly

   if (leftType!=rightType)
   {
       throw std::runtime_error("TypeMatch error: "+leftType+ " and "+rightType+" do not match");
   }
}

void SemanticVisitorDesign::visit(ASTFormalParam* node)
{
    myTable.declaration(node); 
}

void SemanticVisitorDesign::visit(ASTFunctionDecl* node)
{
    myTable.push();

    for(const std::shared_ptr<ASTFormalParam>& p :node->param) //validate all params
    {
       p->accept(this);
    }

    auto AllBlocks = node->functionBlock->block;

    for(int i=0; i<AllBlocks.size()-1; i++) //go throigh all the blocks
    {
        AllBlocks[i]->accept(this);
    }

    myTable.hasReturn = false;

    AllBlocks.back()->accept(this); //check the back of the blocks to see if a return statement is present or not.

    if(!myTable.hasReturn)
    {
        throw std::runtime_error("Return error: "+node->identify->IdentifyStore+" does not contain a return statement.");
    }

    if(node->type != getType)
    {
        throw std::runtime_error("Type error: Function return type, "+node->type+" does not match actual return type, "+getType);
    }

    myTable.pop();

    myTable.declaration(node);
} 

void SemanticVisitorDesign::visit(ASTFunctionCall* node)
{
    String variableName = node->identify->IdentifyStore; //get var name

    std::vector<String> types; //get list of param types 
    for(const std::shared_ptr<ASTExpression>& p :node->param)
    {
       p->accept(this);
       types.push_back(getType);
    }

    if(!myTable.checkDeclaration(variableName, &types)) //check if variable names and types have been declared
    {
        String error = "Function: " + variableName + " has not been defined with type: ";
        for (auto & type : types)
        {
             error += type + " "; //print the incorrect typing
        }

        throw std::runtime_error(error);
    }

}

void SemanticVisitorDesign::visit(ASTMultiplicativeOP* node)
{
   node->valOne->accept(this);
   String leftType = getType;

   node->valTwo->accept(this);
   String rightType = getType;

   if (leftType!=rightType)
   {
       throw std::runtime_error("TypeMatch error: "+leftType+ " and "+rightType+" do not match");
   }
}





