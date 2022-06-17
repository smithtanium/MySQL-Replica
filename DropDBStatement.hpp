//
// Created by smith on 5/2/2022.
//

#ifndef ASSIGNMENT5_GROUP11_DROPDBSTATEMENT_HPP
#define ASSIGNMENT5_GROUP11_DROPDBSTATEMENT_HPP

#include "Statement.hpp"
#include "keywords.hpp"
#include "Tokenizer.hpp"
#include "DBProcessor.hpp"

namespace ECE141 {

    class DropDBStatement : public Statement {
    public:
                                DropDBStatement(DBProcessor *aProc, Tokenizer &aTokenizer);
                                DropDBStatement(const DropDBStatement &aCopy);
                                ~DropDBStatement() override;

                  StatusResult  parse(Tokenizer &aTokenizer) override;
                  std::string   getName() const {return tableName;}

        Keywords                getType() const override {return stmtType;}
        virtual   const char*   getStatementName() const override{return "statement";}
        virtual   StatusResult  run(std::ostream &aStream)override;

    protected:
        Keywords stmtType;
        std::string tableName;
        DBProcessor *processor;

    };
    bool validDropDBCommand(Tokenizer &aTokenizer);
}


#endif //ASSIGNMENT5_GROUP11_DROPDBSTATEMENT_HPP
