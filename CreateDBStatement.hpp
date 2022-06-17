//
// Created by smith on 5/2/2022.
//

#ifndef ASSIGNMENT5_GROUP11_CREATEDBSTATEMENT_HPP
#define ASSIGNMENT5_GROUP11_CREATEDBSTATEMENT_HPP

#include "Statement.hpp"
#include "keywords.hpp"
#include "Tokenizer.hpp"
#include "DBProcessor.hpp"

namespace ECE141 {

    class CreateDBStatement : public Statement {
    public:
        CreateDBStatement(DBProcessor *aProc, Tokenizer &aTokenizer);
        CreateDBStatement(const CreateDBStatement &aCopy);

                 ~CreateDBStatement() override;

        StatusResult  parse(Tokenizer &aTokenizer) override;

        Keywords                getType() const override {return stmtType;}
        virtual   const char*   getStatementName() const override{return "statement";}
        virtual   StatusResult  run(std::ostream &aStream)override;

    protected:
        Keywords stmtType;
        DBProcessor *processor;
    };
    bool validCreateDBCommand(Tokenizer &aTokenizer);
}

#endif //ASSIGNMENT5_GROUP11_CREATEDBSTATEMENT_HPP