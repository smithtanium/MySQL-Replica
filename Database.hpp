//
//  Database.hpp
//  RGAssignment2
//
//  Created by rick gessner on 2/27/21.
//

#ifndef Database_hpp
#define Database_hpp

#include <stdio.h>
#include <fstream> 
#include "Storage.hpp"
#include "SQLProcessor.hpp"
#include "Row.hpp"
#include "BasicTypes.hpp"
#include "Index.hpp"


namespace ECE141 {
  using EntityOpt = std::optional<Entity>;

  using EntityList = std::vector<Entity*>;
  //using BlockVisitor = std::function<bool(const std::string&)>;

  class Database : public Storable {
  public:
    
            Database(const std::string &aPath, CreateDB);
            Database(const std::string &aPath, OpenDB);

    virtual ~Database();

            StatusResult    dump(std::ostream &anOutput);
//            StatusResult    write(Entity *anEntity);
            StatusResult    addEntity(Entity *anEntity, std::ostream &anOutput);
            StatusResult    showEntity(std::ostream &anOutput);
            StatusResult    dropEntity(std::string aName, std::ostream &anOutput);
            StatusResult    describeEntity(const std::string &aName, std::ostream &anOutput);

            StatusResult    insertRows(std::string aName, const RowCollection& rows, std::ostream &anOutput);
//            StatusResult    selectRows(Query *aQuery, RowCollection &aRows, std::ostream &anOutput);
            StatusResult    selectRows(Query *aQuery, RowCollection &aRows, JoinList &theJoins, std::ostream &anOutput);
            StatusResult    updateRows(std::string &aAttributeName, Value &aValue,
                                       Query *aQuery, RowCollection &aRows, std::ostream &anOutput);
            StatusResult    deleteRows(Query *aQuery, RowCollection &aRows, std::ostream &anOutput);

            RowCollection   doSelectRows(Query *aQuery, StatusResult &aResult, std::ostream &anOutput);
            RowCollection   doSingleJoin(Query *aQuery,Join &aJoin, std::ostream &anOutput);
            RowCollection   doMultiJoin(Query *aQuery, JoinList &theJoins, std::ostream &anOutput);
            RowCollection   mergeRows(RowCollection &theRows, RowCollection &otherRows, Join &aJoin, std::string aTableName);

            Entity*         getEntity(const std::string &aName);
            StatusResult    encode(std::ostream &anOutput)override;
            StatusResult    decode(std::istream &anInput)override;

            StatusResult    showIndexes(std::ostream &anOutput); //newly added
            StatusResult    showIndexesByEntity(std::string &aName, StringList &aList, std::ostream &anOutput); //newly added

            StatusResult    loadEntities();

            bool            checkTableExists(const std::string &aName);
            bool            checkAttributesExists(const std::string &tableName, StringList aList) const;
            Storage*         getStorage() {return storage;}




  protected:
    
    std::string     name;
    std::fstream    stream;   //low level stream used by storage...
    bool            changed;  //might be helpful, or ignore if you prefer.
//    std::map<std::string, int> entitymap;
    IndexMap        indexes;
    Storage*        storage;
    EntityList      Entities;
    LRUCache<uint32_t, std::shared_ptr<Row>> theRowCache;  
    //LRUCache<uint32_t, std::shared_ptr<Row>> theViewCache;    

  };

}
#endif /* Database_hpp */
