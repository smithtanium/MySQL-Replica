//
// Created by smith on 5/2/2022.
//

#include "DropDBStatement.hpp"

namespace ECE141 {
    DropDBStatement::DropDBStatement(DBProcessor *aProc, Tokenizer &aTokenizer)
            : processor(aProc), stmtType(aTokenizer.current().keyword)  {
    }

    DropDBStatement::DropDBStatement(const DropDBStatement &aCopy)
            : processor(aCopy.processor), stmtType(aCopy.stmtType) {
    }

    DropDBStatement::~DropDBStatement()=default;

    //USE: -------------------
    StatusResult DropDBStatement::parse(Tokenizer &aTokenizer) {
        if (validDropDBCommand(aTokenizer)) {
            aTokenizer.next(2);
            tableName = aTokenizer.current().data;
            return processor->setDBName(aTokenizer.current().data);
        }
        return StatusResult{invalidCommand};
    }

    StatusResult DropDBStatement::run(std::ostream &aStream) {
        return processor->dropDatabase(tableName);
    }

    bool validDropDBCommand(Tokenizer &aTokenizer) {
        return Keywords::drop_kw == aTokenizer.current().keyword
               && Keywords::database_kw==aTokenizer.peek().keyword
               && TokenType::identifier == aTokenizer.peek(2).type
               && ";" == aTokenizer.peek(3).data;
    }
}