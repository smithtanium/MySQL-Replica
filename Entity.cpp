//
//  Entity.cpp
//  PA3
//
//  Created by rick gessner on 3/2/22.
//

#include <vector>
#include <memory>
#include <string>
#include <iomanip>
#include "Entity.hpp"


namespace ECE141 {

 //STUDENT: Implement this class...

  Entity::Entity(const std::string aName) :
    name(aName),  autoincr(0), primarykey(nullptr) {}

  Entity::Entity(const Entity &aCopy) {
    name=aCopy.name;
    autoincr=aCopy.autoincr;
  }

  Entity::~Entity() {
    //std::cout << "~Entity()\n";
  }
 
  Entity& Entity::addAttribute(const Attribute& anAttribute) {
    if(!getAttribute(anAttribute.getName())) {
      attributes.push_back(anAttribute);
    }
    if (anAttribute.isPrimaryKey()){
      primarykey = &attributes.back();
    }
    return *this;
  }

  Attribute* Entity::getAttribute(const std::string &aName) {
      for (auto &attribute : attributes) {
          if (attribute.getName() == aName) {
              return &attribute;
          }
      }
      return nullptr;
  }
  
  // USE: ask the entity for name of primary key (may not have one...)
  const Attribute* Entity::getPrimaryKey() const {
    return primarykey;
  }

  StatusResult Entity::encode(std::ostream &anOutput){
    anOutput<<name.size()<<" "<<name;

    std::stringstream ss;
    ss << autoincr;
    std::string str;
    ss >> str;
    anOutput<<" "<<str.size() << " " << str;

    anOutput<<" "<<attributes.size() << " ";
    for(auto& theAttribute: attributes){
      theAttribute.encode(anOutput);
    }

    return StatusResult{noError};
  }

  StatusResult Entity::decode(std::istream &anInput){
    int theSize;
    anInput>>theSize;
    anInput.get();
    char buf[theSize+1];
    anInput.read(buf, theSize);
    buf[theSize] = 0;
    name = buf;

    anInput>>theSize;
    anInput.get();
    //buf[theSize+1];
    anInput.read(buf, theSize);
    buf[theSize] = 0;
    std::string aTempString = buf;
    autoincr = std::stoi(aTempString);

//    uint32_t anAutoincrNum;
//    anInput>>anAutoincrNum;
//    autoincr = anAutoincrNum;
    anInput>>theSize;
    anInput.get();
    for(int i = 0; i < theSize; i++){
      Attribute theAtt;
      theAtt.decode(anInput);
      attributes.push_back(theAtt);
    }
    int theBuf;
    anInput.get();

    for(int i = 0; i < attributes.size(); i++){
      attmap[attributes[i].getName()] = i;
      if(attributes[i].isPrimaryKey()){
        primarykey = &attributes[i];
      }
    }
    return StatusResult{noError};
  }



  void Entity::showAttributes(std::ostream &anOutput){
      anOutput<<"+-------------+--------------+------+-----+---------+-----------------------------+"<<std::endl;
      anOutput<<"| Field       | Type         | Null | Key | Default | Extra                       |"<<std::endl;
      anOutput<<"+-------------+--------------+------+-----+---------+-----------------------------+"<<std::endl;
      int i = 0;
      for (const auto& theAtt:attributes){
        i++;
        anOutput<<std::left<<"| " <<std::setw(11)<< theAtt.getName() <<  " | " <<std::setw(12)<< Helpers::dataTypeToString(theAtt.getType())
        << " | " <<std::setw(4)<< (theAtt.isNullable() ? "YES" : "NO")
        << " | " <<std::setw(3)<< (theAtt.isPrimaryKey() ? "YES" : "NO")
        << " | NULL    | "
        <<std::setw(14)<<(theAtt.isAutoIncrement() ? "auto_increment" : " ")
        <<std::setw(13)<<(theAtt.isPrimaryKey() ? " primary key" : " ") << " |"<<std::endl;
      }
      anOutput<<"+-------------+--------------+------+-----+---------+-----------------------------+"<<std::endl;
      anOutput<<i<< " rows in set ("<< std::fixed << Config::getTimer().elapsed() << "secs)"<<std::endl;
  }

    StatusResult Entity::addRow(Row *aRow, Storage* aStorage, std::string &aName){
        std::stringstream theRowStream;
        StatusResult theResult = aRow->encode(theRowStream, attributes);

        if(theResult){
            StorageInfo theInfo(0, theRowStream.str().size(), -1, BlockType::data_block, Helpers::hashString(aName.c_str()));
            theResult = aStorage->save(theRowStream, theInfo);
        }

        return theResult;
    }


    StatusResult Entity::showRow(std::shared_ptr<Row>& aRow, std::stringstream& aStream){
        StatusResult theResult = aRow->decode(aStream, attributes);
        return theResult;
    }
}