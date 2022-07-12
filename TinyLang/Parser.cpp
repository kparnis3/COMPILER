#include <iostream>
#include <fstream>
#include "Parser.hpp"
#include <utility>
#include <vector>
#include <memory>
#include <algorithm>

void Parser::XMLPass() //Starts the XML Pass on program
{
    auto xml = new XMLVisitorDesign();
    
    std::cout << "XML pass:" << std::endl;
    std::cout << "" << std::endl;

    std::cout << "<Program>" << std::endl;
    for(const std::shared_ptr<ASTStatement>& s : this->program)
    {   
        s->accept(xml);
    }
    std::cout << "</Program>\n" << std::endl;
    
    delete xml;
}

void Parser::SemanticPass() //Starts the Semantic Pass on program
{
    auto semantic = new SemanticVisitorDesign();
    
    semantic->myTable.push();

    std::cout << "Semantic pass:" << std::endl;
    std::cout << "" << std::endl;
    
    for(const std::shared_ptr<ASTStatement>& s : this->program)
    {   
        s->accept(semantic);
    }

    std::cout << "Semantic pass completed without error.\n" << std::endl;
    semantic->myTable.pop();
    
    delete semantic;
}

void Parser::InterpretPass()
{
    auto Interpret = new InterpreterPass();

    Interpret->myTable.push();

    std::cout << "Interpreter pass:" << std::endl;
    std::cout << "" << std::endl;
    
    for(const std::shared_ptr<ASTStatement>& s : this->program)
    {   
        s->accept(Interpret);
    }

    Interpret->myTable.pop();

    delete Interpret;
}

std::shared_ptr<ASTProgram> Parser::BeginParse()  
//Starts here with <Program> ::= { <Statement> }   
{
    std::vector<std::shared_ptr<ASTStatement>> AllStatements; //Vector of pointers for tree

    while (!llOne.end) //Loop till end is reached
    {

        AllStatements.push_back(stateParsing());
    }
    
    this->program = std::move(AllStatements);

    return std::shared_ptr<ASTProgram>(std::make_shared<ASTProgram>(AllStatements)); //create the shared pointer

    
}

std::shared_ptr<ASTStatement> Parser::stateParsing()
/*<Statement> := <Assignment>
                 <Block> 
                 <PrintStatement>
                 <RtrnStatement>
                 <IfStatement>
                 <WhileStatement>
                 <VarDecl>
                 <ForStatement>
                 <FuncDecl>*/
{    
    //ASTStatement* singleStatement;
    auto singleStatement = std::make_shared<ASTStatement>();
    
    bool CheckSemi = false;
    int currline = llOne.currLine;
    switch(llOne.type[0]) //check next token, and progress accordingly
    {
        case tIDENTIFY:
            singleStatement = assignParsing();  
            CheckSemi = true;
            break;
        case tRCURLY:
            this->getNextToken();
            singleStatement = blockParsing();
            break;
        case tLET:
            this->getNextToken();
            singleStatement = variableDeclParsing();
            CheckSemi = true;
            break;
        case tPRINT:
            this->getNextToken();
            singleStatement = printParsing();
            CheckSemi = true;
            break;
        case tRETURN:
            this->getNextToken();
            singleStatement = returnParsing();
            CheckSemi = true;
            break;
        case tIF:
            this->getNextToken();
            singleStatement = ifParsing();
            break;
        case tWHILE:
            this->getNextToken();
            singleStatement = whileParsing();
            break;
        case tFOR:
            this->getNextToken();
            singleStatement = forParsing();
            break;
        case tFUNCTION:
            this->getNextToken();
            singleStatement = functionParsing();
            break;
        default:
            throw std::runtime_error("Error at line "+std::to_string(llOne.currLine)+ ", Unexpected "+llOne.word[0]);
    }

    if(CheckSemi)
    {
        if(llOne.type[0] != tSCOLON)
        {
            throw std::runtime_error("Error at line "+std::to_string(currline)+ " ; Expected");
        }
        getNextToken();
    }

    //Implement semicolon check

    return singleStatement;
}

