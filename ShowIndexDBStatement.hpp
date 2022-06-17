//
// Created by smith on 5/29/2022.
//

#ifndef ASSIGNMENT9_GROUP11_SHOWINDEXDBSTATEMENT_HPP
#define ASSIGNMENT9_GROUP11_SHOWINDEXDBSTATEMENT_HPP

#include "Statement.hpp"
#include "DBProcessor.hpp"

namespace ECE141 {
    class ShowIndexDBStatement : public Statement {
    public:
        ShowIndexDBStatement(DBProcessor *aProc, Tokenizer &aTokenizer);
        ShowIndexDBStatement(const ShowIndexDBStatement &aCopy);

        ~ShowIndexDBStatement() override;

        StatusResult  parse(Tokenizer &aTokenizer) override;

        Keywords                getType() const override {return stmtType;}
        virtual   const char*   getStatementName() const override{return "statement";}
        virtual   StatusResult  run(std::ostream &aStream)override;

    protected:
        Keywords stmtType;
        DBProcessor *processor;
    };
    bool validShowIndexCommand(Tokenizer &aTokenizer);
}



#endif //ASSIGNMENT9_GROUP11_SHOWINDEXDBSTATEMENT_HPP
