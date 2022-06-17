//
//  DBProcessor.cpp
//  RGAssignment2
//
//  Created by rick gessner on 2/27/21.
//


#include "DBProcessor.hpp"
#include "Application.hpp"
#include <filesystem>
#include <iostream>
#include "CreateDBStatement.hpp"
#include "DropDBStatement.hpp"
#include "UseDBStatement.hpp"
#include "DumpDBStatement.hpp"
#include "ShowDBStatement.hpp"
#include "ShowIndexDBStatement.hpp"

namespace ECE141 {

    //like a typedef!!!
    using StmtFactory = Statement* (*)(DBProcessor *aProc,
                                                    Tokenizer &aTokenize);

    DBProcessor::DBProcessor(std::ostream &anOutput, Application *anApp)
    : CmdProcessor(anOutput), app(anApp), sqlp(anOutput, anApp) {
    }

    bool DBProcessor::isKnown(Tokenizer &aTokenizer) {
        if ((Keywords::create_kw == aTokenizer.current().keyword
            || Keywords::drop_kw == aTokenizer.current().keyword)
            && Keywords::database_kw == aTokenizer.peek().keyword) {
            return true;
        }
        else if (Keywords::show_kw == aTokenizer.current().keyword
                 && Keywords::databases_kw == aTokenizer.peek().keyword) {
            return true;
        }
        else if (Keywords::show_kw == aTokenizer.current().keyword
                 && Keywords::indexes_kw == aTokenizer.peek().keyword) {
            aTokenizer.next();
            return true;
        }
        return (Keywords::dump_kw  == aTokenizer.current().keyword) || (Keywords::use_kw == aTokenizer.current().keyword);
    }
  
    //CmdProcessor interface ...
    CmdProcessor* DBProcessor::recognizes(Tokenizer &aTokenizer) {
        return isKnown(aTokenizer) ? this : sqlp.recognizes(aTokenizer);
    }

    StatusResult DBProcessor::run(Statement *aStmt) {
        return aStmt->run(app->output);
    }

    // USE: retrieve a statement based on given text input...
    Statement* DBProcessor::makeStatement(Tokenizer &aTokenizer, StatusResult &aResult) {
        std::map<Keywords, StmtFactory> factoryDispatch {
                {Keywords::create_kw, createFactory},
                {Keywords::drop_kw, dropFactory},
                {Keywords::show_kw,showFactory},
                {Keywords::dump_kw, dumpFactory},
                {Keywords::use_kw, useFactory},
                {Keywords::indexes_kw, indexFactory}
        };
        Token theToken=aTokenizer.current();

        if (factoryDispatch.count(theToken.keyword)) {
            auto aStatement = factoryDispatch[theToken.keyword](this, aTokenizer);
            aResult = aStatement->parse(aTokenizer);
            aTokenizer.next();
            if (aResult) return aStatement;
        }
        return nullptr;
    }

    StatusResult DBProcessor::setDBName(std::string &aName) {
      dbname = aName;
      return StatusResult(noError);
    }

    //-------------------------------------------------------

    Database* DBProcessor::getDatabaseInUse() {
        return app->getDatabaseInUse();
    }

    bool DBProcessor::dbExists(const std::string &aDBName) {
        return false;
    }

    StatusResult DBProcessor::createDatabase(const std::string &aName) {
        return app->createDatabase(aName);
    }
  
    StatusResult DBProcessor::showDatabases() const {
        return app->showDatabases();
    }

    StatusResult DBProcessor::dropDatabase(const std::string &aName) {
        return app->dropDatabase(aName);
    }

    StatusResult DBProcessor::dumpDatabase(const std::string &aName)  {
        return app->dumpDatabase(aName);
    }

    StatusResult DBProcessor::useDatabase(const std::string &aName) {
        return app->useDatabase(aName);
    }

    StatusResult DBProcessor::showIndexes() {
        return app->showIndexes();
    }

    Statement* DBProcessor::createFactory (DBProcessor *aProc, Tokenizer& aTokenizer) {
        return new CreateDBStatement(aProc, aTokenizer);
    }

    Statement* DBProcessor::dropFactory(DBProcessor *aProc, Tokenizer& aTokenizer) {
        return new DropDBStatement(aProc, aTokenizer);
    }

    Statement* DBProcessor::useFactory (DBProcessor *aProc, Tokenizer& aTokenizer) {
        return new UseDBStatement(aProc, aTokenizer);
    }

    Statement* DBProcessor::showFactory(DBProcessor *aProc, Tokenizer& aTokenizer){
        return new ShowDBStatement(aProc, aTokenizer);
    }

    Statement* DBProcessor::dumpFactory(DBProcessor *aProc, Tokenizer& aTokenizer) {
        return new DumpDBStatement(aProc, aTokenizer);
    }

    Statement *DBProcessor::indexFactory(DBProcessor *aProc, Tokenizer &aTokenizer) {
        return new ShowIndexDBStatement(aProc, aTokenizer);
    }
}