std::shared_ptr<ASTFunctionDecl> Parser::functionParsing()
// fn <Identify> '(' [ <FormalParams> ] ')' '->' <Type> <Block>
{
    String type;
    auto identify = identifyParsing();
    std::vector<std::shared_ptr<ASTFormalParam>> Params; //Vector of pointers for sub-tree
    if(llOne.type[0] == tLBRACKET)
    {
        this->getNextToken();

        if(llOne.type[0] != tRBRACKET)
        {
            Params.push_back(formalParsing());

            while(llOne.type[0] == tCOMMA)
            {
                this->getNextToken();
                Params.push_back(formalParsing());
            }

            if(llOne.type[0] == tRBRACKET)
            {
                this->getNextToken();
                if(llOne.type[0] == tARROW)
                {
                    this->getNextToken();
                    if(checkType(llOne.type[0]))
                    {
                       type = llOne.word[0];
                        this->getNextToken();
                        auto block = blockParsing();

                        return std::shared_ptr<ASTFunctionDecl>(std::make_shared<ASTFunctionDecl>(identify, Params, type, block));
                    }
                    else
                    {
                        throw std::runtime_error("Error at line "+std::to_string(llOne.currLine)+ ", Unknown type "+llOne.word[0]);
                    }
                }
                else
                {
                    throw std::runtime_error("Error at line "+std::to_string(llOne.currLine)+ ", Expected ->, recieved unexpected "+llOne.word[0]);
                }
            
            }
            else
            {
                throw std::runtime_error("Error at line "+std::to_string(llOne.currLine)+ ", Expected Right Bracket, recieved unexpected "+llOne.word[0]);
            }
        }
        else
        {
            throw std::runtime_error("Error at line "+std::to_string(llOne.currLine)+ ", Expected Right Bracket, revieved unexpected "+llOne.word[0]);
        }
    }
    else
    {
        throw std::runtime_error("Error at line "+std::to_string(llOne.currLine)+ ", Expected Left Bracket, recieved unexpected "+llOne.word[0]);
    }
}

std::shared_ptr<ASTFunctionCall> Parser::functionCallParsing()
{
    auto identify = identifyParsing();
    std::vector<std::shared_ptr<ASTExpression>> Params; //Vector of pointers for sub-tree
    if(llOne.type[0] == tLBRACKET)
    {
        this->getNextToken();

        if(llOne.type[0] != tRBRACKET)
        {
            Params.push_back(expressionParsing());

            while(llOne.type[0] == tCOMMA)
            {
                this->getNextToken();
                Params.push_back(expressionParsing());
            }

            if(llOne.type[0] == tRBRACKET)
            {
                this->getNextToken();
                return std::shared_ptr<ASTFunctionCall>(std::make_shared<ASTFunctionCall>(identify, Params));
            }
            else
            {
                throw std::runtime_error("Error at line "+std::to_string(llOne.currLine)+ ", Expected Right Bracket, recieved unexpected "+llOne.word[0]);
            }
        }
        else
        {   
            this->getNextToken();
            return std::shared_ptr<ASTFunctionCall>(std::make_shared<ASTFunctionCall>(identify, Params));
        }
    }
    else
    {
        throw std::runtime_error("Error at line "+std::to_string(llOne.currLine)+ ", Expected Left Bracket, recieved unexpected "+llOne.word[0]);
    }
}

std::shared_ptr<ASTFormalParam> Parser::formalParsing()
// <<Identify> ':' <Type>
{
    String type;

    auto identify = identifyParsing();
    if(llOne.type[0] == tCOLON)
    {
        this->getNextToken();
        if(checkType(llOne.type[0]))
        {
            type = llOne.word[0];
            this->getNextToken();

             return std::shared_ptr<ASTFormalParam>(std::make_shared<ASTFormalParam>(identify, type));
        }

        else
        {
            throw std::runtime_error("Error at line "+std::to_string(llOne.currLine)+ ", receieved unknown type "+llOne.word[0]);
        }
    }
    else 
    {
        throw std::runtime_error("Error at line "+std::to_string(llOne.currLine)+ "colon expected, received unexpected "+llOne.word[0]);
    }
}

