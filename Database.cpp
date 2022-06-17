//
//  Database.cpp
//  RGAssignment2
//
//  Created by rick gessner on 2/27/21.
//

#include <memory>
#include <string>
#include <iostream>
#include <iomanip>
#include <map>
#include <fstream>
#include "Database.hpp"
#include "TabularView.hpp"
#include <optional>
#include "Compare.hpp"
#include "SelectStatement.hpp"

namespace ECE141 {
  
    Database::Database(const std::string &aName, CreateDB)
                        : name(aName), changed(true), storage(nullptr),  theRowCache(LRUCache<uint32_t, std::shared_ptr<Row>> (Config::getCacheSize(CacheType::row))) {
        std::string thePath = Config::getDBPath(name);
        //create for read/write
        //std::fstream outfile;
        stream.open(thePath, std::fstream::out);
        storage = new Storage(stream);

        Block theBlock(ECE141::BlockType::meta_block);
        theBlock.payload[0] = '0'; //length of entity map
        theBlock.payload[1] = ' '; //save entity name & entity block num
        storage->writeBlock(0, theBlock);
    }

    Database::Database(const std::string &aName, OpenDB)
                        : name(aName), changed(false),  theRowCache(LRUCache<uint32_t, std::shared_ptr<Row>> (Config::getCacheSize(CacheType::row))) {
        std::string thePath = Config::getDBPath(name);
        stream.open(thePath, std::fstream::in|std::fstream::out);
        storage = new Storage(stream);
        std::stringstream sstream;
        storage->load(sstream, 0);
        decode(sstream);
    }

    Database::~Database() {
        //std::cout<<"Delete"<<std::endl;
        if (storage!= nullptr){
        //std::cout<<"close"<<std::endl;
        delete storage;
        stream.close();
        }
        if(changed) {
        //stuff to save?
        }
    }

    // USE: Call this to dump the db for debug purposes...
    StatusResult Database::dump(std::ostream &anOutput) {
        uint32_t theCount = storage->getBlockCount();
        for (uint32_t i = 0; i < theCount; i++){
            Block theBlock(BlockType::data_block);
            storage->readBlock(i, theBlock);
            anOutput<<"| "<<std::left<<std::setw(15) << theBlock.header.type<<"|"<< " " << std::left<<std::setw(7)<<"0"<<  std::left << std::setw(16) << "|"  << "|" << std::endl;
            anOutput<<"+----------------+--------+---------------+"<<std::endl;
            // std::cout<<<<std::endl;
            //anOutput<<theBlock.header.type;
            //return theBlock.header.type;
        }
        anOutput<<storage->getBlockCount() <<" rows in set ("<< std::fixed << Config::getTimer().elapsed() << " secs)"<<std::endl;
        return StatusResult{noError};
    }

    StatusResult Database::addEntity(Entity *anEntity, std::ostream &anOutput) {
      if(!indexes.count(anEntity->getName())) {
          std::stringstream theEntityStream;
          StatusResult theResult = anEntity->encode(theEntityStream);
          if (theResult){ // save entity
              StorageInfo theInfo(0, theEntityStream.str().size(), -1, BlockType::entity_block, Helpers::hashString(anEntity->getName().c_str()));
              theResult = storage->save(theEntityStream, theInfo);
              if (theResult) { // add entity to index
                  std::stringstream theIndexStream;
                  indexes[anEntity->getName()] = std::make_unique<Index>(*storage, storage->getFreeBlock(), theResult.value);
                  theResult = indexes[anEntity->getName()]->encode(theIndexStream);
                  if (theResult) { // save index
                      StorageInfo theInfo1(0, theIndexStream.str().size(),
                                           indexes[anEntity->getName()]->getIndexBlockNum(), BlockType::index_block);
                      theResult = storage->save(theIndexStream, theInfo1);
                      if (theResult) {
                          //store in metablock for easy accessing where the index is
                          std::stringstream theDatabaseStream;
                          theResult = encode(theDatabaseStream);
                          if (theResult) {
                              StorageInfo theInfo2(0, theDatabaseStream.str().size(), 0, BlockType::meta_block);
                              theResult = storage->save(theDatabaseStream, theInfo2);
                          }
                          Entities.push_back(anEntity);
                          anOutput << "Query OK, 1 row affected (" << std::fixed << Config::getTimer().elapsed()
                                   << "secs)" << std::endl;
                      }
                  }
              }
          }
          return theResult;
      }
      else return StatusResult{tableExists};
    }

