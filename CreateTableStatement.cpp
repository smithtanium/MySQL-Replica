//
// Created by smith on 5/3/2022.
//

#include "CreateTableStatement.hpp"
#include "Index.hpp"

namespace ECE141 {
    CreateTableStatement::CreateTableStatement(SQLProcessor *aProc, Tokenizer &aTokenizer)
            : processor(aProc), entity(nullptr), stmtType(aTokenizer.current().keyword)  {
    }

    CreateTableStatement::CreateTableStatement(const CreateTableStatement &aCopy)
            : processor(aCopy.processor), stmtType(aCopy.stmtType), entity(nullptr) {
    }

    CreateTableStatement::~CreateTableStatement()=default;

    //USE: -------------------
    StatusResult CreateTableStatement::parse(Tokenizer &aTokenizer) {
        if (validCreateSQLCommand(aTokenizer)) {
            aTokenizer.next(2);
            tableName = aTokenizer.current().data;
            return makeEntity(aTokenizer);
        }
        else {
            while (aTokenizer.more()) {
                aTokenizer.next();
            }
            return StatusResult{invalidCommand};
        }
    }

    StatusResult CreateTableStatement::run(std::ostream &aStream) {
        return processor->createTable(entity);
    }

    bool validCreateSQLCommand(Tokenizer &aTokenizer) {
        return Keywords::create_kw == aTokenizer.current().keyword
               && Keywords::table_kw==aTokenizer.peek().keyword
               && TokenType::identifier == aTokenizer.peek(2).type;
    }

    StatusResult CreateTableStatement::makeEntity(Tokenizer &aTokenizer) { //FIXME
        if (processor->getDB()->checkTableExists(tableName)) {return StatusResult{tableExists};}
        auto *aEntity = new Entity(tableName);
        Attribute anAttribute(DataTypes::no_type);

        StatusResult aStatus = noError;
        while (!(aTokenizer.current().data ==";")) {
            aTokenizer.next();
            aTokenizer.skipIf('(');

            if (aTokenizer.current().data == "," || (aTokenizer.current().data == ")" && aTokenizer.peek().data == ";")) {
                aEntity->addAttribute(anAttribute);
                anAttribute.reset();
            }
            else {
                if (TokenType::identifier == aTokenizer.current().type) {
                    anAttribute.setName(aTokenizer.current().data);
                }
                else if (Helpers::isDatatype(aTokenizer.current().keyword)) {
                    anAttribute.setDataType(Helpers::getDatatype(aTokenizer.current().keyword));
                    if (Helpers::getDatatype(aTokenizer.current().keyword) == DataTypes::varchar_type) {
                        aTokenizer.next();
                        aTokenizer.skipIf('(');
                        anAttribute.setSize(stoi(aTokenizer.current().data));
                        aTokenizer.skipIf(')');
                    }
                }
                else if (Keywords::primary_kw == aTokenizer.current().keyword&& Keywords::key_kw == aTokenizer.peek().keyword) {
                    anAttribute.setPrimaryKey(true);
//                    aEntity->setPrimaryKey(&anAttribute);
                    aTokenizer.next();
                }
                else if (Keywords::not_kw == aTokenizer.current().keyword && Keywords::null_kw == aTokenizer.peek().keyword) {
                    anAttribute.setNullable(false);
                    aTokenizer.next();
                }
                else if (Keywords::auto_increment_kw == aTokenizer.current().keyword) {
                    anAttribute.setAutoIncrement(true);
                }
            }
        }
        entity = aEntity;
        return aStatus;
    }
}