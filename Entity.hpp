//
//  Entity.hpp
//  Assignment3
//
//  Created by rick gessner on 3/18/22.
//  Copyright © 2022 rick gessner. All rights reserved.
//

#ifndef Entity_hpp
#define Entity_hpp

#include <stdio.h>
#include <vector>
#include <optional>
#include <memory>
#include <string>

#include "Attribute.hpp"
#include "Errors.hpp"
#include "BasicTypes.hpp"
#include "Storage.hpp"
#include "Helpers.hpp"
#include "Config.hpp"
#include "Row.hpp"

namespace ECE141 {
  
  using AttributeOpt = std::optional<Attribute>;
  using AttributeList = std::vector<Attribute>;

  //------------------------------------------------
  
  class Entity : public Storable{
  public:
                          Entity(const std::string aName);
                          Entity(const Entity &aCopy);
    
                          ~Entity();
    
    const std::string&    getName() const {return name;}
        
    const AttributeList&  getAttributes() const {return attributes;}
    Entity&               addAttribute(const Attribute& anAttribute);
    Attribute*            getAttribute(const std::string &aName);
    const Attribute*      getPrimaryKey() const;
    Attribute*            setPrimaryKey(Attribute* anAttribute) {return primarykey=anAttribute;}
    StatusResult          encode(std::ostream &anOutput)override;
    StatusResult          decode(std::istream &anInput)override;
    void                  showAttributes(std::ostream &anOutput);
    StatusResult          addRow(Row *aRow, Storage* aStorage, std::string &aName);

    StatusResult          showRow(std::shared_ptr<Row>& aRow, std::stringstream& aStream);
    uint32_t              getAUniqueKey() {return ++autoincr;}
    //    StatusResult          isValid();
           
  protected:
        
    AttributeList   attributes;
    std::string     name;
    uint32_t        autoincr;  //auto_increment
    std::map<std::string, int> attmap;  
    Attribute*      primarykey;
  };
  
}
#endif /* Entity_hpp */
