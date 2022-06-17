//
//  Row.cpp
//  PA3
//
//  Created by rick gessner on 4/2/22.
//



#include <string>
#include "Row.hpp"


namespace ECE141 {
  
  Row::Row(uint32_t entityId) {}
  Row::Row(const Row &aRow) {*this=aRow;}

  Row::~Row() {}

  Row& Row::operator=(const Row &aRow) {return *this;}
  bool Row::operator==(Row &aCopy) const {return false;}

  //STUDENT: What other methods do you require?
                      
  Row& Row::set(const std::string &aKey,const Value &aValue) {
      data[aKey] = aValue; // I did this
      return *this;
  }

    // StatusResult  Row::encodeRow(Row &aRow, std::string aEntityName){
    //   Entity theEnt(aEntityName);
    //   AttributeList theAttributes = theEnt.getAttributes();
    //   std::stringstream theStream;
    //   for (const auto& theAtt:theAttributes){
    //     if (aRow.getData().count(theAtt.getName()) != 0) {
    //       aRow.encode(theStream);
    //       StorageInfo theInfo(0, theStream.str().size(), -1, BlockType::data_block);
    //       StatusResult theResult = storage->save(thStream, theInfo);
    //     }
    //   }
    // }

    StatusResult Row::encode(std::ostream &anOutput, AttributeList& attributes){
      for (auto& theAtt:attributes){
            if (data.count(theAtt.getName())) {
//                std::visit([](const auto& v) { std::cout << v << std::endl; },data[theAtt.getName()]);
                theAtt.encodeValue(anOutput, data[theAtt.getName()]);
            }
            else {
                theAtt.encodeValue(anOutput, theAtt.toValue(""));
            }
        }
        return StatusResult{noError};
    }

    StatusResult Row::decode(std::istream &anInput,AttributeList& attributes){
        for (auto& theAtt:attributes){
            data[theAtt.getName()] = theAtt.decodeValue(anInput);
        }
        return StatusResult{noError};
    }
}
