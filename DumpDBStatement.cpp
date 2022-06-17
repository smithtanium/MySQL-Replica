//
// Created by smith on 5/2/2022.
//

#include "DumpDBStatement.hpp"

namespace ECE141 {
    DumpDBStatement::DumpDBStatement(DBProcessor *aProc, Tokenizer &aTokenizer)
            : processor(aProc), stmtType(aTokenizer.current().keyword)  {
    }

    DumpDBStatement::DumpDBStatement(const DumpDBStatement &aCopy)
            : processor(aCopy.processor), stmtType(aCopy.stmtType) {
    }

    DumpDBStatement::~DumpDBStatement()=default;

    //USE: -------------------
    StatusResult DumpDBStatement::parse(Tokenizer &aTokenizer) {
        if (validDumpDBCommand(aTokenizer)) {
            aTokenizer.next(2);
            return processor->setDBName(aTokenizer.current().data);
        }
        return StatusResult{invalidCommand};
    }

    StatusResult DumpDBStatement::run(std::ostream &aStream) {
        return processor->dumpDatabase(processor->getDBName());
    }

    bool validDumpDBCommand(Tokenizer &aTokenizer) {
        return Keywords::dump_kw == aTokenizer.current().keyword
               && Keywords::database_kw==aTokenizer.peek().keyword
               && TokenType::identifier == aTokenizer.peek(2).type
               && ";" == aTokenizer.peek(3).data;
    }
}