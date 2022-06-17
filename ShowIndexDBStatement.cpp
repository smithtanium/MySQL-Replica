//
// Created by smith on 5/29/2022.
//

#include "ShowIndexDBStatement.hpp"
namespace ECE141 {
    ShowIndexDBStatement::ShowIndexDBStatement(DBProcessor *aProc, Tokenizer &aTokenizer)
            : processor(aProc), stmtType(aTokenizer.current().keyword)  {
    }

    ShowIndexDBStatement::ShowIndexDBStatement(const ShowIndexDBStatement &aCopy)
            : processor(aCopy.processor), stmtType(aCopy.stmtType) {
    }

    ShowIndexDBStatement::~ShowIndexDBStatement() = default;

    //USE: -------------------
    StatusResult ShowIndexDBStatement::parse(Tokenizer &aTokenizer) {
        if (validShowIndexCommand(aTokenizer)) {
            aTokenizer.next(2);
            return StatusResult{noError};
        }
        else {
            while (aTokenizer.more()) {aTokenizer.next();}
            return StatusResult{invalidCommand};
        }
    }

    StatusResult ShowIndexDBStatement::run(std::ostream &aStream) {
        return processor->showIndexes();
    }

    bool validShowIndexCommand(Tokenizer &aTokenizer){
        return (Keywords::indexes_kw == aTokenizer.current().keyword
            && ";" == aTokenizer.peek().data);
    }
}