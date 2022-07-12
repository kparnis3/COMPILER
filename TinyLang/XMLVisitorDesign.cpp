#include <iostream>
#include <fstream>
#include <vector>
#include "Parser.hpp"


XMLVisitorDesign::XMLVisitorDesign()
{
    this->visited = 1; //set the initial visited value;
}

XMLVisitorDesign::~XMLVisitorDesign()= default;

void XMLVisitorDesign::printIndent() const //add indents to XML
{
    for(int i=0; i<=visited; i++)
    {
        std::cout << "   ";
    }
}

// -- The following all follow a similar pattern of traversing the AST, updating the visiter count for the indents and printing the values stored in certain classes --- 

void XMLVisitorDesign::visit(ASTProgram* node){}

void XMLVisitorDesign::visit(ASTForStatement* node)
{
    printIndent();
    std::cout << "<For>" << std::endl;
    visited++;

    if(node->varD!=nullptr)
    {
        node->varD->accept(this);
    }

    node->val->accept(this);

    if(node->varD!=nullptr)
    {
        node->assign->accept(this);
    }

    visited--;
    printIndent();
    std::cout << "</For>" << std::endl;

    printIndent();
    std::cout << "<loop>" << std::endl;
    visited++;

    node->forBlock->accept(this);

    visited--;
    printIndent();
    std::cout << "</loop>" << std::endl;

}

void XMLVisitorDesign::visit(ASTBlock* node)
{
    printIndent();
    std::cout << "<Block>" << std::endl;
    visited++;

    for(const std::shared_ptr<ASTStatement>& s : node->block)
    {   
        s->accept(this);
    }

    visited--;
    printIndent();
    std::cout << "</Block>" << std::endl;
}

void XMLVisitorDesign::visit(ASTAssign* node)
{
    printIndent();
    std::cout << "<Assign>" << std::endl;
    visited++;

    printIndent();
    node->identify->accept(this);

    printIndent();
    node->val->accept(this);

    visited--;
    printIndent();
    std::cout << "</Assign>" << std::endl;
    
}

void XMLVisitorDesign::visit(ASTIdentify* node) //print the stored value
{
    std::cout << "<Identify>" << node->IdentifyStore << "</Identify>" << std::endl;
    
}

void XMLVisitorDesign::visit(ASTLiteral* node){}

void XMLVisitorDesign::visit(ASTLiteralBool* node)
{
    std::cout << "<LiteralBool>" << node->boolStore << "</LiteralBool>" << std::endl;
}

void XMLVisitorDesign::visit(ASTLiteralChar* node)
{
    std::cout << "<LiteralString>" << node->StringStore << "</LiteralString>" << std::endl;
}

void XMLVisitorDesign::visit(ASTLiteralInt* node)
{
    std::cout << "<LiteralInt>" << node->intStore << "</LiteralInt>" << std::endl;
}

void XMLVisitorDesign::visit(ASTLiteralFloat* node)
{
    std::cout << "<LiteralFloat>" << node->floatStore << "</Literalfloat>" << std::endl;
}

void XMLVisitorDesign::visit(ASTReturnStatement* node)
{   
    printIndent();
    std::cout << "<Return>" << std::endl;
    visited++;

    printIndent();
    node->val->accept(this);

    visited--;
    printIndent();
    std::cout << "</Return>" << std::endl;
}

void XMLVisitorDesign::visit(ASTPrintStatement* node)
{   
    printIndent();
    std::cout << "<Print>" << std::endl;
    visited++;

    printIndent();
    node->val->accept(this);

    visited--;
    printIndent();
    std::cout << "</Print>" << std::endl;
}

void XMLVisitorDesign::visit(ASTWhileStatement* node)
{
    printIndent();
    std::cout << "<while>" << std::endl;
    visited++;

    node->val->accept(this);

    visited--;
    printIndent();
    std::cout << "</while>" << std::endl;

    printIndent();
    std::cout << "<loop>" << std::endl;
    visited++;

    node->whileBlock->accept(this);

    visited--;
    printIndent();
    std::cout << "</loop>" << std::endl;
    
}

