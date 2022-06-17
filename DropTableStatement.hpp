//
// Created by smith on 5/3/2022.
//

#ifndef ASSIGNMENT5_GROUP11_DROPTABLESTATEMENT_HPP
#define ASSIGNMENT5_GROUP11_DROPTABLESTATEMENT_HPP

#include "Statement.hpp"
#include "keywords.hpp"
#include "Tokenizer.hpp"
#include "SQLProcessor.hpp"
#include "Database.hpp"

namespace ECE141 {

    class DropTableStatement : public Statement {
    public:
        DropTableStatement(SQLProcessor *aProc, Tokenizer &aTokenizer);
        DropTableStatement(const DropTableStatement &aCopy);

        ~DropTableStatement() override;

        StatusResult  parse(Tokenizer &aTokenizer) override;

        Keywords                getType() const override {return stmtType;}
        virtual   const char*   getStatementName() const override{return "statement";}
        virtual   StatusResult  run(std::ostream &aStream)override;

    protected:
        Keywords stmtType;
        std::string tableName;
        SQLProcessor *processor;
    };
    bool validDropSQLCommand(Tokenizer &aTokenizer);
}

#endif //ASSIGNMENT5_GROUP11_DROPTABLESTATEMENT_HPP