std::shared_ptr<ASTForStatement> Parser::forParsing()
// 'for' '(' [<VariableDecl>] ';' <Expression> ';' [<Assignment>] ')' <Block>
{
    if(llOne.type[0]==tLBRACKET)
    {
        auto dec = std::make_shared<ASTVariableDecl>();
        auto assign = std::make_shared<ASTAssign>();
        
        this->getNextToken();
        if(llOne.type[0] == tLET)
        {   
            this->getNextToken();
            dec = variableDeclParsing();
        }
        else
        {
            dec = nullptr;
        }

        if(llOne.type[0]==tSCOLON)
        {
            this->getNextToken();
            auto expression = expressionParsing();
            if(llOne.type[0]==tSCOLON)
            {
                this->getNextToken();
                if(llOne.type[0] == tIDENTIFY)
                {
                    assign = assignParsing();
                }
                else
                {
                    assign = nullptr;
                }
                if(llOne.type[0]==tRBRACKET)
                {
                    this->getNextToken();
                    auto Fblock = blockParsing();

                    return std::shared_ptr<ASTForStatement>(std::make_shared<ASTForStatement>(dec,expression,assign,Fblock));
                }

            }
            else
            {
                throw std::runtime_error("Error at line "+std::to_string(llOne.currLine)+ "semi colon expected, received unexpected "+llOne.word[0]);
            }
        }
        else
        {
              throw std::runtime_error("Error at line "+std::to_string(llOne.currLine)+ "semi colon expected, received unexpected "+llOne.word[0]);
        }
    }
    throw std::runtime_error("Error at line "+std::to_string(llOne.currLine)+ "right curly bracket expected, received unexpected "+llOne.word[0]);
}

std::shared_ptr<ASTBlock> Parser::blockParsing()
// <Block> ::== '{' <Statement> '}'
{
    if(llOne.type[0] == tRCURLY)
    {
        this->getNextToken();
       
        std::vector<std::shared_ptr<ASTStatement>> AllStatements; //Vector of pointers for sub-tree
        while(llOne.end == false) //Loop till end is reached
        {
            if(llOne.type[0] == tLCURLY)
            {
                break;
            }
            AllStatements.push_back(stateParsing());
        }

        if(llOne.type[0]==tLCURLY)
        {
            this->getNextToken();
            return std::shared_ptr<ASTBlock>(std::make_shared<ASTBlock>(AllStatements));
        }
        else
        {
            throw std::runtime_error("Error at line "+std::to_string(llOne.currLine)+ "right curly bracket expected, received unexpected "+llOne.word[0]);
        }
        
    }
    else
    {
        throw std::runtime_error("Error at line "+std::to_string(llOne.currLine)+ ", left curly bracket expected, received unexpected "+llOne.word[0]);
    }
}

std::shared_ptr<ASTWhileStatement> Parser::whileParsing()
// <WhileStatement> ::= 'while' '(' <Expression> ')' <Block>
{
    if(llOne.type[0]==tLBRACKET)
    {
        this->getNextToken();
        auto expression = expressionParsing();
        if(llOne.type[0]==tRBRACKET)
        {
            this->getNextToken();
            auto Wblock = blockParsing();
            return std::shared_ptr<ASTWhileStatement>(std::make_shared<ASTWhileStatement>(expression, Wblock));
        }

         throw std::runtime_error("Error at line "+std::to_string(llOne.currLine)+ ", right bracket expected, received unexpected "+llOne.word[0]);
        
    }

    throw std::runtime_error("Error at line "+std::to_string(llOne.currLine)+ ", left bracket expected, received unexpected "+llOne.word[0]);
    
}

std::shared_ptr<ASTIfStatement> Parser::ifParsing()
// <IfStatement> ::= 'if' '(' <Expression> ')' <Block> [ 'else' <Block> ]
{
    if(llOne.type[0]==tLBRACKET)
    {
        this->getNextToken();
        auto expression = expressionParsing();
        if(llOne.type[0]==tRBRACKET)
        {
            this->getNextToken();
            auto Fblock = blockParsing();
            if(llOne.type[0]==tELSE)
            {
                this->getNextToken();
                auto Eblock = blockParsing();

                return std::shared_ptr<ASTIfStatement>(std::make_shared<ASTIfStatement>(expression, Fblock, Eblock));
            }
            else
            {

                return std::shared_ptr<ASTIfStatement>(std::make_shared<ASTIfStatement>(expression, Fblock, nullptr));
            }
            
        }

        throw std::runtime_error("Error at line "+std::to_string(llOne.currLine)+ ", right bracket expected, received unexpected "+llOne.word[0]);

    }

    throw std::runtime_error("Error at line "+std::to_string(llOne.currLine)+ ", left bracket expected, received unexpected "+llOne.word[0]);
}

std::shared_ptr<ASTPrintStatement> Parser::printParsing()
{
    auto expression = expressionParsing();

    return std::shared_ptr<ASTPrintStatement>(std::make_shared<ASTPrintStatement>(expression));
}

std::shared_ptr<ASTReturnStatement> Parser::returnParsing()
{
    auto expression = expressionParsing();

    return std::shared_ptr<ASTReturnStatement>(std::make_shared<ASTReturnStatement>(expression));
}


