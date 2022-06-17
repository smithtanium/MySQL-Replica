//
//  CommandProcessor.cpp
//
//  Created by rick gessner on 3/30/20
//  Copyright © 2018 rick gessner. All rights reserved.
//

#include <iostream>
#include "Application.hpp"
#include "Tokenizer.hpp"
#include <memory>
#include <algorithm>
#include <vector>
#include "Config.hpp"
#include "BlockView.hpp"
#include "DBView.hpp"
#include "FolderView.hpp"

namespace ECE141 {
  
  size_t Config::cacheSize[]={0,200,0};


  Application::Application(std::ostream &anOutput)
    : CmdProcessor(anOutput), dbp(anOutput, this), activeDB(nullptr) {
  }
  
  Application::~Application() {
    delete activeDB;
  }
  
  // USE: -----------------------------------------------------
  

  bool Application::isKnown(Tokenizer &aTokenizer) {
    static Keywords theKnown[]=
            {Keywords::quit_kw,Keywords::version_kw, Keywords::help_kw};
    auto theIt = std::find(std::begin(theKnown),
                           std::end(theKnown), aTokenizer.current().keyword);
    return theIt!=std::end(theKnown);
  }

  CmdProcessor* Application::recognizes(Tokenizer &aTokenizer) {
      return isKnown(aTokenizer) ? this : dbp.recognizes(aTokenizer);
  }

  StatusResult Application::run(Statement* aStatement) {
    switch(aStatement->getType()) {
      case Keywords::quit_kw:
        output << "DB::141 is shutting down\n";
        return StatusResult(ECE141::userTerminated);
      case Keywords::version_kw:
        output << "Version " << getVersion() << "\n"; break;
      case Keywords::help_kw:
        output << "Help system available\n"; break;
      default: break;
    }
    return StatusResult{Errors::noError};
  }
  
  // USE: retrieve a statement based on given text input...
  Statement* Application::makeStatement(Tokenizer &aTokenizer,
                                        StatusResult &aResult) {
    Token theToken=aTokenizer.current();

    if (isKnown(aTokenizer)) {
      aTokenizer.next(); //skip ahead...
      return new Statement(theToken.keyword);
    }
    return nullptr;
  }

  //build a tokenizer, tokenize input, ask processors to handle...
  StatusResult Application::handleInput(std::istream &anInput){
    Tokenizer theTokenizer(anInput);
    StatusResult theResult=theTokenizer.tokenize();

//    if (inValidateInput(theTokenizer, theResult)) {return theResult;}

    if (theTokenizer.remaining() > 0) {
        if (theTokenizer.tokenAt(theTokenizer.remaining()-1).data != ";") {theTokenizer.addSemiColon();}

        while (theResult && theTokenizer.more()) {
            Config::getTimer().reset();
            if(auto *theProc=recognizes(theTokenizer)) {
                if(auto *theCmd=theProc->makeStatement(theTokenizer,theResult)) {
                    if (theResult) { //I did this
                        theResult = theProc->run(theCmd);
                        if (theResult) theTokenizer.skipIf(';');
                        delete theCmd;
                    }
                }
            }
            else theResult=unknownCommand;
        }
    }
    return theResult;
  }

    StatusResult Application::createDatabase(const std::string &aName) {
        if (std::filesystem::exists(Config::getDBPath(aName))){
            return StatusResult{databaseExists};
        }
        Database theDB = Database(aName, CreateDB{});
        DBView theDBView(1);
        theDBView.show(output);
        return StatusResult{noError};
    }

    StatusResult Application::dumpDatabase(const std::string &aName) {
        if (std::filesystem::exists(Config::getDBPath(aName))){
            Database theDB(aName, OpenDB{});
            BlockView theBlockView(&theDB);
            theBlockView.show(output);
            return StatusResult{noError};
        }
        return StatusResult{unknownDatabase};
    }

    StatusResult Application::dropDatabase(const std::string &aName) {
        if (std::filesystem::exists(Config::getDBPath(aName))){
            std::remove(Config::getDBPath(aName).c_str());
            DBView theDBView(0);
            theDBView.show(output);
            return StatusResult{noError};
        }
        return StatusResult{unknownDatabase};
    }

    StatusResult Application::showDatabases() const {
        FolderView theFolderView(Config::getStoragePath());
        theFolderView.show(output);
        return StatusResult{noError};
    }

    StatusResult Application::useDatabase(const std::string &aName) {
        if (std::filesystem::exists(Config::getDBPath(aName))){
            delete activeDB;
            activeDB = new Database(aName, OpenDB{});
            output << "Database changed\n";
            return getDatabaseInUse()->loadEntities();
        }
        return StatusResult{unknownDatabase};
    }

    Database *Application::getDatabaseInUse() {
        return activeDB;
    }

    bool Application::inValidateInput(Tokenizer &aTokenizer, StatusResult &aResult) {
        if (checkNoEndingSemiColon(aTokenizer)) {
            aResult.error=syntaxError;
            return true;
        }
        return false;
    }

    StatusResult Application::showIndexes() const {
        return activeDB->showIndexes(output);
    }


    bool checkNoEndingSemiColon(Tokenizer &aTokenizer) {
    return aTokenizer.size() > 0
           && aTokenizer.tokenAt(aTokenizer.size()-1).data != ";";

  }
}