    StatusResult Database::showEntity(std::ostream &anOutput) {
        anOutput << "+----------------------+\n";
        anOutput << "| Tables_in_" << std::left<<std::setw(10) << name << " |\n";
        anOutput << "+----------------------+\n";
        int theNum = 0;
        for (auto &index : indexes){
            theNum++;
            anOutput << "| " << std::left<<std::setw(20) << index.first << " |\n";
        }
        anOutput << "+----------------------+\n";
        anOutput << theNum << " rows in set (" << std::fixed << Config::getTimer().elapsed() << "sec.)" <<std::endl;
        return StatusResult{noError};
    }

    StatusResult Database::dropEntity(std::string aName, std::ostream &anOutput) {
    if (!indexes.count(aName)) return StatusResult{unknownEntity};

    size_t eraseCount{0};
    for (auto& someData : indexes[aName]->getData()) {
        storage->releaseBlocks(someData.second, true);
        eraseCount++;
    }
    indexes[aName]->getData().clear();
    storage->releaseBlocks(indexes[aName]->getEntityBlockNum(), true);
    storage->releaseBlocks(indexes[aName]->getIndexBlockNum(), true);
    eraseCount++;
    indexes.erase(aName);

    std::stringstream theDatabaseStream;
    StatusResult theResult = encode(theDatabaseStream);
    if (theResult){
        StorageInfo theInfo2(0, theDatabaseStream.str().size(), 0, BlockType::meta_block);
        theResult = storage->save(theDatabaseStream, theInfo2);
    }
    anOutput << "Query OK, "<<eraseCount<<" rows affected (" << std::fixed << Config::getTimer().elapsed() << "secs)"<<std::endl;
    return theResult;
    }

    StatusResult Database::describeEntity(const std::string &aName, std::ostream &anOutput) {
    if (!indexes.count(aName)) return StatusResult{unknownEntity};
    std::stringstream ss;
    StatusResult theResult = storage->load(ss, indexes[aName]->getEntityBlockNum());
    if (theResult){
        Entity theEnt(aName);
        theEnt.decode(ss);
        theEnt.showAttributes(anOutput);
    }
    return theResult;
    }

    StatusResult Database::insertRows(std::string aName, const RowCollection& rows, std::ostream &anOutput) {
        if (!indexes.count(aName)) return StatusResult{unknownEntity};

        std::stringstream ss;
        StatusResult theResult = storage->load(ss, indexes[aName]->getEntityBlockNum());
        if (theResult){
            Entity theEnt(aName);
            theEnt.decode(ss);
            std::string theRowKey;
            for(auto& theRow: rows){
                theResult = theEnt.addRow(theRow.get(),storage, aName);
                IndexKey aKey{indexes[aName]->toIndexKey(theRow->getData()[theEnt.getPrimaryKey()->getName()])};
                indexes[aName]->setKeyValue(aKey, theResult.value);
                //for cache

                if (aKey.index()==0){ //Index Key is a int
                    theRowKey = (aName + std::to_string(std::get<uint32_t>(aKey)));
                } 
                else{
                    theRowKey = aName + std::get<std::string>(aKey);
                }
                auto RowKey = Helpers::hashString(theRowKey.c_str());
                theRowCache.put(RowKey, theRow);
              //add and save index block
            }
        }

        std::stringstream theIndexStream;
        theResult = indexes[getEntity(aName)->getName()]->encode(theIndexStream);
        if (theResult) { // save index
            StorageInfo theInfo1(0, theIndexStream.str().size(),
                                 indexes[getEntity(aName)->getName()]->getIndexBlockNum(), BlockType::index_block);
            theResult = storage->save(theIndexStream, theInfo1);

        }
        std::stringstream theEntityStream;
        theResult = getEntity(aName)->encode(theEntityStream);
        if (theResult) { // save entity
            StorageInfo theInfo(0, theEntityStream.str().size(),
                                indexes[getEntity(aName)->getName()]->getEntityBlockNum(), BlockType::entity_block,
                                Helpers::hashString(getEntity(aName)->getName().c_str()));
            theResult = storage->save(theEntityStream, theInfo);
        }
        if (!theResult) { return theResult;}
        stream.flush();
        anOutput << "Query OK, "<< rows.size() << " row affected (" << std::fixed << Config::getTimer().elapsed() << "secs)"<<std::endl;
        anOutput.flush();
        return theResult;
    }


