//
// Created by smith on 5/2/2022.
//

#include "UseDBStatement.hpp"

namespace ECE141 {
    UseDBStatement::UseDBStatement(DBProcessor *aProc, Tokenizer &aTokenizer)
            : processor(aProc), stmtType(aTokenizer.current().keyword)  {
    }

    UseDBStatement::UseDBStatement(const UseDBStatement &aCopy)
            : processor(aCopy.processor), stmtType(aCopy.stmtType) {
    }

    UseDBStatement::~UseDBStatement()=default;
    //USE: -------------------
    StatusResult UseDBStatement::parse(Tokenizer &aTokenizer) {
        if (validUseDBCommand(aTokenizer)) {
            aTokenizer.next();
            return processor->setDBName(aTokenizer.current().data);
        }
        return StatusResult{invalidCommand};
    }

    StatusResult UseDBStatement::run(std::ostream &aStream) {
        return processor->useDatabase(processor->getDBName());
    }

    bool validUseDBCommand(Tokenizer &aTokenizer){
        return (Keywords::use_kw == aTokenizer.current().keyword
                && TokenType::identifier == aTokenizer.peek().type)
               && ";" == aTokenizer.peek(2).data;
    }
}