std::shared_ptr<ASTVariableDecl> Parser::variableDeclParsing()
// <VariableDecl> ::== 'let' <Identifier> ':' <Type> '=' <Expression>
{
    auto identify = identifyParsing();
    if(llOne.type[0] == tCOLON)
    {   
        String type;
        this->getNextToken();

        if(checkType(llOne.type[0]))
        {
            type = llOne.word[0];

            this->getNextToken();

            if(llOne.type[0] == tEQUAL)
            {
                this->getNextToken();
                auto expression = expressionParsing();

                return std::shared_ptr<ASTVariableDecl>(std::make_shared<ASTVariableDecl>(identify,type, expression));
            }
        }

        throw std::runtime_error("Error at line "+std::to_string(llOne.currLine)+ ", "+llOne.word[0] + " is not a valid type");
    }

    throw std::runtime_error("Error at line "+std::to_string(llOne.currLine)+ ", Colon expected, received unexpected "+llOne.word[0]);
}

std::shared_ptr<ASTAssign> Parser::assignParsing()
//<Assignment> ::= <Identify> '=' <Expression>
{ 
    auto identify = identifyParsing();
    if(llOne.type[0] == tEQUAL) //check if next token is equals
    {
        this->getNextToken();
        auto val = expressionParsing(); 

        return std::shared_ptr<ASTAssign>(std::make_shared<ASTAssign>(identify, val));
    }

    throw std::runtime_error("Error at line "+std::to_string(llOne.currLine)+ ", Unexpected "+llOne.word[0]+ "Expected '='");
    
}
// --Helper functions to valaidate proper tokens ---
bool Parser::checkRelation(MyTokens token)
{
    switch(token)
    {
        case tGREATER: return  true;
        case tGORE: return true;
        case tRELA: return true;
        case tLESSTHAN: return true;
        case tLORE: return true;
        case tNOTE: return true;

        default: return false;
    }
}

bool Parser::checkType(MyTokens token)
{
    switch(token)
    {
        case tFLOAT: return true;
        case tINT: return true;
        case tBOOLEAN: return true;
        case tCHAR: return true;

        default: return false;

    }
}

bool Parser::checkAdditive(MyTokens token)
{
    switch(token)
    {
        case tOR: return  true;
        case tSUBTRACT: return true;
        case tADD: return true;
        
        default: return false;
    }
}

bool Parser::checkMultiplicative(MyTokens token)
{
    switch(token)
    {
        case tSTAR: return  true;
        case tSLASH: return true;
        case tAND: return true;
        
        default: return false;
    }
}

OP Parser::getRelation(MyTokens token)
{
    switch(token)
    {
        case tGREATER: return  oGREATERTHAN;
        case tGORE: return oGREATEROREQUAL;
        case tRELA: return oRELATION;
        case tLESSTHAN: return oLESSTHAN;
        case tLORE: return oLESSOREQUAL;
        case tNOTE: return oNOTEQUAL;

        default:
            throw std::runtime_error("Error at line "+std::to_string(llOne.currLine)+ ", Unexpected "+llOne.word[0]);
    }
}


OP Parser::getMultiplicative(MyTokens token)
{
    switch(token)
    {
        case tSTAR: return  oSTAR;
        case tSLASH: return oSLASH;
        case tAND: return oAND;
      

        default:
            throw std::runtime_error("Error at line "+std::to_string(llOne.currLine)+ ", Unexpected "+llOne.word[0]);
    }
}

OP Parser::getAdditive(MyTokens token)
{
    switch(token)
    {
        case tOR: return oOR;
        case tSUBTRACT: return oSUBTRACT;
        case tADD: return oPLUS;

        default:
            throw std::runtime_error("Error at line "+std::to_string(llOne.currLine)+ ", Unexpected "+llOne.word[0]);
    }
}

std::shared_ptr<ASTExpression> Parser::expressionParsing()
// <Expression> ::= <SimpleExpr> {<RelationalOp> <SimpleExpr>}
{   
    OP relation;
    auto expression = std::make_shared<ASTExpression>();
    auto expressionLeft = std::make_shared<ASTExpression>();

    expressionLeft = simpleexpressionParsing();
    if(checkRelation(llOne.type[0]))
    {
        relation = getRelation(llOne.type[0]);
        this->getNextToken();
        auto expressionRight = std::make_shared<ASTExpression>();
        expressionRight = simpleexpressionParsing();

        expression = std::shared_ptr<ASTRelationalOP>(std::make_shared<ASTRelationalOP>(expressionLeft, relation, expressionRight));
    }
    else
    {
        expression = std::move(expressionLeft);
    }


    return expression;
}



