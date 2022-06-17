//
// Created by smith on 5/15/2022.
//

#ifndef ASSIGNMENT_7_GROUP11_UPDATESTATEMENT_HPP
#define ASSIGNMENT_7_GROUP11_UPDATESTATEMENT_HPP

#include "Statement.hpp"
#include "Tokenizer.hpp"
#include "SQLProcessor.hpp"
#include "Database.hpp"

namespace ECE141 {
    class UpdateStatement : public Statement {
    public:
        UpdateStatement(SQLProcessor *aProc, Tokenizer &aTokenizer);
        UpdateStatement(const UpdateStatement &aCopy);

        ~UpdateStatement() override;
//        StatusResult makeQuery(Tokenizer &aTokenizer);
//        StatusResult makeAllQuery(Tokenizer &aTokenizer);
//        StatusResult makeAttributeQuery(Tokenizer &aTokenizer);
//        StatusResult makeFilter(Tokenizer &aTokenizer);

        StatusResult  parse(Tokenizer &aTokenizer) override;

        Keywords                getType() const override {return stmtType;}
        virtual   const char*   getStatementName() const override{return "statement";}
        StatusResult  run(std::ostream &aStream)override;

    protected:
        Keywords stmtType;
        Query *query;
        std::string name;
        RowCollection rows;
        Value value;
        SQLProcessor *processor;
    };
    bool validUpdateCommand(Tokenizer &aTokenizer);
}

#endif //ASSIGNMENT_7_GROUP11_UPDATESTATEMENT_HPP
