//
// Created by smith on 5/3/2022.
//

#ifndef ASSIGNMENT5_GROUP11_INSERTSTATEMENT_HPP
#define ASSIGNMENT5_GROUP11_INSERTSTATEMENT_HPP

#include "Statement.hpp"
#include "keywords.hpp"
#include "Tokenizer.hpp"
#include "SQLProcessor.hpp"
#include "Database.hpp"

namespace ECE141 {

    class InsertStatement : public Statement {
    public:
        InsertStatement(SQLProcessor *aProc, Tokenizer &aTokenizer);
        InsertStatement(const InsertStatement &aCopy);

        ~InsertStatement() override;

        StatusResult  parse(Tokenizer &aTokenizer) override;

        Keywords                getType() const override {return stmtType;}
        virtual   const char*   getStatementName() const override{return "statement";}
        virtual   StatusResult  run(std::ostream &aStream)override;

        StatusResult makeRows(Tokenizer &aTokenizer);
        std::shared_ptr<Row> makeRow(Tokenizer &aTokenizer, StringList &aName);

    protected:
        Keywords stmtType;
        RowCollection rows;
        std::string tableName;
        SQLProcessor *processor;
    };
    bool validInsertSQLCommand(Tokenizer &aTokenizer);

    StringList getAttributeNames(Tokenizer &aTokenizer);
}


#endif //ASSIGNMENT5_GROUP11_INSERTSTATEMENT_HPP