std::shared_ptr<ASTIdentify> Parser::identifyParsing()
//<Identify>
{
    String identify;

    if(llOne.type[0] == tIDENTIFY) //getNextToken() -> should be an identifier
    {   
        identify = llOne.word[0];
        this->getNextToken();
    }
    else
    {
        throw std::runtime_error("Error at line "+std::to_string(llOne.currLine)+ "Expected identifier, recieved unexpected "+llOne.word[0]);
    }

     std::shared_ptr<ASTIdentify> myIdentify = std::make_shared<ASTIdentify>();
     myIdentify->IdentifyStore = identify;

     return std::move(myIdentify);
}

std::shared_ptr<ASTExpression> Parser::simpleexpressionParsing() // Add optional RHS
//<SimpleExpression> ::= <Term> { <AdditiveOp> <Term> }
{
    OP relation;
    auto expression = std::make_shared<ASTExpression>();
    auto expressionLeft = std::make_shared<ASTExpression>();

    expressionLeft = termParsing();

    if(checkAdditive(llOne.type[0]))
    {
        relation = getAdditive(llOne.type[0]);
        this->getNextToken();
        auto expressionRight = std::make_shared<ASTExpression>();
        expressionRight = simpleexpressionParsing();

        expression = std::shared_ptr<ASTAdditiveOP>(std::make_shared<ASTAdditiveOP>(expressionLeft, relation, expressionRight));
    }
    else
    {
        expression = std::move(expressionLeft);
    }


    return expression;
}

std::shared_ptr<ASTExpression> Parser::termParsing() //Add RHS + ll2
// <Term> ::= <Factor> { <MultiplicativeOP> <Factor>  }
{
    OP relation;
    auto expression = std::make_shared<ASTExpression>();
    auto expressionLeft = std::make_shared<ASTExpression>();

    expressionLeft = factorParsing();
    if(checkMultiplicative(llOne.type[0]))
    {
        relation = getMultiplicative(llOne.type[0]);
        this->getNextToken();
        auto expressionRight = std::make_shared<ASTExpression>();
        expressionRight = termParsing();

        expression = std::shared_ptr<ASTMultiplicativeOP>(std::make_shared<ASTMultiplicativeOP>(expressionLeft, relation, expressionRight));
    }
    else
    {
        expression = std::move(expressionLeft);
    }


    return expression;
}

std::shared_ptr<ASTExpression> Parser::factorParsing() //Add rest
/*
<Factor> ::= <Literal>
             <Identify>
             <FunctionCall>
             <SubExpression>
             <Unary>  
*/ 

{
    auto factor = std::make_shared<ASTExpression>();

    switch(llOne.type[0])
    {
        case tDIGIT:
        case tFLOAT:
        case tSLit:
        case tTRUE:
        case tFALSE:
            factor = literalParsing();
            break;
        case tIDENTIFY:
            if(llTwo.type[0]==tLBRACKET)
            {
                factor=functionCallParsing();
            }
            else
            {
                factor = identifyParsing();
            }
            break;
        case tLBRACKET:
            factor = subExpressionParsing();
            break;   
        case tN:
        case tSUBTRACT:
            factor = unaryParsing();
            break;
        default:
            throw std::runtime_error("Error at line "+std::to_string(llOne.currLine)+ ", Unexpected "+llOne.word[0]);
    }

    return factor;
}

std::shared_ptr<ASTExpression> Parser::subExpressionParsing()
// <SubExpression> ::= '(' Expression ')'
{
    auto expression = std::make_shared<ASTExpression>();
    this->getNextToken();
    expression = expressionParsing();
    if(llOne.type[0]!=tRBRACKET)
    {
        throw std::runtime_error("Error at line "+std::to_string(llOne.currLine)+ ", Unexpected "+llOne.word[0]);
    }
    this->getNextToken();
    
    return expression;
}

std::shared_ptr<ASTUnary> Parser::unaryParsing()
// <unary> :== ('-' | 'not')<Expression>
{
    OP myOperator;

    if(llOne.type[0]==tN)
    {
        myOperator = oNOT;
        this->getNextToken();
    }
    else if(llOne.type[0]==tSUBTRACT)
    {
        myOperator = oSUBTRACT;
        this->getNextToken();
    }
    else{
        throw std::runtime_error("Error at line "+std::to_string(llOne.currLine)+ ", Unexpected "+llOne.word[0]);
    }
    auto val = expressionParsing();

    return std::shared_ptr<ASTUnary>(std::make_shared<ASTUnary>(myOperator, val));
}

