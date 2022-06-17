//
// Created by smith on 5/3/2022.
//

#include "ShowIndexStatement.hpp"

namespace ECE141 {
    ShowIndexStatement::ShowIndexStatement(SQLProcessor *aProc, Tokenizer &aTokenizer)
            : processor(aProc), stmtType(aTokenizer.current().keyword)  {
    }

    ShowIndexStatement::ShowIndexStatement(const ShowIndexStatement &aCopy)
            : processor(aCopy.processor), stmtType(aCopy.stmtType) {
    }

    ShowIndexStatement::~ShowIndexStatement() = default;

    //USE: -------------------
    StatusResult ShowIndexStatement::parse(Tokenizer &aTokenizer) {
        if (validShowSQLCommand(aTokenizer)) {
            aTokenizer.next(1);
            if (Keywords::indexes_kw == aTokenizer.current().keyword){
                aTokenizer.next(1);
                aTokenizer.skipIf(';');
                return StatusResult{noError};
            } 
            if (Keywords::index_kw == aTokenizer.current().keyword){
                aTokenizer.next(1);
                while(aTokenizer.current().type == TokenType::identifier){
                    attributeNameCollections.push_back(aTokenizer.current().data);
                    aTokenizer.next(1);
                    aTokenizer.skipIf(',');
                }
                //aTokenizer.next(1);
                if (Keywords::from_kw==aTokenizer.current().keyword){
                    aTokenizer.next(1);
                    tableName = aTokenizer.current().data;
                    aTokenizer.next(1);
                    aTokenizer.skipIf(';');
                    return StatusResult{noError};
                }

            }
        }
        else {
            while (aTokenizer.more()) {
                aTokenizer.next();
            }
            return StatusResult{invalidCommand};
        }
        return StatusResult{noError};
    }

    StatusResult ShowIndexStatement::run(std::ostream &aStream) {
        return processor->showIndex(tableName, attributeNameCollections);
    }
}

