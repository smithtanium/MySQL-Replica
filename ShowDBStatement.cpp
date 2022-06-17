//
// Created by smith on 5/2/2022.
//

#include "ShowDBStatement.hpp"

namespace ECE141 {
    ShowDBStatement::ShowDBStatement(DBProcessor *aProc, Tokenizer &aTokenizer)
            : processor(aProc), stmtType(aTokenizer.current().keyword)  {
    }

    ShowDBStatement::ShowDBStatement(const ShowDBStatement &aCopy)
            : processor(aCopy.processor), stmtType(aCopy.stmtType) {
    }

    ShowDBStatement::~ShowDBStatement()=default;

    //USE: -------------------
    StatusResult ShowDBStatement::parse(Tokenizer &aTokenizer) {
        if (validShowDBCommand(aTokenizer)) {
            aTokenizer.next();
            return processor->setDBName(aTokenizer.current().data);
        }
        return StatusResult{invalidCommand};
    }

    StatusResult ShowDBStatement::run(std::ostream &aStream) {
        return processor->showDatabases();
    }

    bool validShowDBCommand(Tokenizer &aTokenizer){
        return Keywords::show_kw == aTokenizer.current().keyword
               && Keywords::databases_kw==aTokenizer.peek().keyword
               && ";" == aTokenizer.peek(2).data;
    }
}