std::shared_ptr<ASTLiteral> Parser::literalParsing()
/* <Literal> ::== <BooleanLiteral>
                  <IntegerLiteral>
                  <FloatLiteral> 
                  <charLiteral> 
*/
{
    auto Literal = std::make_shared<ASTLiteral>();
    String tokenValue = llOne.word[0];

    switch (llOne.type[0])
    {
    case tFLOAT: 
        {
        std::shared_ptr<ASTLiteralFloat> myFloat = std::make_shared<ASTLiteralFloat>();
        myFloat->floatStore = tokenValue;
        Literal = std::move(myFloat);
        }
        break;
    case tTRUE:
        {
        std::shared_ptr<ASTLiteralBool> myBool = std::make_shared<ASTLiteralBool>();
        myBool->boolStore = true;
        Literal = std::move(myBool);
        break;
        }
    case tFALSE:
        {
        std::shared_ptr<ASTLiteralBool> myBool = std::make_shared<ASTLiteralBool>();
        myBool->boolStore = false;
        Literal = std::move(myBool);
        break;
        }
    case tDIGIT:
        {
        std::shared_ptr<ASTLiteralInt> myInt = std::make_shared<ASTLiteralInt>();
        myInt->intStore = tokenValue;
        Literal = std::move(myInt);
        break;
        }
    case tSLit:
        {
        std::shared_ptr<ASTLiteralChar> myChar = std::make_shared<ASTLiteralChar>();
        tokenValue.erase(0,1);
        tokenValue.erase(tokenValue.size()-1);
        myChar->StringStore = tokenValue;
        Literal = std::move(myChar);
        break;
        }
    default:
        throw std::runtime_error("Error at line "+std::to_string(llOne.currLine)+ ", Unexpected "+llOne.word[0]);
    }

    this->getNextToken();

    return Literal;
}

/// --- The following functions assign any data needed to the classes themselves ---

ASTProgram::ASTProgram(std::vector<std::shared_ptr<ASTStatement>> AllStatements)
{
    this->program = std::move(AllStatements); //Move all statements to program
}

ASTBlock::ASTBlock(std::vector<std::shared_ptr<ASTStatement>> AllStatements)
{
    this->block = std::move(AllStatements);
}

ASTFormalParam::ASTFormalParam()= default;

ASTBlock::ASTBlock()= default;

ASTIdentify::ASTIdentify() = default;

ASTLiteralFloat::ASTLiteralFloat()= default;

ASTLiteralBool::ASTLiteralBool()= default;

ASTLiteralInt::ASTLiteralInt()= default;

ASTLiteralChar::ASTLiteralChar()= default;

ASTVariableDecl::ASTVariableDecl()= default;

ASTAssign::ASTAssign()= default;

ASTAssign::ASTAssign(std::shared_ptr<ASTIdentify> Identify, std::shared_ptr<ASTExpression> Expression)
{
    this->identify = std::move(Identify);
    this->val = std::move(Expression);
}

ASTVariableDecl::ASTVariableDecl(std::shared_ptr<ASTIdentify> Identify, String Type, std::shared_ptr<ASTExpression> Expression)
{
    this->identify = std::move(Identify);
    this->type = std::move(Type);
    this->val = std::move(Expression);
}

ASTUnary::ASTUnary(OP op, std::shared_ptr<ASTExpression> Expression)
{
    this->operatorStore = op;
    this->val = std::move(Expression);
}

ASTRelationalOP::ASTRelationalOP(std::shared_ptr<ASTExpression> ExpressionLeft, OP Operator, std::shared_ptr<ASTExpression> ExpressionRight)
{   
    this->valOne = std::move(ExpressionLeft);
    this->operatorStore = Operator;
    this->valTwo = std::move(ExpressionRight);
}

ASTAdditiveOP::ASTAdditiveOP(std::shared_ptr<ASTExpression> ExpressionLeft, OP Operator, std::shared_ptr<ASTExpression> ExpressionRight)
{   
    this->valOne = std::move(ExpressionLeft);
    this->operatorStore = Operator;
    this->valTwo = std::move(ExpressionRight);
}

ASTFunctionDecl::ASTFunctionDecl(std::shared_ptr<ASTIdentify> Identify, std::vector<std::shared_ptr<ASTFormalParam>> par ,String Type, std::shared_ptr<ASTBlock> fBlock)
{
    this->identify = std::move(Identify);
    this->param = std::move(par);
    this->type = std::move(Type);
    this->functionBlock = std::move(fBlock);
}