    StatusResult Database::selectRows(Query *aQuery, RowCollection &aRows, JoinList &theJoins, std::ostream &anOutput) {
        if (!indexes.count(aQuery->getTableName())) return StatusResult{unknownEntity};
        for (const auto& aJoin : theJoins) {
            if (!indexes.count(aJoin.table)) return StatusResult{unknownEntity};
        }

        StatusResult theResult{noError};
        Entity *theEnt = getEntity(aQuery->getTableName());

        if (theJoins.empty()) { // normal select query
            aRows = doSelectRows(aQuery, theResult, anOutput);
        }
        else {
            for (auto aJoin : theJoins) {
                aRows = doSingleJoin(aQuery, aJoin, anOutput);
            }
        }
        //added for filter
        size_t idx = 0; //I did this
        if (aQuery->getFilters()) {
            while (idx < aRows.size()) {
                if (!aQuery->getFilters()->matches(aRows[idx]->getData())) {
                    aRows.erase(aRows.begin() + idx);
                } else {
                    idx++;
                }
            }
        }
        //order by
        if(aQuery->isOrder()){
            std::sort(aRows.begin(),aRows.end(), std::bind([](std::shared_ptr<Row>& firstRow, std::shared_ptr<Row>& secondRow, const std::string& attribute){
                return isLess(firstRow->getData()[attribute], secondRow->getData()[attribute]);
            },std::placeholders::_1, std::placeholders::_2, aQuery->getOrderBy() ));
        }
        // limit by
        size_t theLimit = aQuery->getLimit();
        size_t theSize = aRows.size();

        idx = theLimit;
        while (idx<theSize) {
            aRows.pop_back();
            idx++;
        }

        TabularView theView(anOutput, *theEnt, aRows, aQuery);
        theView.show(anOutput);
        return theResult;
    }

    RowCollection Database::doSelectRows(Query *aQuery, StatusResult &aResult, std::ostream &anOutput) {
        RowCollection aRows;
        Entity *theEnt = getEntity(aQuery->getTableName());
        for (auto& someData : indexes[aQuery->getTableName()]->getData()) {
            std::string theRowKey;
            aRows.push_back(std::make_shared<Row>());
            if (someData.first.index()==0){ //Index Key is a int
                theRowKey = (aQuery->getTableName()) + std::to_string(std::get<uint32_t>(someData.first));
            }
            else{
                theRowKey = (aQuery->getTableName()) + std::get<std::string>(someData.first);
            } 
            auto RowKey = Helpers::hashString(theRowKey.c_str());
            if(theRowCache.contains(RowKey)){
                theRowCache.get(RowKey, aRows.back());
            }
            else{
                std::stringstream theStream;
                storage->load(theStream, someData.second);
                theEnt->showRow(aRows.back(), theStream);
                theRowCache.put(RowKey, aRows.back());
            }
        }
        return aRows;
    }

