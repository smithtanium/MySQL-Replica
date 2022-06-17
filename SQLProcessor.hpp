//
//  SQLProcessor.hpp
//  PA3
//
//  Created by rick gessner on 4/2/22.
//

#ifndef SQLProcessor_hpp
#define SQLProcessor_hpp

#include <stdio.h>
#include "CmdProcessor.hpp"
#include "Tokenizer.hpp"
#include "Entity.hpp"
#include "Row.hpp"
#include "Query.hpp"
//#include "SelectStatement.hpp"


namespace ECE141 {
  class Application;
  class Statement;
  class DBProcessor; //define this later...
  class Entity;
  class Database;
  struct Join;
  using JoinList = std::vector<Join>;

  //Should this processor have it's own Statement types?


  class SQLProcessor : public CmdProcessor {
  public:
    
    SQLProcessor(std::ostream &anOutput, Application *anApp);
    virtual ~SQLProcessor();

    bool isKnown(Tokenizer &aTokenizer) override;

    CmdProcessor* recognizes(Tokenizer &aTokenizer) override;
    Statement*    makeStatement(Tokenizer &aTokenizer,
                                StatusResult &aResult) override;
    
    StatusResult  run(Statement *aStmt) override;

    Database*     getDB();

    
    //We don't DO the work here, but we can help route cmds...
    StatusResult  createTable(Entity *anEntity);
    StatusResult  describeTable(const std::string &aName);
    StatusResult  dropTable(const std::string &aName);
    StatusResult  showTables();
    
    StatusResult  insertRows(const std::string &aName, const RowCollection &theRow);
//    StatusResult  showQuery(Query *aQuery,  RowCollection &theRow);
    StatusResult  showQuery(Query *aQuery,  RowCollection &theRow, JoinList &theJoins);

    StatusResult updateRows(std::string &aAttributeName, Value &aValue, Query *aQuery, RowCollection &theRows);
    StatusResult deleteRows(Query *aQuery, RowCollection &theRows);
    StatusResult showIndex(std::string &aName, StringList &aList);

    static Statement*    createFactory(SQLProcessor *aProc, Tokenizer& aTokenizer);
    static Statement*    dropFactory(SQLProcessor *aProc, Tokenizer& aTokenizer);
    static Statement*    describeFactory(SQLProcessor *aProc, Tokenizer& aTokenizer);
    static Statement*    showFactory(SQLProcessor *aProc, Tokenizer& aTokenizer);
    static Statement*    insertFactory(SQLProcessor *aProc, Tokenizer& aTokenizer);
    static Statement*    selectFactory(SQLProcessor *aProc, Tokenizer& aTokenizer);
    static Statement*    updateFactory(SQLProcessor *aProc, Tokenizer& aTokenizer);
    static Statement*    deleteFactory(SQLProcessor *aProc, Tokenizer& aTokenizer);

    StatusResult  setRows(RowCollection &theRow);
    RowCollection getRows() const;

    StatusResult  setTableName(std::string &aName);
    std::string   getTableName() const;

    StatusResult  setQuery(Query *aQuery);
    Query*        getQuery();

  protected:
    Query *query;
    RowCollection rows;
    Entity *entity;
    std::string tableName;
    Application *app;
  };
}
#endif /* SQLProcessor_hpp */
