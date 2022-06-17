//
// Created by smith on 5/15/2022.
//

#ifndef ASSIGNMENT_7_GROUP11_DELETESTATEMENT_HPP
#define ASSIGNMENT_7_GROUP11_DELETESTATEMENT_HPP

#include "Statement.hpp"
#include "Tokenizer.hpp"
#include "SQLProcessor.hpp"

namespace ECE141 {
    class DeleteStatement : public Statement {
    public:
        DeleteStatement(SQLProcessor *aProc, Tokenizer &aTokenizer);
        DeleteStatement(const DeleteStatement &aCopy);

        ~DeleteStatement() override;

        StatusResult  parse(Tokenizer &aTokenizer) override;

        Keywords                getType() const override {return stmtType;}
        virtual   const char*   getStatementName() const override{return "statement";}
        StatusResult  run(std::ostream &aStream)override;

    protected:
        Keywords stmtType;
        Query *query;
        RowCollection rows;
        SQLProcessor *processor;
    };
    bool validDeleteCommand(Tokenizer &aTokenizer);
}


#endif //ASSIGNMENT_7_GROUP11_DELETESTATEMENT_HPP