    RowCollection Database::doSingleJoin(Query *aQuery, Join &aJoin, std::ostream &anOutput) {
        RowCollection leftRows;
        auto *leftEnt = getEntity(aQuery->getTableName());
        RowCollection rightRows;
        auto *rightEnt = getEntity(aJoin.table);
        

        for (auto& someData : indexes[aQuery->getTableName()]->getData()) {
            std::string theRowKey;
            leftRows.push_back(std::make_shared<Row>());
            if (someData.first.index()==0){ //Index Key is a int
                theRowKey = (aQuery->getTableName()) + std::to_string(std::get<uint32_t>(someData.first));
            } 
            else{
                theRowKey = (aQuery->getTableName()) + std::get<std::string>(someData.first);
            }
            auto RowKey = Helpers::hashString(theRowKey.c_str());
            if(theRowCache.contains(RowKey)){
                theRowCache.get(RowKey, leftRows.back());
            }
            else{
                Block theBlock(BlockType::data_block);
                storage->readBlock(someData.second, theBlock);
                if (theBlock.header.entity ==
                    Helpers::hashString(aQuery->getTableName().c_str())) { // same an entity name same as name?
                    std::stringstream theStream;
                    storage->load(theStream, someData.second);
                    leftEnt->showRow(leftRows.back(), theStream);
                    theRowCache.put(RowKey, leftRows.back());
                }
            }
        }

        for (auto& someData : indexes[aJoin.table]->getData()) {
            std::string theRowKey;
            rightRows.push_back(std::make_shared<Row>());
            if (someData.first.index()==0){ //Index Key is a int
                theRowKey = (aJoin.table) + std::to_string(std::get<uint32_t>(someData.first));
            }
            else{
                theRowKey = (aJoin.table) + std::get<std::string>(someData.first);
            }
            auto RowKey = Helpers::hashString(theRowKey.c_str());
            if(theRowCache.contains(RowKey)){
                theRowCache.get(RowKey, rightRows.back());
            }
            else{
                Block theBlock(BlockType::data_block);
                storage->readBlock(someData.second, theBlock);
                if (theBlock.header.entity == Helpers::hashString(aJoin.table.c_str())){ // same an entity name same as name?
                    std::stringstream theStream;
                    storage->load(theStream, someData.second);
                    rightEnt->showRow(rightRows.back(), theStream);
                    theRowCache.put(RowKey, rightRows.back());
                }
            }
        }
        leftRows = mergeRows(leftRows, rightRows, aJoin, aQuery->getTableName());
        return leftRows;
    }

    StatusResult Database::updateRows(std::string &aAttributeName, Value &aValue, Query *aQuery, RowCollection &aRows,
                                      std::ostream &anOutput) {
        if (indexes.count(aQuery->getTableName()) == 0) return StatusResult{unknownEntity};

        std::stringstream ss;
        StatusResult theResult = storage->load(ss, indexes[aQuery->getTableName()]->getEntityBlockNum());
        Entity theEnt(aQuery->getTableName());
        theEnt.decode(ss);

        for (auto& someData : indexes[aQuery->getTableName()]->getData()) {
            Block theBlock(BlockType::data_block);
            storage->readBlock(someData.second, theBlock);
            if (theBlock.header.entity == Helpers::hashString(aQuery->getTableName().c_str())){ // same an entity name same as name?
                std::stringstream theStream;
                storage->load(theStream, someData.second);
                aRows.push_back(std::make_shared<Row>());
                theEnt.showRow(aRows.back(), theStream);
                aRows.back()-> blockNumber = someData.second;
            }
        }

        //added for filter
        size_t idx = 0; //I did this
        if (aQuery->getFilters()) {
            while (idx < aRows.size()) {
                if (!aQuery->getFilters()->matches(aRows[idx]->getData())) {
                    aRows.erase(aRows.begin() + idx);
                } else {
                    idx++;
                }
            }
        }

        //update rows and storage
        for (auto & aRow : aRows){
            aRow->getData()[aAttributeName] = aValue;
            std::stringstream theRowStream;
            StatusResult theResult = aRow->encode(theRowStream, const_cast<AttributeList &>(theEnt.getAttributes()));
            if (theResult){
                StorageInfo theInfo(0, theRowStream.str().size(), aRow->blockNumber, BlockType::data_block, Helpers::hashString(aQuery->getTableName().c_str()));
                storage->save(theRowStream, theInfo);
                // FIXME: I think I should be saving the updated index, but it fails when i do
//                IndexKey aKey{indexes[aQuery->getTableName()]->toIndexKey(aRow->getData()[theEnt.getPrimaryKey()->getName()])};
//                indexes[aQuery->getTableName()]->setKeyValue(aKey, theResult.value);
            }

        }

        anOutput<< "Query Ok.";
        anOutput << aRows.size() << " rows in set (" << std::fixed << Config::getTimer().elapsed() << "sec.)" <<std::endl;
        anOutput.flush();

        return theResult;
    }


