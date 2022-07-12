#include "VisitorDesign.hpp"

class Parser
{
    public:
        Parser(const std::vector<String>& file);
        ~Parser();
        std::shared_ptr<ASTProgram> BeginParse();
        
        void Testlexer(); //used to test the lexer
        void getNextToken(); //get token from lexer

        //-- initial passes go through parser --
        void XMLPass();
        void SemanticPass();
        void InterpretPass();

        std::vector<std::shared_ptr<ASTStatement>> program;

        //-- helper functions --
        bool checkRelation(MyTokens Token);
        OP getRelation(MyTokens Token); //OP -> operator

        bool checkAdditive(MyTokens Token);
        OP getAdditive(MyTokens Token);

        bool checkMultiplicative(MyTokens Token);
        OP getMultiplicative(MyTokens Token);

        bool checkType(MyTokens Token);
    private:
        Lexer* myObj;
        Token llOne; // ll(1) 
        Token llTwo; // ll(2)
        bool EoF=false;

        // --All the functions whoch manage parsing --
        
        std::shared_ptr<ASTStatement> stateParsing();
        std::shared_ptr<ASTVariableDecl> variableDeclParsing();
        std::shared_ptr<ASTAssign> assignParsing();
        std::shared_ptr<ASTBlock> blockParsing();
        std::shared_ptr<ASTIdentify> identifyParsing();
        std::shared_ptr<ASTPrintStatement> printParsing();
        std::shared_ptr<ASTReturnStatement> returnParsing();
        std::shared_ptr<ASTIfStatement> ifParsing();
        std::shared_ptr<ASTWhileStatement> whileParsing();
        std::shared_ptr<ASTForStatement> forParsing();

        std::shared_ptr<ASTExpression> simpleexpressionParsing();
        std::shared_ptr<ASTExpression> subExpressionParsing();
        std::shared_ptr<ASTExpression> termParsing();
        std::shared_ptr<ASTExpression> factorParsing();
        std::shared_ptr<ASTExpression> expressionParsing();
        std::shared_ptr<ASTUnary> unaryParsing();

        std::shared_ptr<ASTLiteral> literalParsing();
        std::shared_ptr<ASTFunctionDecl> functionParsing();
        std::shared_ptr<ASTFormalParam> formalParsing();
        std::shared_ptr<ASTFunctionCall> functionCallParsing();
};









