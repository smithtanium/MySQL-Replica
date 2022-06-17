//
// Created by smith on 5/3/2022.
//

#include "ShowTableStatement.hpp"

namespace ECE141 {
    ShowTableStatement::ShowTableStatement(SQLProcessor *aProc, Tokenizer &aTokenizer)
            : processor(aProc), stmtType(aTokenizer.current().keyword)  {
    }

    ShowTableStatement::ShowTableStatement(const ShowTableStatement &aCopy)
            : processor(aCopy.processor), stmtType(aCopy.stmtType) {
    }

    ShowTableStatement::~ShowTableStatement() = default;

    //USE: -------------------
    StatusResult ShowTableStatement::parse(Tokenizer &aTokenizer) {
        if (validShowSQLCommand(aTokenizer)) {
            aTokenizer.next(1);
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

    StatusResult ShowTableStatement::run(std::ostream &aStream) {
        return processor->showTables();
    }

    bool validShowSQLCommand(Tokenizer &aTokenizer) {
        return Keywords::show_kw == aTokenizer.current().keyword
               && Keywords::tables_kw==aTokenizer.peek().keyword || 
               Keywords::show_kw == aTokenizer.current().keyword
               && Keywords::index_kw == aTokenizer.peek().keyword ||
               Keywords::show_kw == aTokenizer.current().keyword
               && Keywords::indexes_kw == aTokenizer.peek().keyword ;
    }
}

