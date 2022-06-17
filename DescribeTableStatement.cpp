//
// Created by smith on 5/3/2022.
//

#include "DescribeTableStatement.hpp"

namespace ECE141 {
    DescribeTableStatement::DescribeTableStatement(SQLProcessor *aProc, Tokenizer &aTokenizer)
            : processor(aProc), stmtType(aTokenizer.current().keyword)  {
    }

    DescribeTableStatement::DescribeTableStatement(const DescribeTableStatement &aCopy)
            : processor(aCopy.processor), stmtType(aCopy.stmtType) {
    }

    DescribeTableStatement::~DescribeTableStatement()=default;

    //USE: -------------------
    StatusResult DescribeTableStatement::parse(Tokenizer &aTokenizer) {
        if (validDescribeSQLCommand(aTokenizer)) {
            aTokenizer.next();
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

    StatusResult DescribeTableStatement::run(std::ostream &aStream) {
        return processor->describeTable(tableName);
    }

    bool validDescribeSQLCommand(Tokenizer &aTokenizer) {
        return Keywords::describe_kw == aTokenizer.current().keyword
               && TokenType::identifier == aTokenizer.peek().type;
    }
}