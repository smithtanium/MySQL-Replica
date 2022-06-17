//
// Created by smith on 5/3/2022.
//

#ifndef ShowIndexStatement_hpp
#define ShowIndexStatement_hpp

#include "Statement.hpp"
#include "keywords.hpp"
#include "Tokenizer.hpp"
#include "SQLProcessor.hpp"
#include "Database.hpp"
#include "ShowTableStatement.hpp"


namespace ECE141 {

    class ShowIndexStatement : public Statement {
    public:
        ShowIndexStatement(SQLProcessor *aProc, Tokenizer &aTokenizer);
        ShowIndexStatement(const ShowIndexStatement &aCopy);

        ~ShowIndexStatement() override;

        StatusResult  parse(Tokenizer &aTokenizer) override;

        Keywords                getType() const override {return stmtType;}
        virtual   const char*   getStatementName() const override{return "statement";}
        virtual   StatusResult  run(std::ostream &aStream) override ;

    protected:
        Keywords stmtType;
        std::string tableName;
        SQLProcessor *processor;
        std::string attributeName;
        std::vector<std::string> attributeNameCollections;
    };

}

#endif