    StatusResult Database::deleteRows(Query *aQuery, RowCollection &aRows, std::ostream &anOutput) {
        if (!indexes.count(aQuery->getTableName())) return StatusResult{unknownEntity};
        std::stringstream ss;
        StatusResult theResult = storage->load(ss, indexes[aQuery->getTableName()]->getEntityBlockNum());
        Entity theEnt(aQuery->getTableName());
        theEnt.decode(ss);

        for (auto& someData : indexes[aQuery->getTableName()]->getData()) {
            Block theBlock(BlockType::data_block);
            storage->readBlock(someData.second, theBlock);
            if (theBlock.header.entity == Helpers::hashString(aQuery->getTableName().c_str())){ 
                std::stringstream theStream;
                storage->load(theStream, someData.second);
                aRows.push_back(std::make_shared<Row>());
                theEnt.showRow(aRows.back(), theStream);
                aRows.back()-> blockNumber = someData.second;
            }
        }

        //added for filter
        size_t idx = 0; //I did this
        if (aQuery->getFilters()) {
            while (idx < aRows.size()) {
                if (!aQuery->getFilters()->matches(aRows[idx]->getData())) {
                    aRows.erase(aRows.begin() + idx);
                } else {
                    idx++;
                }
            }
        }

        for (size_t i = 0; i < aRows.size(); i++){
            storage->releaseBlocks(aRows[i]->blockNumber,true);
                IndexKey aKey{indexes[aQuery->getTableName()]->toIndexKey(aRows.at(i)->getData()[theEnt.getPrimaryKey()->getName()])};
                indexes[aQuery->getTableName()]->erase(std::get<int>(aRows.at(i)->getData()[theEnt.getPrimaryKey()->getName()]));
        }

        std::stringstream theIndexStream;
        theResult = indexes[aQuery->getTableName()]->encode(theIndexStream);
        if (theResult) { // save index
            StorageInfo theInfo1(0, theIndexStream.str().size(),
                                 indexes[aQuery->getTableName()]->getIndexBlockNum(), BlockType::index_block);
            theResult = storage->save(theIndexStream, theInfo1);
        }


        anOutput<< "Query Ok, ";
        anOutput << aRows.size() << " rows in set (" << std::fixed << Config::getTimer().elapsed() << "sec.)" <<std::endl;
        anOutput.flush();

        return theResult;
    }

    StatusResult Database::showIndexes(std::ostream &anOutput) {
        anOutput<<"+-----------------+-----------------+\n";
        anOutput<<"| table           | field(s)        |\n";
        anOutput<<"+-----------------+-----------------+\n";
        for (auto &index : indexes) {
            anOutput << "| " << std::left<<std::setw(15) << index.first << " |";
            anOutput << std::left<<std::setw(15) << index.second->getName() << " |\n";
        }
        anOutput<<"+-----------------+-----------------+\n";
        anOutput << indexes.size() << " rows in set (" << std::fixed << Config::getTimer().elapsed() << "secs)" <<std::endl;
        anOutput.flush();
        return StatusResult{noError};
    }
    StatusResult Database::showIndexesByEntity(std::string &aName, StringList &aList, std::ostream &anOutput) {
        anOutput<<"+-----------------+-----------------+\n";
        anOutput<<"| key             | block#          | \n";
        anOutput<<"+-----------------+-----------------+\n";

        std::map<IndexKey, uint32_t> theData = indexes[aName]->getData();
        size_t theSize = theData.size();
        for (auto &someData: theData) {
            std::visit([&anOutput](const auto &x) { anOutput << "| " << std::left<<std::setw(15) << x << " |"; }, someData.first);
            anOutput << std::left << std::setw(15) << someData.second << " |\n";
        }
        anOutput<<"+-----------------+-----------------+\n";
        anOutput << theSize << " rows in set (" << std::fixed << Config::getTimer().elapsed() << "secs)" <<std::endl;
        anOutput.flush();
        return StatusResult{noError};
    }


   Entity* Database::getEntity(const std::string &aName) {
     for (auto entity : Entities) {
         if (entity->getName() == aName) {
             return entity;
         }
     }
     std::stringstream ss;
    StatusResult theResult = storage->load(ss, indexes[aName]->getEntityBlockNum());
     if (theResult) {
         auto *theEnt = new Entity(aName);
         theEnt->decode(ss);
         return theEnt;
     }
     return nullptr;
   }


