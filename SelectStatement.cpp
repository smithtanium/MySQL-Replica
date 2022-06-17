//
// Created by smith on 5/2/2022.
//

#include "SelectStatement.hpp"

namespace ECE141 {
    SelectStatement::SelectStatement(SQLProcessor *aProc, Tokenizer &aTokenizer)
    : processor(aProc), stmtType(aTokenizer.current().keyword), query(nullptr) {
    }

    SelectStatement::SelectStatement(const SelectStatement &aCopy)
            : processor(aCopy.processor), stmtType(aCopy.stmtType) {
    }

    SelectStatement::~SelectStatement() = default;

    //USE: -------------------
    StatusResult SelectStatement::parse(Tokenizer &aTokenizer) {
        StatusResult aResult = makeQuery(aTokenizer);
        if (aResult && ";" != aTokenizer.peek().data) {
            aTokenizer.next();
            return makeFilter(aTokenizer);
        }
        aTokenizer.next(aTokenizer.remaining());
        return aResult;
    }

    StatusResult SelectStatement::run(std::ostream &aStream) {
        return processor->showQuery(query, rows, joins);
    }

    bool validSelectAllSQLCommand(Tokenizer &aTokenizer) {
        return Keywords::select_kw == aTokenizer.current().keyword
               && TokenType::operators ==aTokenizer.peek().type // add attribute check?
               && Keywords::from_kw == aTokenizer.peek(2).keyword
               && TokenType::identifier == aTokenizer.peek(3).type;
    }

    StatusResult SelectStatement::makeQuery(Tokenizer &aTokenizer) {
        return validSelectAllSQLCommand(aTokenizer)
               ? makeAllQuery(aTokenizer) : makeAttributeQuery(aTokenizer);
    }

    StatusResult SelectStatement::makeAllQuery(Tokenizer &aTokenizer) { //select all query
        StatusResult aResult{noError};
        if (validSelectAllSQLCommand(aTokenizer)) {
            aTokenizer.next(3);
            auto *aQuery = new Query(aTokenizer.current().data, aTokenizer);
            query = aQuery;
            return aResult;
        }
        return aResult = invalidCommand;
    }

    StatusResult SelectStatement::makeAttributeQuery(Tokenizer &aTokenizer) {
        StatusResult aResult{noError};
        StringList aList;
        std::string aName;
        aTokenizer.next();
        while (Keywords::from_kw != aTokenizer.current().keyword && aResult) {
            if (TokenType::identifier ==  aTokenizer.current().type) {
                aList.push_back(aTokenizer.current().data);
            }
            else {
                return aResult = invalidCommand;
            }
            aTokenizer.next();
            aTokenizer.skipIf(',');
            if (!(aTokenizer.more())) {
                aResult = invalidCommand;
            }
        }
        aTokenizer.next();
        aName = aTokenizer.current().data;
        auto *aQuery = new Query(aName, aTokenizer);
        query = aQuery;
        query->setAttributeList(aList);
        return aResult;
    }

    StatusResult SelectStatement::makeFilter(Tokenizer &aTokenizer) {
        static Keywords gJoinTypes[] =
                {Keywords::cross_kw, Keywords::full_kw, Keywords::inner_kw, Keywords::left_kw,
                 Keywords::right_kw};
        StatusResult theStatus{noError};
        Keywords theJoinType{Keywords::join_kw};
        while (aTokenizer.more() && theStatus) {
            if (Keywords::where_kw == aTokenizer.current().keyword) {
                aTokenizer.next();
                theStatus = query->addFilters(aTokenizer, processor->getDB()->getEntity(query->getTableName()));
            }
            else if (Keywords::order_kw == aTokenizer.current().keyword && Keywords::by_kw == aTokenizer.peek().keyword) {
                aTokenizer.next(2);
                theStatus = query->setOrderBy(aTokenizer.current().data);
                aTokenizer.next();
            }
            else if (Keywords::limit_kw == aTokenizer.current().keyword) {
                aTokenizer.next();
                theStatus = query->setLimit(std::stoi(aTokenizer.current().data));
                aTokenizer.next();
            }
            else if(in_array<Keywords, 5>(gJoinTypes, aTokenizer.current().keyword)){
            //SELECT users.first_name, users.last_name, order_number FROM users LEFT JOIN orders ON users.id=orders.user_id
                theStatus = joinTypeExpected;
                theJoinType = aTokenizer.current().keyword;
                aTokenizer.next();
                if(aTokenizer.skipIf(Keywords::join_kw)) {
                    std::string theTable;
                    theTable = aTokenizer.current().data;
                    Join theJoin(theTable, theJoinType);
                    aTokenizer.next();
                    if(aTokenizer.skipIf(Keywords::on_kw)){
                        theJoin.onLeft.entityId = Helpers::hashString(aTokenizer.current().data.c_str());
                        aTokenizer.next();
                        aTokenizer.skipIf('.');
                        theJoin.onLeft.name = aTokenizer.current().data;
                        aTokenizer.next();
                        if(aTokenizer.skipIf(Operators::equal_op)){
                            theJoin.onRight.entityId = Helpers::hashString(aTokenizer.current().data.c_str());
                            aTokenizer.next();
                            aTokenizer.skipIf('.');
                            theJoin.onRight.name = aTokenizer.current().data;
                            aTokenizer.next();
                            joins.push_back(theJoin);
                        }
                    }

                }
                theStatus = noError;
            }
            else {
                theStatus = invalidCommand;
                aTokenizer.next(aTokenizer.remaining());
            }
            aTokenizer.skipIf(';');
        }
        return theStatus;
    }

}