//
//  AppProcessor.hpp
//  Database1
//
//  Created by rick gessner on 4/4/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#ifndef Application_hpp
#define Application_hpp

#include <stdio.h>
#include "DBProcessor.hpp"
#include "CmdProcessor.hpp"

namespace ECE141 {

  class Application : public CmdProcessor {
  public:
    
    Application(std::ostream &anOutput);
    virtual ~Application();

    bool isKnown(Tokenizer &aTokenizer) override;

      //app api...
            std::string   getVersion() {return "1.0";}
    
    virtual StatusResult  handleInput(std::istream &anInput);
            CmdProcessor* recognizes(Tokenizer &aTokenizer) override;
            Statement*    makeStatement(Tokenizer &aTokenizer,
                                        StatusResult &aResult) override;
            StatusResult  run(Statement *aStmt) override;

            StatusResult  createDatabase(const std::string &aName);
            StatusResult  dumpDatabase(const std::string &aName);
            StatusResult  dropDatabase(const std::string &aName);
            StatusResult  showDatabases() const;
            StatusResult  useDatabase(const std::string &aName);
            StatusResult  showIndexes() const;

            bool inValidateInput(Tokenizer &aTokenizer, StatusResult &aResult);

            Database*     getDatabaseInUse();

  protected:
    Database *activeDB;
    DBProcessor dbp;
  };
    bool checkNoEndingSemiColon(Tokenizer &aTokenizer);
}

#endif /* Application_hpp */