  StatusResult Database::encode(std::ostream &anOutput){
      anOutput<<indexes.size()<<" ";
      for(auto& index: indexes){
          anOutput<<index.first.size()<<" "<<index.first<<" ";
          anOutput << index.second->getIndexBlockNum() << " ";
          anOutput << index.second->getEntityBlockNum() << " ";
          index.second->encode(anOutput);
      }
      return StatusResult{noError};
    
  }
  
  StatusResult Database::decode(std::istream &anInput){
    int theSize;
    anInput>>theSize;
    anInput.get();
    for(int i = 0; i < theSize; i++){
        int aSize;
        anInput>>aSize;
        anInput.get();
        char theBuf[aSize+1];
        anInput.read(theBuf, aSize);
        theBuf[aSize] = 0;
        uint32_t aIndexBlockNum;
        anInput>>aIndexBlockNum;
        anInput.get();
        uint32_t aEntityBlockNum;
        anInput>>aEntityBlockNum;
        anInput.get();
        Index anIdx(*storage, aIndexBlockNum, aEntityBlockNum);
        std::stringstream sstream;
        storage->load(sstream, aIndexBlockNum);
        anIdx.decode(sstream);
        indexes[theBuf] = std::make_unique<Index>(anIdx);
    }
    return StatusResult{noError};
  }

  bool Database::checkTableExists(const std::string &aName) {
    loadEntities();
    for (auto entity : Entities) {
        if (entity->getName() == aName) {
            return true;
        }
    }
    return false;
  }


    bool Database::checkAttributesExists(const std::string &tableName, std::vector<std::string> aList) const {
      for (auto entity : Entities) {
          if (entity->getName() == tableName){
              for (auto& str : aList) {
                  if (!entity->getAttribute(str)) {
                      return false;
                  }
              }
          }
      }
      return true;
    }

    StatusResult Database::loadEntities() {
        Entities.clear();
        StatusResult theResult = noError;

        for(auto iter = indexes.begin(); iter != indexes.end(); ++iter) {
            std::stringstream ss;
            theResult = storage->load(ss, iter->second->getEntityBlockNum());
            if (theResult){
                auto *theEnt = new Entity(iter->first);
                theEnt->decode(ss);
                Entities.push_back(theEnt);
            }
        }

        return theResult;
    }

    RowCollection Database::mergeRows(RowCollection &leftRows, RowCollection &rightRows, Join &aJoin, std::string aTableName) {
      RowCollection combinedRows;
      if (Keywords::left_kw == aJoin.joinType) {
          for (const auto& aLeftRow : leftRows) {
              size_t theCount = 0;
              for (const auto& aRightRow : rightRows) {
                  if (aRightRow->getData()[aJoin.onRight.name] == aLeftRow->getData()[aJoin.onLeft.name]) {
                      theCount++;
                      combinedRows.push_back(std::make_shared<Row>());
                      for (const auto &anAtt : aLeftRow->getData()) {
                          combinedRows.back() = aRightRow;
                          combinedRows.back()->getData()[anAtt.first] = aLeftRow->getData()[anAtt.first];
                      }
                  }
              }
              if (theCount == 0) {
                  combinedRows.push_back(std::make_shared<Row>());
                  combinedRows.back() = aLeftRow;
              }
          }
      }
      else {
          for (const auto& aRightRow : rightRows) {
              size_t theCount = 0;
              for (const auto& aLeftRow : leftRows) {
                  if (aRightRow->getData()[aJoin.onRight.name] == aLeftRow->getData()[aJoin.onLeft.name]) {
                      theCount++;
                      combinedRows.push_back(std::make_shared<Row>());
                      for (const auto &anAtt : aLeftRow->getData()) {
                          combinedRows.back() = aRightRow;
                          combinedRows.back()->getData()[anAtt.first] = aLeftRow->getData()[anAtt.first];
                      }
                  }
              }
              if (theCount == 0) {
                  combinedRows.push_back(std::make_shared<Row>());
                  combinedRows.back() = aRightRow;
              }
          }
      }
      return combinedRows;
    }
}
