#include "SymTable.hpp"
//-- VisitorDesign Patterns for XML, Semantic Analysis and Interp ---
class VisitorDesign
{
    public:
        virtual void visit(ASTProgram *) = 0;
        virtual void visit(ASTAssign *) = 0;
        virtual void visit(ASTIdentify *) = 0;
        virtual void visit(ASTLiteral *) = 0;
        virtual void visit(ASTLiteralBool *) = 0;
        virtual void visit(ASTLiteralInt *) = 0;
        virtual void visit(ASTLiteralFloat *) = 0; 
        virtual void visit(ASTLiteralChar *) = 0;

        virtual void visit(ASTReturnStatement *) = 0;
        virtual void visit(ASTWhileStatement *) = 0;
        virtual void visit(ASTVariableDecl *) = 0;
        virtual void visit(ASTUnary *) = 0;
        virtual void visit(ASTForStatement *) = 0;
        virtual void visit(ASTRelationalOP *) = 0;
        virtual void visit(ASTAdditiveOP *) = 0;
        virtual void visit(ASTMultiplicativeOP *) = 0;
        virtual void visit(ASTBlock *) = 0;
        virtual void visit(ASTPrintStatement *) = 0;
        virtual void visit(ASTIfStatement *) = 0;
        virtual void visit(ASTFormalParam *) = 0;
        virtual void visit(ASTFunctionDecl *) = 0;
        virtual void visit(ASTFunctionCall *) = 0;
      
};


class XMLVisitorDesign: public VisitorDesign
{
    public:
        XMLVisitorDesign();
        ~XMLVisitorDesign();

        void printIndent() const;
        void visit(ASTProgram *) override;
        void visit(ASTAssign *) override;
        void visit(ASTIdentify *) override;
        void visit(ASTLiteral *) override;
        void visit(ASTLiteralBool *) override;
        void visit(ASTLiteralInt *) override;
        void visit(ASTLiteralFloat *) override;
        void visit(ASTLiteralChar *) override;

        void visit(ASTReturnStatement *) override;
        void visit(ASTVariableDecl *) override;
        void visit(ASTUnary *) override;
        void visit(ASTForStatement *) override;

        void visit(ASTRelationalOP *) override;
        void visit(ASTAdditiveOP *) override;
        void visit(ASTMultiplicativeOP *) override;

        void visit(ASTBlock *) override;
        void visit(ASTPrintStatement *) override;
        void visit(ASTIfStatement *) override;
        void visit(ASTWhileStatement *) override;
        void visit(ASTFormalParam *) override;
        void visit(ASTFunctionDecl *) override;
        void visit(ASTFunctionCall *) override;

    private:
        int visited; //store the visited nodes to print indents accordingly


};

class SemanticVisitorDesign: public VisitorDesign
{
    public:
        SemanticVisitorDesign();
        ~SemanticVisitorDesign();

        void visit(ASTProgram *) override;
        void visit(ASTAssign *) override;
        void visit(ASTIdentify *) override;
        void visit(ASTLiteral *) override;
        void visit(ASTLiteralBool *) override;
        void visit(ASTLiteralInt *) override;
        void visit(ASTLiteralFloat *) override;
        void visit(ASTLiteralChar *) override;

        void visit(ASTReturnStatement *) override;
        void visit(ASTVariableDecl *) override;
        void visit(ASTUnary *) override;
        void visit(ASTForStatement *) override;

        void visit(ASTRelationalOP *) override;
        void visit(ASTAdditiveOP *) override;
        void visit(ASTMultiplicativeOP *) override;

        void visit(ASTBlock *) override;
        void visit(ASTPrintStatement *) override;
        void visit(ASTIfStatement *) override;
        void visit(ASTWhileStatement *) override;
        void visit(ASTFormalParam *) override;
        void visit(ASTFunctionDecl *) override; 
        void visit(ASTFunctionCall *) override;

        SymTable myTable;
        
        private: 
            String getType;
            String getOpType;
       
};

class InterpreterPass: public VisitorDesign
{
    public:
        InterpreterPass();
        ~InterpreterPass();

        void visit(ASTProgram *) override;
        void visit(ASTAssign *) override;
        void visit(ASTIdentify *) override;
        void visit(ASTLiteral *) override;
        void visit(ASTLiteralBool *) override;
        void visit(ASTLiteralInt *) override;
        void visit(ASTLiteralFloat *) override;
        void visit(ASTLiteralChar *) override;

        void visit(ASTReturnStatement *) override;
        void visit(ASTVariableDecl *) override;
        void visit(ASTUnary *) override;
        void visit(ASTForStatement *) override;

        void visit(ASTRelationalOP *) override;
        void visit(ASTAdditiveOP *) override;
        void visit(ASTMultiplicativeOP *) override;

        void visit(ASTBlock *) override;
        void visit(ASTPrintStatement *) override;
        void visit(ASTIfStatement *) override;
        void visit(ASTWhileStatement *) override;
        void visit(ASTFormalParam *) override;
        void visit(ASTFunctionDecl *) override; 
        void visit(ASTFunctionCall *) override;

        SymTable myTable; //initilise the symbol table
        
        private: 
            String getType; //store value types
            String getOpType; //store operator types
            
            //store the actual values
            float actualFloat{};
            int actualInt{};
            String actualString;
            bool actualBool{};

       
};


