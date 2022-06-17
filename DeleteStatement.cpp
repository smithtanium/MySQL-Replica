//
// Created by smith on 5/15/2022.
//

#include "DeleteStatement.hpp"
#include "Database.hpp"

namespace ECE141 {

    DeleteStatement::DeleteStatement(ECE141::SQLProcessor *aProc, ECE141::Tokenizer &aTokenizer)
    : processor(aProc), stmtType(aTokenizer.current().keyword), query(nullptr) {

    }

    DeleteStatement::DeleteStatement(const ECE141::DeleteStatement &aCopy) {

    }

    DeleteStatement::~DeleteStatement() = default;

    StatusResult ECE141::DeleteStatement::parse(ECE141::Tokenizer &aTokenizer) {
        StatusResult aStatus{invalidCommand};

        if (validDeleteCommand(aTokenizer)) {
            aTokenizer.next(2);
            auto *aQuery = new Query(aTokenizer.current().data, aTokenizer);
            aTokenizer.next();
            if (Keywords::where_kw == aTokenizer.current().keyword) {
                aTokenizer.next();
                aStatus = aQuery->addFilters(aTokenizer, processor->getDB()->getEntity(aQuery->getTableName()));
                query = aQuery;
            };
        }
        return aStatus;
    }

    StatusResult ECE141::DeleteStatement::run(std::ostream &aStream) {
        return processor->deleteRows(query, rows);
    }

    bool validDeleteCommand(Tokenizer &aTokenizer) {
        return Keywords::delete_kw == aTokenizer.current().keyword
               && Keywords::from_kw == aTokenizer.peek().keyword
               && TokenType::identifier == aTokenizer.peek(2).type;
    }
}