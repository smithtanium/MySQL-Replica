//
//  DBProcessor.hpp
//  RGAssignment2
//
//  Created by rick gessner on 2/27/21.
//

#ifndef DBProcessor_hpp
#define DBProcessor_hpp

#include <stdio.h>
#include "CmdProcessor.hpp"
#include "Tokenizer.hpp"
#include "Database.hpp"
#include "SQLProcessor.hpp"


namespace ECE141 {
  
  class DBStatement;
  class Application;

  class DBProcessor : public CmdProcessor {
  public:
    
    DBProcessor(std::ostream &anOutput, Application *anApp);

    ~DBProcessor()=default;
    
      //cmd processor interface...
    bool isKnown(Tokenizer &aTokenizer) override;

    CmdProcessor* recognizes(Tokenizer &aTokenizer) override;
    Statement*    makeStatement(Tokenizer &aTokenizer,
                                StatusResult &aResult) override;
    StatusResult  run(Statement *aStmt) override;
               
    //dbProcessor interface...
    StatusResult  createDatabase(const std::string &aName);
    StatusResult  dumpDatabase(const std::string &aName);
    StatusResult  dropDatabase(const std::string &aName);
    StatusResult  showDatabases() const;
    StatusResult  useDatabase(const std::string &aName);
    StatusResult  showIndexes();

    bool          dbExists(const std::string &aDBName);
    Database*     getDatabaseInUse();


    static Statement*    createFactory(DBProcessor *aProc, Tokenizer& aTokenizer);
    static Statement*    dropFactory(DBProcessor *aProc, Tokenizer& aTokenizer);
    static Statement*    useFactory(DBProcessor *aProc, Tokenizer& aTokenizer);
    static Statement*    showFactory(DBProcessor *aProc, Tokenizer& aTokenizer);
    static Statement*    dumpFactory(DBProcessor *aProc, Tokenizer& aTokenizer);
    static Statement*    indexFactory(DBProcessor *aProc, Tokenizer& aTokenizer);

    StatusResult  setDBName(std::string &aName);
    std::string   getDBName() {return dbname;}

  protected:
    std::string dbname;
    SQLProcessor sqlp;
    Application *app;
  };
}
#endif /* DBProcessor_hpp */