ASTFunctionCall::ASTFunctionCall(std::shared_ptr<ASTIdentify> Identify, std::vector<std::shared_ptr<ASTExpression>> par )
{
    this->identify = std::move(Identify);
    this->param = std::move(par);
}

ASTIfStatement::ASTIfStatement(std::shared_ptr<ASTExpression> Expression, std::shared_ptr<ASTBlock> ifB, std::shared_ptr<ASTBlock> elseB)
{
    this->elseBlock = std::move(elseB);
    this->ifBlock = std::move(ifB);
    this->val = std::move(Expression);
}

ASTPrintStatement::ASTPrintStatement(std::shared_ptr<ASTExpression> Expression)
{
    this->val = std::move(Expression);
}

ASTReturnStatement::ASTReturnStatement(std::shared_ptr<ASTExpression> Expression)
{
    this->val = std::move(Expression);
}

ASTMultiplicativeOP::ASTMultiplicativeOP(std::shared_ptr<ASTExpression> ExpressionLeft, OP Operator, std::shared_ptr<ASTExpression> ExpressionRight)
{   
    this->valOne = std::move(ExpressionLeft);
    this->operatorStore = Operator;
    this->valTwo = std::move(ExpressionRight);
}

ASTFormalParam::ASTFormalParam(std::shared_ptr<ASTIdentify> Identify, String Type)
{
    this->identify = std::move(Identify);
    this->type = std::move(Type);
}

ASTWhileStatement::ASTWhileStatement(std::shared_ptr<ASTExpression> Expression, std::shared_ptr<ASTBlock> whBlock)
{
    this->val = std::move(Expression);
    this->whileBlock = std::move(whBlock);
}

ASTForStatement::ASTForStatement(std::shared_ptr<ASTVariableDecl> declare, std::shared_ptr<ASTExpression> Expression, std::shared_ptr<ASTAssign> Assignment, std::shared_ptr<ASTBlock> fBlock)
{
    this->varD = std::move(declare);
    this->assign = std::move(Assignment);
    this->val = std::move(Expression);
    this->forBlock = std::move(fBlock);
}

// --- Destroy, functions used pre-XML to check proper deletion (no memory leaks)---

ASTUnary::~ASTUnary()
{
   // std::cout<<"ASTUnary Destroyed: " << checkOperator(this->operatorStore) <<std::endl;
}

ASTRelationalOP::~ASTRelationalOP()
{
   // std::cout<<"ASTRelationalOP Destroyed: " << checkOperator(this->operatorStore) <<std::endl;
}

ASTForStatement::~ASTForStatement()
{
  //  std::cout<<"ASTForStatement Destroyed: " <<std::endl;
}

ASTFunctionCall::~ASTFunctionCall()= default;

ASTFunctionDecl::~ASTFunctionDecl()= default;

ASTFormalParam::~ASTFormalParam()= default;

ASTMultiplicativeOP::~ASTMultiplicativeOP()
{
  //  std::cout<<"ASTMultiplicativeOP Destroyed: " << checkOperator(this->operatorStore) <<std::endl;
}

ASTAdditiveOP::~ASTAdditiveOP()
{
  //   std::cout<<"ASTAdditiveOP Destroyed: " << checkOperator(this->operatorStore) <<std::endl;
}

ASTPrintStatement::~ASTPrintStatement()
{
  //  std::cout<<"ASTPrintStatement Destroyed" <<std::endl;
}

ASTReturnStatement::~ASTReturnStatement()
{
  //  std::cout<<"ASTReturnStatement Destroyed" <<std::endl;
}

ASTBlock::~ASTBlock()
{
    // std::cout<<"ASTBlock Destroyed" <<std::endl;
}

ASTLiteralBool::~ASTLiteralBool()
{
    // std::cout<<"ASTLiteralBool Destroyed: " << this->boolStore <<std::endl;
}

ASTLiteralChar::~ASTLiteralChar(){}


ASTAssign::~ASTAssign()
{
   // std::cout<<"ASTAssign Destroyed" <<std::endl;
}

ASTVariableDecl::~ASTVariableDecl()
{
  //  std::cout << "ASTVariableDecl Destroyed: " << this->type <<std::endl;
}

ASTIdentify::~ASTIdentify()
{
  //  std::cout<<"ASTIdentify Destroyed: "<< this->IdentifyStore <<std::endl;
}

