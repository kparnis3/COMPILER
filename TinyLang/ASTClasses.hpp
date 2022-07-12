#include <memory>
class VisitorDesign;

enum OP
{
    oSTAR = 0,
    oSUBTRACT,
    oPLUS,
    oSLASH,

    oAND,
    oOR,
    oNOT,

    oLESSTHAN,
    oGREATERTHAN,
    oLESSOREQUAL,
    oGREATEROREQUAL,
    oEQUAL,
    oRELATION,
    oNOTEQUAL

};

// --- The following are a hierarchy of class nodes, each of which stem from ASTNode, all theses classes 
//     receive an acceptance override to allow the visiter pointer to traverse through each node ---

class ASTNode
{   
    public:
        virtual void accept(VisitorDesign* v) = 0;
};

class ASTExpression: public ASTNode
{
    public:
        String checkOperator(OP op); //helper function that recieves a string operator symbol
        void accept(VisitorDesign* v) override;
        
};

class ASTStatement: public ASTNode
{
    public:
        void accept(VisitorDesign* v) override;
};

class ASTLiteral: public ASTExpression
{
    public:
    void accept(VisitorDesign* v) override;
};

class ASTProgram: public ASTNode
{
    public:
        ASTProgram(std::vector<std::shared_ptr<ASTStatement>> AllStatements);
        ~ASTProgram();
        std::vector<std::shared_ptr<ASTStatement>> program; //store array of shared pointers

        void accept(VisitorDesign* v) override;
};

class ASTIdentify: public ASTExpression
{
    public:
        ASTIdentify();
        ~ASTIdentify();
        String IdentifyStore; //Store identifier
        void accept(VisitorDesign* v) override;
};

class ASTUnary: public ASTExpression
{
    public:
        ASTUnary(OP Operator, std::shared_ptr<ASTExpression> Expression);
        ~ASTUnary();

        OP operatorStore; //store the operator
        std::shared_ptr<ASTExpression> val = std::make_shared<ASTExpression>(); //store shared pointer for expression
        void accept(VisitorDesign* v) override;
          
};

class ASTRelationalOP: public ASTExpression
{
    public:
        ASTRelationalOP(std::shared_ptr<ASTExpression> ExpressionLeft, OP Operator, std::shared_ptr<ASTExpression> ExpressionRight);
        ~ASTRelationalOP();

        OP operatorStore;
        std::shared_ptr<ASTExpression> valOne = std::make_shared<ASTExpression>();
        std::shared_ptr<ASTExpression> valTwo = std::make_shared<ASTExpression>(); 
        void accept(VisitorDesign* v) override;
        
};

class ASTAdditiveOP: public ASTExpression
{
    public:
        ASTAdditiveOP(std::shared_ptr<ASTExpression> ExpressionLeft, OP Operator, std::shared_ptr<ASTExpression> ExpressionRight);
        ~ASTAdditiveOP();

        OP operatorStore;
        std::shared_ptr<ASTExpression> valOne = std::make_shared<ASTExpression>();
        std::shared_ptr<ASTExpression> valTwo = std::make_shared<ASTExpression>(); 
        void accept(VisitorDesign* v) override;
        
};

class ASTMultiplicativeOP: public ASTExpression
{
    public:
        ASTMultiplicativeOP(std::shared_ptr<ASTExpression> ExpressionLeft, OP Operator, std::shared_ptr<ASTExpression> ExpressionRight);
        ~ASTMultiplicativeOP();

        OP operatorStore;
        std::shared_ptr<ASTExpression> valOne = std::make_shared<ASTExpression>();
        std::shared_ptr<ASTExpression> valTwo = std::make_shared<ASTExpression>(); 
        void accept(VisitorDesign* v) override;
        
};


class ASTAssign: public ASTStatement
{
    public:
        ASTAssign(std::shared_ptr<ASTIdentify> Identify, std::shared_ptr<ASTExpression> Expression);
        ASTAssign();
        ~ASTAssign();
        void accept(VisitorDesign* v) override;
        std::shared_ptr<ASTIdentify> identify = std::make_shared<ASTIdentify>();
        std::shared_ptr<ASTExpression> val = std::make_shared<ASTExpression>();

};

class ASTVariableDecl: public ASTStatement
{
    public:
        ASTVariableDecl(std::shared_ptr<ASTIdentify> Identify, String Type, std::shared_ptr<ASTExpression> Expression);
        ASTVariableDecl();
        ~ASTVariableDecl();
        std::shared_ptr<ASTIdentify> identify = std::make_shared<ASTIdentify>();
        String type; //store declaration type
        std::shared_ptr<ASTExpression> val = std::make_shared<ASTExpression>();
        void accept(VisitorDesign* v) override;
};

class ASTPrintStatement: public ASTStatement
{
    public:
        ASTPrintStatement(std::shared_ptr<ASTExpression> Expression);
        ~ASTPrintStatement();
        std::shared_ptr<ASTExpression> val = std::make_shared<ASTExpression>();
        void accept(VisitorDesign* v) override;
};