void XMLVisitorDesign::visit(ASTIfStatement* node)
{
    printIndent();
    std::cout << "<If>" << std::endl;
    visited++;

    node->val->accept(this);

    node->ifBlock->accept(this);

    visited--;
    printIndent();
    std::cout << "</If>" << std::endl;

    if(node->elseBlock!=nullptr)
    {
        printIndent();
        std::cout << "<Else>" << std::endl;
        visited++;

        node->elseBlock->accept(this);

        visited--;
        printIndent();
        std::cout << "</Else>" << std::endl;
    }
}

void XMLVisitorDesign::visit(ASTVariableDecl* node)
{
    
    printIndent();
    std::cout << "<VariableDecl type="<< node->type << ">" << std::endl;
    visited++;

    printIndent();
    node->identify->accept(this);

    printIndent();
    node->val->accept(this);

    visited--;
    printIndent();
    std::cout << "</VariableDecl>" << std::endl;

}

void XMLVisitorDesign::visit(ASTUnary* node)
{
    std::cout << "<Unary type="<< node->checkOperator(node->operatorStore) << ">" << std::endl;
    visited++;

    printIndent();
    node->val->accept(this);

    visited--;
    printIndent();
    std::cout << "</Unary>" << std::endl;
   
}

void XMLVisitorDesign::visit(ASTRelationalOP* node)
{
    printIndent();
    std::cout << "<Relational type= \""<< node->checkOperator(node->operatorStore) << "\" >" << std::endl;
    visited++;

    printIndent();
    node->valOne->accept(this);

    printIndent();
    node->valTwo->accept(this);

    visited--;
    printIndent();
    std::cout << "</Relational>" << std::endl;


}

void XMLVisitorDesign::visit(ASTAdditiveOP* node)
{
   std::cout << "<Additive type= \""<< node->checkOperator(node->operatorStore) << "\" >" << std::endl;
   visited++; 

   printIndent();
   node->valOne->accept(this);

   printIndent();
   node->valTwo->accept(this);

   visited--;
   printIndent();
   std::cout << "</Additive>" << std::endl;
}

void XMLVisitorDesign::visit(ASTFormalParam* node)
{
    printIndent();
    std::cout << "<FunctionDecl type="<< node->type << ">" << std::endl;
    visited++;

    printIndent();
    node->identify->accept(this);


    visited--;
    printIndent();
    std::cout << "</FunctionDecl>" << std::endl;
}

void XMLVisitorDesign::visit(ASTFunctionDecl* node)
{
    printIndent();
    std::cout << "<FunctionDecl type="<< node->type << ">" << std::endl;
    visited++;

    printIndent();
    node->identify->accept(this);

    for(const std::shared_ptr<ASTFormalParam>& s : node->param)
    {   
        s->accept(this);
    }


    node->functionBlock->accept(this);

    visited--;
    printIndent();
    std::cout << "</FunctionDecl>" << std::endl;
}

void XMLVisitorDesign::visit(ASTFunctionCall* node)
{
    std::cout << "<FunctionCall>" << std::endl;
    visited++;

    printIndent();
    node->identify->accept(this);

    for(const std::shared_ptr<ASTExpression>& e : node->param)
    {   
        printIndent();
        e->accept(this);
    }

    visited--;
    printIndent();
    std::cout << "</FunctionCall>" << std::endl;
} 

void XMLVisitorDesign::visit(ASTMultiplicativeOP* node)
{
   std::cout << "<Multiplicative type= \""<< node->checkOperator(node->operatorStore) << "\" >" << std::endl;
   visited++; 

   printIndent();
   node->valOne->accept(this);

   printIndent();
   node->valTwo->accept(this);

   visited--;
   printIndent();
   std::cout << "</Multiplicative>" << std::endl;
}





