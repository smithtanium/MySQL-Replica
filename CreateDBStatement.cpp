//
// Created by smith on 5/2/2022.
//

#include "CreateDBStatement.hpp"

namespace ECE141 {
    CreateDBStatement::CreateDBStatement(DBProcessor *aProc, Tokenizer &aTokenizer)
            : processor(aProc), stmtType(aTokenizer.current().keyword)  {
    }

    CreateDBStatement::CreateDBStatement(const CreateDBStatement &aCopy)
    : processor(aCopy.processor), stmtType(aCopy.stmtType) {
    }

    CreateDBStatement::~CreateDBStatement()=default;

    //USE: -------------------
    StatusResult CreateDBStatement::parse(Tokenizer &aTokenizer) {
        if (validCreateDBCommand(aTokenizer)) {
            aTokenizer.next(2);
            return processor->setDBName(aTokenizer.current().data);
        }
        return StatusResult{invalidCommand};
    }

    StatusResult CreateDBStatement::run(std::ostream &aStream) {
        return processor->createDatabase(processor->getDBName());
    }

    bool validCreateDBCommand(Tokenizer &aTokenizer) {
        return Keywords::create_kw == aTokenizer.current().keyword
               && Keywords::database_kw==aTokenizer.peek().keyword
               && TokenType::identifier == aTokenizer.peek(2).type
               && ";" == aTokenizer.peek(3).data;
    }
}
