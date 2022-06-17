//
// Created by smith on 5/3/2022.
//

#include "DropTableStatement.hpp"

namespace ECE141 {
    DropTableStatement::DropTableStatement(SQLProcessor *aProc, Tokenizer &aTokenizer)
            : processor(aProc), stmtType(aTokenizer.current().keyword)  {
    }

    DropTableStatement::DropTableStatement(const DropTableStatement &aCopy)
            : processor(aCopy.processor), stmtType(aCopy.stmtType) {
    }

    DropTableStatement::~DropTableStatement() = default;

    //USE: -------------------
    StatusResult DropTableStatement::parse(Tokenizer &aTokenizer) {
        if (validDropSQLCommand(aTokenizer)) {
            aTokenizer.next(2);
            tableName = aTokenizer.current().data;
            return StatusResult{noError};
        }
        else {
            while (aTokenizer.more()) {
                aTokenizer.next();
            }
            return StatusResult{invalidCommand};
        }
    }

    StatusResult DropTableStatement::run(std::ostream &aStream) {
        return processor->dropTable(tableName);
    }

    bool validDropSQLCommand(Tokenizer &aTokenizer) {
        return Keywords::drop_kw == aTokenizer.current().keyword
               && Keywords::table_kw==aTokenizer.peek().keyword
               && TokenType::identifier == aTokenizer.peek(2).type;
    }
}