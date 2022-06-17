//
// Created by smith on 5/2/2022.
//

#ifndef ASSIGNMENT5_GROUP11_USEDBSTATEMENT_HPP
#define ASSIGNMENT5_GROUP11_USEDBSTATEMENT_HPP


#include "Statement.hpp"
#include "keywords.hpp"
#include "Tokenizer.hpp"
#include "DBProcessor.hpp"

namespace ECE141 {

    class UseDBStatement : public Statement {
    public:
        UseDBStatement(DBProcessor *aProc, Tokenizer &aTokenizer);
        UseDBStatement(const UseDBStatement &aCopy);

        ~UseDBStatement() override;

        StatusResult  parse(Tokenizer &aTokenizer) override;

        Keywords                getType() const override {return stmtType;}
        std::string             getName() const {return tableName;}
        virtual   const char*   getStatementName() const override{return "statement";}
        virtual   StatusResult  run(std::ostream &aStream)override;

    protected:
        Keywords stmtType;
        std::string tableName;
        DBProcessor *processor;
    };

    bool validUseDBCommand(Tokenizer &aTokenizer);
}


#endif //ASSIGNMENT5_GROUP11_USEDBSTATEMENT_HPP
