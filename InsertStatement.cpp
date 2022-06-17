//
// Created by smith on 5/3/2022.
//

#include "InsertStatement.hpp"
#include <string>

namespace ECE141 {
    InsertStatement::InsertStatement(SQLProcessor *aProc, Tokenizer &aTokenizer)
            : processor(aProc), stmtType(aTokenizer.current().keyword){
    }

    InsertStatement::InsertStatement(const InsertStatement &aCopy)
            : processor(aCopy.processor), stmtType(aCopy.stmtType) {
    }

    InsertStatement::~InsertStatement() = default;

    //USE: -------------------
    StatusResult InsertStatement::parse(Tokenizer &aTokenizer) {
        if (validInsertSQLCommand(aTokenizer)) {
            aTokenizer.next(2);
            tableName= aTokenizer.current().data;
            return makeRows(aTokenizer);
        }
        aTokenizer.next(aTokenizer.remaining());
        return StatusResult{invalidCommand};
    }

    StatusResult InsertStatement::run(std::ostream &aStream) {
        return processor->insertRows(tableName, rows);
    }

    bool validInsertSQLCommand(Tokenizer &aTokenizer) {
        return Keywords::insert_kw == aTokenizer.current().keyword
               && Keywords::into_kw == aTokenizer.peek().keyword
               && TokenType::identifier == aTokenizer.peek(2).type;
    }

    std::shared_ptr<Row> InsertStatement::makeRow(Tokenizer &aTokenizer, StringList &aName) {
        auto rowPtr = std::make_shared<Row>(0);
        Entity *theEnt = processor->getDB()->getEntity(tableName);
        Attribute anAtt;
//        std::cout << theEnt->getPrimaryKey() <<std::endl;
//        std::cout << &theEnt->getAttributes().at(0) <<std::endl;
        rowPtr->set(theEnt->getAttributes().at(0).getName(), anAtt.toValue(std::to_string(theEnt->getAUniqueKey())));
        size_t index = 0;
        while (!(aTokenizer.current().data == ")" && (aTokenizer.peek().data == ","
                                                      || aTokenizer.peek().data == ";"))) {
            if (aTokenizer.current().type == TokenType::identifier) {
                rowPtr->set(aName.at(index++), aTokenizer.current().data);
            }
            else if (aTokenizer.current().type == TokenType::number) {
                if (Helpers::checkInt(aTokenizer.current().data)) {
                    rowPtr->set(aName.at(index++), anAtt.toValue(aTokenizer.current().data));

                }
                else {
                    rowPtr->set(aName.at(index++), std::get<double>(anAtt.toValue(aTokenizer.current().data)));
                }
            }
            aTokenizer.next();
            aTokenizer.skipIf(',');
        }
        return rowPtr;
    }

    StatusResult InsertStatement::makeRows(Tokenizer &aTokenizer) {
        RowCollection theRows;
        if (!processor->getDB()->checkTableExists(tableName)) {return StatusResult(unknownTable);}
        aTokenizer.next();
        StringList aList = getAttributeNames(aTokenizer);
        if (!processor->getDB()->checkAttributesExists(tableName, aList)) {return StatusResult(unknownAttribute);}
        aTokenizer.next();
        if (Keywords::values_kw == aTokenizer.current().keyword) {
            aTokenizer.next();
            aTokenizer.skipIf('(');
            while (!(aTokenizer.current().data == ";")) {
                theRows.push_back(makeRow(aTokenizer, aList));
                aTokenizer.next();
            }
        }
        else {return StatusResult(invalidCommand);}
        rows = theRows;
        return StatusResult(noError);
    }

    StringList getAttributeNames(Tokenizer &aTokenizer) {
        StringList aList;
        while (aTokenizer.current().data != ")" || aTokenizer.current().data == ";") {
            if (aTokenizer.current().type == TokenType::identifier) {
                aList.push_back(aTokenizer.current().data);
            }
            aTokenizer.next();
        }
        return aList;
    }
}