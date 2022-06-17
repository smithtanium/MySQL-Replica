//
// Created by smith on 5/15/2022.
//

#include "UpdateStatement.hpp"

namespace ECE141 {

    UpdateStatement::UpdateStatement(ECE141::SQLProcessor *aProc, Tokenizer &aTokenizer)
            : processor(aProc), stmtType(aTokenizer.current().keyword), query(nullptr) {
    }


    UpdateStatement::~UpdateStatement() = default;

    StatusResult ECE141::UpdateStatement::run(std::ostream &aStream) {
        return processor->updateRows(name, value, query, rows);
    }

    StatusResult ECE141::UpdateStatement::parse(Tokenizer &aTokenizer) {
        StatusResult aStatus{invalidCommand};
        Attribute anAtt;
        if (validUpdateCommand(aTokenizer)) {
            aTokenizer.next();
            auto *aQuery = new Query(aTokenizer.current().data, aTokenizer);
            aTokenizer.next(2);
            Token aToken = aTokenizer.current();
            if (TokenType::identifier == aToken.type) {
                name = aToken.data;
                aTokenizer.next();
                aToken = aTokenizer.current();
                if ("=" == aToken.data) {
                    aTokenizer.next();
                    value = anAtt.toValue((aTokenizer.current().data));
                    aTokenizer.next();
                    if (Keywords::where_kw == aTokenizer.current().keyword) {
                        aTokenizer.next();
//                        Entity *theEnt = processor->getDB()->getEntity(aQuery->getTableName());
                        aStatus = aQuery->addFilters(aTokenizer, processor->getDB()->getEntity(aQuery->getTableName()));
                        query = aQuery;
                    }
                }
            }
        }
        return aStatus;
    }

    bool validUpdateCommand(Tokenizer &aTokenizer) {
        return Keywords::update_kw == aTokenizer.current().keyword
               && TokenType::identifier == aTokenizer.peek().type
               && Keywords::set_kw == aTokenizer.peek(2).keyword;
//               && TokenType::identifier == aTokenizer.peek(3).type;
    }
}