ASTWhileStatement::~ASTWhileStatement()
{
  //  std::cout<<"ASTWhile Destroyed: " << std::endl;
}

ASTProgram::~ASTProgram()
{
  //  std::cout<<"ASTProgram Destroyed"<<std::endl;
}

ASTIfStatement::~ASTIfStatement()
{
  //  std::cout<<"ASTIfStatement Destroyed"<<std::endl;
}


ASTLiteralInt::~ASTLiteralInt()
{
 //   std::cout<<"ASTLiteralInt Destroyed: " << this->intStore <<std::endl;
}

ASTLiteralFloat::~ASTLiteralFloat()= default;


Parser::Parser(const std::vector<String>& file)
{
    myObj = new Lexer(file);
    this->llOne = myObj->getNextToken(); //Preload next token and the one after for ll(1) and ll(2)

    if(!this->llTwo.end)
        this->llTwo = myObj->getNextToken();
    else
        this->EoF=true;
}

void Parser::getNextToken() //always look ahead one, use lexers getNextToken() to update parsers getNextToken();
{
    this->llOne = llTwo;

    if(!this->llTwo.end) //if at end of file dont update ll(2)
        this->llTwo = myObj->getNextToken();
    if(this->llTwo.end)
        this->EoF=true;
    
}

Parser::~Parser() //free parser memory
{   
    //std::cout << "Parser has been destroyed" << std::endl;
    delete(myObj);
}

String ASTExpression::checkOperator(OP op) //return operator tokens as strings
{
    switch(op)
    {
        case oNOT: return "not";
        case oGREATERTHAN: return ">";
        case oLESSTHAN: return "<";
        case oGREATEROREQUAL: return ">=";
        case oLESSOREQUAL: return "<=";
        case oRELATION: return "==";
        case oNOTEQUAL: return "!=";

        case oSUBTRACT: return "-";
        case oPLUS: return "+";
        case oOR: return "or";

        case oSTAR: return "*";
        case oSLASH: return "/";
        case oAND: return "and";

        default: return "Not yet implemented";
    }
}

void Parser::Testlexer() // print each token till end of file
{
    std::cout << "Lexer output:\n" << std::endl;
    while(!this->llOne.end)
    {
        if(!this->llOne.end)
            this->llOne.printResult();

        this->getNextToken();   
    }
}

// Accept functions for visitor design patten.

void ASTProgram::accept(VisitorDesign* v)
{
    v->visit(this);
}

void ASTAssign::accept(VisitorDesign* v)
{
    v->visit(this);
}

void ASTLiteralBool::accept(VisitorDesign* v)
{
    v->visit(this);
}

void ASTLiteralInt::accept(VisitorDesign* v)
{
    v->visit(this);
}

void ASTLiteralFloat::accept(VisitorDesign* v)
{
    v->visit(this);
}

void ASTLiteralChar::accept(VisitorDesign* v)
{
    v->visit(this);
}

void::ASTUnary::accept(VisitorDesign* v)
{
    v->visit(this);
}

void::ASTExpression::accept(VisitorDesign* v){}

void::ASTLiteral::accept(VisitorDesign* v){}

void::ASTStatement::accept(VisitorDesign* v){}

void::ASTIfStatement::accept(VisitorDesign* v)
{
    v->visit(this);
}

void::ASTWhileStatement::accept(VisitorDesign* v)
{
    v->visit(this);
}

void::ASTIdentify::accept(VisitorDesign* v)
{
    v->visit(this);
}

void::ASTVariableDecl::accept(VisitorDesign* v)
{
    v->visit(this);
}

void::ASTReturnStatement::accept(VisitorDesign* v)
{
    v->visit(this);
}

void::ASTPrintStatement::accept(VisitorDesign* v)
{
    v->visit(this);
}

void::ASTFormalParam::accept(VisitorDesign* v)
{
    v->visit(this);
}

void::ASTFunctionDecl::accept(VisitorDesign* v)
{
    v->visit(this);
}

void::ASTFunctionCall::accept(VisitorDesign* v)
{
    v->visit(this);
}

void::ASTForStatement::accept(VisitorDesign* v)
{
    v->visit(this);
}

void::ASTRelationalOP::accept(VisitorDesign* v)
{
    v->visit(this);
}

void::ASTAdditiveOP::accept(VisitorDesign* v)
{
    v->visit(this);

}

void::ASTBlock::accept(VisitorDesign* v)
{
    v->visit(this);
}

void::ASTMultiplicativeOP::accept(VisitorDesign* v)
{
    v->visit(this);
}

