//
//  SQLProcessor.cpp
//  PA3
//
//  Created by rick gessner on 4/2/22.
//

#include "SQLProcessor.hpp"
#include <algorithm>
#include "Application.hpp"
#include "SelectStatement.hpp"
#include "CreateTableStatement.hpp"
#include "DropTableStatement.hpp"
#include "InsertStatement.hpp"
#include "ShowTableStatement.hpp"
#include "DescribeTableStatement.hpp"
#include "UpdateStatement.hpp"
#include "DeleteStatement.hpp"
#include "ShowIndexStatement.hpp"


namespace ECE141 {
    using StmtFactory = Statement* (*)(SQLProcessor *aProc,
                                                     Tokenizer &aTokenize);

    SQLProcessor::SQLProcessor(std::ostream &anOutput, Application *anApp)
            : CmdProcessor(anOutput), app(anApp), query(nullptr), entity(nullptr) {
    }

    SQLProcessor::~SQLProcessor()=default;

    bool SQLProcessor::isKnown(Tokenizer &aTokenizer) {
        static Keywords theKnown[] =
                {Keywords::create_kw, Keywords::drop_kw, Keywords::describe_kw, Keywords::show_kw,
                 Keywords::insert_kw, Keywords::select_kw, Keywords::update_kw, Keywords::delete_kw};
        auto theIt = std::find(std::begin(theKnown),
                               std::end(theKnown), aTokenizer.current().keyword);
        return theIt != std::end(theKnown);
    }

    CmdProcessor *SQLProcessor::recognizes(Tokenizer &aTokenizer) {
        return isKnown(aTokenizer) ? this : nullptr;
    }

    Statement *SQLProcessor::makeStatement(Tokenizer &aTokenizer, StatusResult &aResult) {
        std::map<Keywords, StmtFactory> factoryDispatch {
                {Keywords::create_kw, createFactory},
                {Keywords::drop_kw, dropFactory},
                {Keywords::show_kw, showFactory},
                {Keywords::describe_kw, describeFactory},
                {Keywords::insert_kw, insertFactory},
                {Keywords::select_kw, selectFactory},
                {Keywords::update_kw,updateFactory},
                {Keywords::delete_kw, deleteFactory}
        };
        if (!app->getDatabaseInUse()) {
            aResult=noDatabaseSpecified;
            return nullptr;
        }
        Token theToken=aTokenizer.current();
        if (factoryDispatch.count(theToken.keyword)) {
            auto aStatement = factoryDispatch[theToken.keyword](this, aTokenizer);
            aResult = aStatement->parse(aTokenizer);
            aTokenizer.next();
            if (aResult) return aStatement;
        }
        return nullptr;
    }

    StatusResult SQLProcessor::run(Statement *aStmt) {
        return aStmt->run(app->output);
    }

    StatusResult SQLProcessor::createTable(Entity *anEntity) {
        return app->getDatabaseInUse()->addEntity(anEntity, app->output);
    }

    StatusResult SQLProcessor::describeTable(const std::string &aName) {
        return app->getDatabaseInUse()->describeEntity(aName, app->output);
    }

    StatusResult SQLProcessor::dropTable(const std::string &aName) {
        return app->getDatabaseInUse()->dropEntity(aName, app->output);
    }

    StatusResult SQLProcessor::showTables() {
        return app->getDatabaseInUse()->showEntity(app->output);
    }

    StatusResult SQLProcessor::insertRows(const std::string &aName, const  RowCollection &theRows) {
        return app->getDatabaseInUse()->insertRows(aName, theRows, app->output);
    }

    StatusResult SQLProcessor::showQuery(Query *aQuery, RowCollection &theRows, JoinList &theJoins) {
        return app->getDatabaseInUse()->selectRows(aQuery, theRows, theJoins, app->output);

    }

    StatusResult SQLProcessor::updateRows(std::string &aAttributeName, Value &aValue, Query *aQuery, RowCollection &theRows) {
        return app->getDatabaseInUse()->updateRows(aAttributeName, aValue, aQuery, theRows, app->output);
    }

    StatusResult SQLProcessor::deleteRows(Query *aQuery, RowCollection &theRows) {
        return app->getDatabaseInUse()->deleteRows(aQuery, theRows, app->output);
    }

    StatusResult SQLProcessor::showIndex(std::string &aName, StringList &aList) {
        return app->getDatabaseInUse()->showIndexesByEntity(aName, aList, app->output);
    }

/*----------------------------------factories-------------------------------------------------*/

    Statement *SQLProcessor::createFactory(SQLProcessor *aProc, Tokenizer &aTokenizer) {
        return new CreateTableStatement(aProc, aTokenizer);
    }

    Statement *SQLProcessor::dropFactory(SQLProcessor *aProc, Tokenizer &aTokenizer) {
        return new DropTableStatement(aProc, aTokenizer);
    }

    Statement *SQLProcessor::describeFactory(SQLProcessor *aProc, Tokenizer &aTokenizer) {
        return new DescribeTableStatement(aProc, aTokenizer);
    }

    Statement *SQLProcessor::showFactory(SQLProcessor *aProc, Tokenizer &aTokenizer) {
        if(aTokenizer.peek().keyword == Keywords::index_kw || aTokenizer.peek().keyword == Keywords::indexes_kw) return new ShowIndexStatement(aProc, aTokenizer);
        return new ShowTableStatement(aProc, aTokenizer);
    }

    Statement *SQLProcessor::insertFactory(SQLProcessor *aProc, Tokenizer &aTokenizer) {
        return new InsertStatement(aProc, aTokenizer);
    }

    Statement *SQLProcessor::selectFactory(SQLProcessor *aProc, Tokenizer &aTokenizer) {
        return new SelectStatement(aProc, aTokenizer);
    }

    Statement *SQLProcessor::updateFactory(SQLProcessor *aProc, Tokenizer &aTokenizer) {
        return new UpdateStatement(aProc, aTokenizer);
    }

    Statement *SQLProcessor::deleteFactory(SQLProcessor *aProc, Tokenizer &aTokenizer) {
        return new DeleteStatement(aProc, aTokenizer);
    }

    Database *SQLProcessor::getDB() {
        return app->getDatabaseInUse();
    }

    StatusResult  SQLProcessor::setTableName(std::string &aName) {
        tableName = aName;
        return StatusResult{noError};
    }

    std::string SQLProcessor::getTableName() const {
        return tableName;
    }

    StatusResult SQLProcessor::setQuery(Query *aQuery) {
        query = aQuery;
        return StatusResult{noError};
    }

    Query *SQLProcessor::getQuery() {
        return query;
    }
}
