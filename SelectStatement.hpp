//
// Created by smith on 5/2/2022.
//

#ifndef ASSIGNMENT5_GROUP11_SELECTSTATEMENT_HPP
#define ASSIGNMENT5_GROUP11_SELECTSTATEMENT_HPP

#include "Statement.hpp"

#include "Statement.hpp"
#include "keywords.hpp"
#include "Tokenizer.hpp"
#include "SQLProcessor.hpp"
#include "Database.hpp"
#include "BasicTypes.hpp"
#include "Helpers.hpp"
#include "Filters.hpp"


namespace ECE141 {
    struct Join  {
        Join(const std::string &aTable, Keywords aType)
        : table(aTable), joinType(aType) {}
            
        Keywords    joinType;
        std::string table;
        Operand  onLeft;
        Operand  onRight;
    };

    using JoinList = std::vector<Join>;

    class SelectStatement : public Statement {
    public:
        SelectStatement(SQLProcessor *aProc, Tokenizer &aTokenizer);
        SelectStatement(const SelectStatement &aCopy);

        ~SelectStatement() override;
        StatusResult makeQuery(Tokenizer &aTokenizer);
        StatusResult makeAllQuery(Tokenizer &aTokenizer);
        StatusResult makeAttributeQuery(Tokenizer &aTokenizer);
        StatusResult makeFilter(Tokenizer &aTokenizer);

        StatusResult  parse(Tokenizer &aTokenizer) override;

        Keywords                getType() const override {return stmtType;}
        virtual   const char*   getStatementName() const override {return "statement";}
                                StatusResult   run(std::ostream &aStream) override ;



    protected:
        Keywords stmtType;
        RowCollection rows;
        Query *query;
        SQLProcessor *processor;
        JoinList joins;
    };
    bool validSelectAllSQLCommand(Tokenizer &aTokenizer);
}


#endif //ASSIGNMENT5_GROUP11_SELECTSTATEMENT_HPP