class ASTBlock: public ASTStatement
{
    public:
        ASTBlock(std::vector<std::shared_ptr<ASTStatement>> AllStatements);
        ASTBlock();
        ~ASTBlock();
        std::vector<std::shared_ptr<ASTStatement>> block;
        void accept(VisitorDesign* v) override;

};


class ASTLiteralFloat: public ASTLiteral
{
    public:
    String floatStore; //store float as string
        ASTLiteralFloat();
        ~ASTLiteralFloat();
        void accept (VisitorDesign* v) override;

};

class ASTLiteralChar: public ASTLiteral
{
    public:
        String StringStore; //store string
        ASTLiteralChar();
        ~ASTLiteralChar();
        void accept (VisitorDesign* v) override;
};

class ASTLiteralBool: public ASTLiteral
{
    public:
        bool boolStore; //store bool
        ASTLiteralBool();
        ~ASTLiteralBool();
        void accept(VisitorDesign* v) override;
};

class ASTLiteralInt: public ASTLiteral
{
    public:
        String intStore; //store int as string
        ASTLiteralInt();
        ~ASTLiteralInt();
        void accept(VisitorDesign* v) override;
};


class ASTIfStatement: public ASTStatement
{
    public:
        ASTIfStatement(std::shared_ptr<ASTExpression> Expression, std::shared_ptr<ASTBlock> ifB, std::shared_ptr<ASTBlock> elseB);
        ~ASTIfStatement();
        std::shared_ptr<ASTBlock> elseBlock = std::make_shared<ASTBlock>();
        std::shared_ptr<ASTBlock> ifBlock = std::make_shared<ASTBlock>();
        std::shared_ptr<ASTExpression> val = std::make_shared<ASTExpression>();
        void accept(VisitorDesign* v) override;
       
};

class ASTForStatement: public ASTStatement
{
    public:
        ASTForStatement(std::shared_ptr<ASTVariableDecl> declare, std::shared_ptr<ASTExpression> Expression, std::shared_ptr<ASTAssign> Assignment, std::shared_ptr<ASTBlock> fBlock);
        ~ASTForStatement();

        std::shared_ptr<ASTVariableDecl> varD = std::make_shared<ASTVariableDecl>();
        std::shared_ptr<ASTAssign> assign = std::make_shared<ASTAssign>();
        std::shared_ptr<ASTExpression> val = std::make_shared<ASTExpression>();
        std::shared_ptr<ASTBlock> forBlock = std::make_shared<ASTBlock>();
        void accept(VisitorDesign* v) override;

};

class ASTWhileStatement: public ASTStatement
{
    public:
        ASTWhileStatement(std::shared_ptr<ASTExpression> Expression, std::shared_ptr<ASTBlock> whBlock);
        ~ASTWhileStatement();
        std::shared_ptr<ASTBlock> whileBlock = std::make_shared<ASTBlock>();
        std::shared_ptr<ASTExpression> val = std::make_shared<ASTExpression>();
        void accept(VisitorDesign* v) override;
};

class ASTReturnStatement: public ASTStatement
{
    public:
        ASTReturnStatement(std::shared_ptr<ASTExpression> Expression);
        ~ASTReturnStatement();
        std::shared_ptr<ASTExpression> val = std::make_shared<ASTExpression>();
        void accept(VisitorDesign* v) override;
};

class ASTFormalParam: public ASTNode
{
    public:
        ASTFormalParam(std::shared_ptr<ASTIdentify> Identify, String Type);
        ASTFormalParam();
        ~ASTFormalParam();
        void accept(VisitorDesign* v) override;

        std::shared_ptr<ASTIdentify> identify = std::make_shared<ASTIdentify>();
        String type;

};

class ASTFunctionDecl: public ASTStatement
{
    public:
    
        ASTFunctionDecl(std::shared_ptr<ASTIdentify> Identify, std::vector<std::shared_ptr<ASTFormalParam>> par ,String Type, std::shared_ptr<ASTBlock> fBlock);
        ~ASTFunctionDecl();


        std::shared_ptr<ASTIdentify> identify = std::make_shared<ASTIdentify>();
        std::shared_ptr<ASTBlock> functionBlock = std::make_shared<ASTBlock>();
        std::vector<std::shared_ptr<ASTFormalParam>> param;

        String type; //store function decl type
        void accept(VisitorDesign* v) override;
};

class ASTFunctionCall :public ASTExpression
{
    public:
        ASTFunctionCall(std::shared_ptr<ASTIdentify> Identify, std::vector<std::shared_ptr<ASTExpression>> par);
        ~ASTFunctionCall();

        void accept(VisitorDesign* v) override;
        
        std::shared_ptr<ASTIdentify> identify = std::make_shared<ASTIdentify>();
        std::vector<std::shared_ptr<ASTExpression>> param;
        

};


