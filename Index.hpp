//
//  Index.hpp
//  RGAssignment3
//
//  Created by rick gessner on 4/2/21.
//

#ifndef Index_hpp
#define Index_hpp

#include <stdio.h>
#include <map>
#include <map>
#include <functional>
#include "Storage.hpp"
#include "BasicTypes.hpp"
#include "Errors.hpp"
#include "BlockIterator.hpp"


namespace ECE141 {

  enum class IndexType {intKey=0, strKey};
  
  using IndexVisitor =
    std::function<bool(const IndexKey&, uint32_t)>;
    
  struct Index : public Storable, BlockIterator {
    
    Index(Storage &aStorage, uint32_t aBlockNum=0, uint32_t entBlockNum=0,
          IndexType aType=IndexType::intKey)
        : storage(aStorage), type(aType), indexBlockNum(aBlockNum), entityBlockNum(entBlockNum) {
          changed=false;
          entityId=0;
        }
    
    class ValueProxy {
    public:
      Index     &index;
      IndexKey  key;
      IndexType type;
      
      ValueProxy(Index &anIndex, uint32_t aKey)
        : index(anIndex), key(aKey), type(IndexType::intKey) {}
      
      ValueProxy(Index &anIndex, const std::string &aKey)
        : index(anIndex), key(aKey), type(IndexType::strKey) {}
      
      ValueProxy& operator= (uint32_t aValue) {
        index.setKeyValue(key,aValue);
        return *this;
      }
      
      operator IntOpt() {return index.valueAt(key);}
    }; //value proxy
    
    ValueProxy operator[](const std::string &aKey) {
      return ValueProxy(*this,aKey);
    }

    ValueProxy operator[](uint32_t aKey) {
      return ValueProxy(*this,aKey);
    }
      
    uint32_t getIndexBlockNum() const {return indexBlockNum;}

    uint32_t getEntityBlockNum() const {return entityBlockNum;}

    std::map<IndexKey, uint32_t> getData() const {return data;}

    Index&   setBlockNum(uint32_t aBlockNum) {
        indexBlockNum=aBlockNum;
               return *this;
             }
    
    bool    isChanged() {return changed;}

    Index&  setChanged(bool aChanged) {
      changed=aChanged; return *this;      
    }
        
    StorageInfo getStorageInfo(size_t aSize) {
      //student complete...
      return StorageInfo{0, aSize};
    }
            
    IntOpt valueAt(IndexKey &aKey) {
      return exists(aKey) ? data[aKey] : (IntOpt)(std::nullopt);
    }

    bool setKeyValue(IndexKey &aKey, uint32_t aValue) {
      data[aKey]=aValue;
      return changed=true; //side-effect indended!
    }
        
    StatusResult erase(const std::string &aKey) {
        //student implement
//        if (exists(aKey)) return StatusResult{Errors::unknownIndex};
        data.erase(aKey);
        return StatusResult{Errors::noError};
    }

    StatusResult erase(uint32_t aKey) {
        //student implement
//        if (exists((aKey))) return StatusResult{Errors::unknownIndex};
        data.erase(aKey);
        return StatusResult{Errors::noError};
    }

    size_t getSize() {return data.size();}
    
    bool exists(IndexKey &aKey) {
      return data.count(aKey);
    }
      
    StatusResult encode(std::ostream &anOutput) override {
      //student implement
        anOutput<<data.size()<<" ";
        for(auto& someData: data){
            switch (type) {
                case IndexType::intKey: {
                    anOutput << "0 ";
                    int aInt = std::get<uint32_t>(someData.first);
                    anOutput << aInt << " ";
                    anOutput << someData.second << " ";
                    break;
                }
                case IndexType::strKey: {
                    anOutput << "1 ";
                    std::string aString = std::get<std::string>(someData.first);
                    anOutput << aString.size() << " ";
                    anOutput << aString << " ";
                    anOutput << someData.second << " ";
                    break;
                }
                default:
                    break;
            }
        }
      return StatusResult{Errors::noError};
    }
    
    StatusResult decode(std::istream &anInput) override {
        //student implement...
        int theSize;
        anInput>>theSize;
        anInput.get();
        for(int i = 0; i < theSize; i++){
            int aValue;
            anInput>>aValue;
            anInput.get();
            if (aValue) {
                type = IndexType::strKey;
                uint32_t theValue;
                anInput>>theValue;
                anInput.get();
                char theBuf[theValue+1];
                anInput.read(theBuf, theValue);
                theBuf[theValue] = 0;
                IndexKey aKey = static_cast<std::string>(theBuf);
                anInput>>theValue;
                anInput.get();
                data[aKey] = theValue;
            }
            else {
                type = IndexType::intKey;
                uint32_t theValue;
                anInput>>theValue;
                anInput.get();
                IndexKey aKey = theValue;
                anInput>>theValue;
                anInput.get();
                data[aKey] = theValue;
            }
        }
      return StatusResult{Errors::noError};
    }
    
    //visit blocks associated with index
    bool each(BlockVisitor aVisitor) /*override*/ {
      Block theBlock;
      for(auto thePair : data) {
        if(storage.readBlock(thePair.second, theBlock)) {
          if(!aVisitor(theBlock,thePair.second)) {return false;}
        }
      }
      return true;
    }

    //visit index values (key, value)...
    bool eachKV(IndexVisitor aCall) {
      for(auto thePair : data) {
        if(!aCall(thePair.first,thePair.second)) {
          return false;
        }
      }
      return true;
    }

    std::string getName() {return name;}

    IndexKey toIndexKey(const Value &aValue) const {
        if (std::holds_alternative<int>(aValue)) {
            IndexKey aKey{(uint32_t)std::get<int>(aValue)};
            return aKey;
        }
        IndexKey aKey{std::get<std::string>(aValue)};
        return aKey;
    }
    
  protected:
    
    Storage                       &storage;
    std::map<IndexKey, uint32_t>  data; // row's primary key, row's block#
    IndexType                     type;
    std::string                   name;
    bool                          changed;
    uint32_t                      indexBlockNum; //where index storage begins
    uint32_t                      entityBlockNum;
    uint32_t                      entityId;
  }; //index

  using IndexMap = std::map<std::string, std::unique_ptr<Index> >;

}


#endif /* Index_hpp */
