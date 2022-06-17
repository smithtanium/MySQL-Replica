//
//  Attribute.cpp
//  Assignment4
//
//  Created by rick gessner on 4/18/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#include <iostream>
#include "Attribute.hpp"
#include "BasicTypes.hpp"
#include "Helpers.hpp"

namespace ECE141 {

  Attribute::Attribute(DataTypes aType)
    : type(aType),size(0),autoIncrement(0),primary(0),nullable(1) {}
 
  Attribute::Attribute(std::string &aName, DataTypes aType, uint32_t aSize)  {
    name=aName;
    type=aType;
    size=aSize;
    autoIncrement=0;
    primary=0;
    nullable=1;
  }
 
  Attribute::Attribute(const Attribute &aCopy)  {
    name=aCopy.name;
    type=aCopy.type;
    size=aCopy.size;
    autoIncrement=aCopy.autoIncrement;
    primary=aCopy.primary;
    nullable=aCopy.nullable;
  }
 
  Attribute::~Attribute()  {
  }

  StatusResult  Attribute::encode(std::ostream &anOutput){
    anOutput<<name.size()<<" "<<name;
    anOutput<<" "<<type;
    anOutput<<" "<<size;
    anOutput<<" "<<autoIncrement;
    anOutput<<" "<<primary;
    anOutput<<" "<<nullable;
    anOutput<<" ";

    return StatusResult{noError};
  }

  StatusResult  Attribute::decode(std::istream &anInput){
    int theSize;
    anInput>>theSize;
    anInput.get();
//    char* buf = new char[theSize];
    char buf[theSize+1];
    anInput.read(buf, theSize);
    buf[theSize] = 0;
    name = buf;
    DataTypes theDTBuf;
    anInput>>theDTBuf;
    type = theDTBuf;
    anInput.get();
    int theBuf;
    anInput>>theBuf;
    size = theBuf;
    anInput.get();
    anInput>>theBuf;
    autoIncrement = theBuf;
    anInput.get();
    anInput>>theBuf;
    primary = theBuf;
    anInput.get();
    anInput>>theBuf;
    nullable = theBuf;
    anInput.get();

    return StatusResult{noError};
  }
 
  Attribute& Attribute::setName(std::string &aName)  {
      name = aName; //I added this
      return *this;
  }
 
  Attribute& Attribute::setDataType(DataTypes aType) {
    type=aType;
    return *this;
  }

  Attribute& Attribute::setSize(int aSize) {
    size=aSize; return *this;
  }

  Attribute& Attribute::setAutoIncrement(bool anAuto) {
    autoIncrement=anAuto; return *this;
  }

  Attribute& Attribute::setPrimaryKey(bool aPrimary) {
    primary=aPrimary; return *this;
  }

  Attribute& Attribute::setNullable(bool aNullable) {
    nullable=aNullable; return *this;
  }

  bool Attribute::isValid() {
    return true;
  }

    Attribute& Attribute::reset() {
        name="";
        type=DataTypes::no_type;
        size=0;
        autoIncrement=0;
        primary=0;
        nullable=1;
        return *this;
  }

    void Attribute::encodeValue(std::ostream &anOutput, Value aValue){
        switch (type) {
            case DataTypes::bool_type:{
                bool aBool = std::get<bool>(aValue);
                anOutput<<aBool<< " ";
                break;
            }
            case DataTypes::int_type:{
                int aInt = std::get<int>(aValue);
//                std::visit([&anOutput](const auto& v) { anOutput << v << " "; },aValue);
                anOutput<<aInt<< " ";
                break;
            }
            case DataTypes::float_type:{
                double aDouble = std::get<double>(aValue);
                anOutput<<aDouble<< " ";
                break;
            }
            case DataTypes::varchar_type:{
                std::string aString = std::get<std::string>(aValue);
                anOutput<<aString.size()<<" ";
                anOutput<<aString << " ";
                break;
            }
            default:
                break;
        }
    }

    Value Attribute::decodeValue(std::istream &anInput){
//        std::cout << anInput.rdbuf();
        switch (type) {
            case DataTypes::bool_type:{
                bool theBuf;
                anInput>>theBuf;
                anInput.get();
                return theBuf;
                break;
            }
            case DataTypes::int_type:{
                int theBuf;
                anInput>>theBuf;
                anInput.get();
                return theBuf;
                break;
            }
            case DataTypes::float_type:{
                double theBuf;
                anInput>>theBuf;
                anInput.get();
                return theBuf;
                break;
            }
            case DataTypes::varchar_type:{
                int theSize;
                anInput>>theSize;
                anInput.get();
                //                char* theBuf = new char[theSize+1];
                char theBuf[theSize+1];
                anInput.read(theBuf, theSize);
                theBuf[theSize] = 0;
                return toValue(theBuf);
                break;
            }
            default:
                break;
        }
        return nullptr;
    }

  Value Attribute::toValue(const std::string &aValue) const {
    //might need to do type-casting here...
      if (Helpers::checkInt(const_cast<std::string &>(aValue))) { return std::stoi(aValue);}
      if (Helpers::checkDouble(const_cast<std::string &>(aValue))) { return std::stod(aValue);}
    return Value{aValue};
  }
}
