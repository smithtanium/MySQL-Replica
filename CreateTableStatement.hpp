//
// Created by smith on 5/3/2022.
//

#ifndef ASSIGNMENT5_GROUP11_CREATETABLESTATEMENT_HPP
#define ASSIGNMENT5_GROUP11_CREATETABLESTATEMENT_HPP

#include "Statement.hpp"
#include "keywords.hpp"
#include "Tokenizer.hpp"
#include "SQLProcessor.hpp"
#include "Database.hpp"

namespace ECE141 {

    class CreateTableStatement : public Statement {
    public:
        CreateTableStatement(SQLProcessor *aProc, Tokenizer &aTokenizer);
        CreateTableStatement(const CreateTableStatement &aCopy);

        ~CreateTableStatement() override;

        StatusResult  parse(Tokenizer &aTokenizer) override;
        StatusResult makeEntity(Tokenizer &aTokenizer);

        Keywords                getType() const override {return stmtType;}
        virtual   const char*   getStatementName() const override {return "statement";}
        virtual   StatusResult  run(std::ostream &aStream) override;

    protected:
        Keywords stmtType;
        std::string tableName;
        Entity *entity;
        SQLProcessor *processor;
    };
    bool validCreateSQLCommand(Tokenizer &aTokenizer);
}


#endif //ASSIGNMENT5_GROUP11_CREATETABLESTATEMENT_HPP
