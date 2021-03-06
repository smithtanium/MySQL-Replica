//
//  Filters.cpp
//  Datatabase5
//
//  Created by rick gessner on 3/5/21.
//  Copyright © 2021 rick gessner. All rights reserved.
//

#include "Filters.hpp"
#include <string>
#include <limits>
#include "keywords.hpp"
#include "Helpers.hpp"
#include "Entity.hpp"
#include "Attribute.hpp"
#include "Compare.hpp"

namespace ECE141 {

    using Comparitor = bool (*)(Value &aLHS, Value &aRHS);

    bool equals(Value &aLHS, Value &aRHS) {
    bool theResult=false;

    std::visit([&](auto const &aLeft) {
        std::visit([&](auto const &aRight) {
            theResult=isEqual(aLeft,aRight);
        },aRHS);
    },aLHS);
    return theResult;
    }

    bool greater(Value &aLHS, Value &aRHS) {
        bool theResult=false;

        std::visit([&](auto const &aLeft) {
            std::visit([&](auto const &aRight) {
                theResult=isGreater(aLeft,aRight);
            },aRHS);
        },aLHS);
        return theResult;
    }

    bool less(Value &aLHS, Value &aRHS) {
        bool theResult=false;

        std::visit([&](auto const &aLeft) {
            std::visit([&](auto const &aRight) {
                theResult=isLess(aLeft,aRight);
            },aRHS);
        },aLHS);
        return theResult;
    }

    bool lessThanEqual(Value &aLHS, Value &aRHS) {
        return less(aLHS, aRHS) && equals(aLHS, aRHS);
    }

    bool greaterThanEqual(Value &aLHS, Value &aRHS) {
        return less(aLHS, aRHS) && equals(aLHS, aRHS);
    }

  static std::map<Operators, Comparitor> comparitors {
    {Operators::equal_op, equals},
    {Operators::lt_op, less},
    {Operators::gt_op, greater},
    {Operators::gte_op, greaterThanEqual},
    {Operators::lte_op, lessThanEqual}
    //STUDENT: Add more for other operators...
  };

  bool Expression::operator()(KeyValues &aList) {
    Value theLHS{lhs.value};
    Value theRHS{rhs.value};

    if(TokenType::identifier==lhs.ttype) {
      theLHS=aList[lhs.name]; //get row value
    }

    if(TokenType::identifier==rhs.ttype) {
      theRHS=aList[rhs.name]; //get row value
    }

    return comparitors.count(op)
      ? comparitors[op](theLHS, theRHS) : false;
  }

  //--------------------------------------------------------------
  
  Filters::Filters() {
  }
  
  Filters::Filters(const Filters &aCopy)  {
  }
  
  Filters::~Filters() {
    //no need to delete expressions, they're unique_ptrs!
  }

  Filters& Filters::add(Expression *anExpression) {
      expressions.push_back(std::unique_ptr<Expression>(anExpression));
      return *this;
  }
    
  //compare expressions to row; return true if matches
  bool Filters::matches(KeyValues &aList) const {
    
    //STUDENT: You'll need to add code here to deal with
    //         logical combinations (AND, OR, NOT):
    //         like:  WHERE zipcode=92127 AND age>20

      for(auto &theExpr : expressions) {
          switch((*theExpr).logic){
              case Logical::no_op:{
                  return (*theExpr)(aList);
                  break;
              }
              case Logical::and_op:{
                  if(!(*theExpr)(aList)) {
                      return false;
                  }
                  break;
              }
              case Logical::or_op:{
                  if((*theExpr)(aList)) {
                      return true;
                  }
                  break;
              }
              case Logical::not_op:{
                  if(!(*theExpr)(aList)) {
                      return true;
                  }
                  break;
              }
          }

      }
      return true;
  }



    //where operand is field, number, string...
  StatusResult parseOperand(Tokenizer &aTokenizer,
                            Entity &anEntity, Operand &anOperand) {
    StatusResult theResult{noError};
    Token &theToken = aTokenizer.current();
    if(TokenType::identifier==theToken.type) {
      if(auto *theAttr=anEntity.getAttribute(theToken.data)) {
        anOperand.ttype=theToken.type;
        anOperand.name=theToken.data; //hang on to name...
//        anOperand.entityId=Entity::hashString(theToken.data);

        anOperand.dtype=theAttr->getType();
      }
      else {
        anOperand.ttype=TokenType::string;
        anOperand.dtype=DataTypes::varchar_type;
        anOperand.value=theToken.data;
      }
    }
    else if(TokenType::number==theToken.type) {
      anOperand.ttype=TokenType::number;
      anOperand.dtype=DataTypes::int_type;
      if (theToken.data.find('.')!=std::string::npos) {
        anOperand.dtype=DataTypes::float_type;
        anOperand.value=std::stof(theToken.data);
      }
      else anOperand.value=std::stoi(theToken.data);
    }
    else theResult.error=syntaxError;
    if(theResult) aTokenizer.next();
    return theResult;
  }
    
  //STUDENT: Add validation here...
  bool validateOperands(Operand &aLHS, Operand &aRHS, Entity &anEntity) {
    if(TokenType::identifier==aLHS.ttype) { //most common case...
      //STUDENT: Add code for validation as necessary
        if(anEntity.getAttribute(aLHS.name) && aLHS.dtype == aRHS.dtype){
            return true;
        }
    }
    else if(TokenType::identifier==aRHS.ttype) {
      //STUDENT: Add code for validation as necessary
        if(anEntity.getAttribute(aRHS.name) && aLHS.dtype == aRHS.dtype){
            return true;
        }
    }
    return false;
  }

  //STUDENT: This starting point code may need adaptation...
  StatusResult Filters::parse(Tokenizer &aTokenizer,Entity &anEntity) {
    StatusResult  theResult{noError};

    while(theResult && (2<aTokenizer.remaining())
    && aTokenizer.current().keyword != Keywords::limit_kw
    && aTokenizer.current().keyword != Keywords::order_kw) {
      Operand theLHS,theRHS;
      Token &theToken=aTokenizer.current();
      if(theToken.type!=TokenType::identifier) return theResult;
      if((theResult=parseOperand(aTokenizer,anEntity,theLHS))) {
        Token &theToken=aTokenizer.current();
        if(theToken.type==TokenType::operators) {
          Operators theOp=Helpers::toOperator(theToken.data);
          aTokenizer.next();
          if((theResult=parseOperand(aTokenizer,anEntity,theRHS))) {
            if(validateOperands(theLHS, theRHS, anEntity)) {
              add(new Expression(theLHS, theOp, theRHS));
              if(aTokenizer.skipIf(semicolon) /*|| aTokenizer.current().keyword == Keywords::limit_kw ||aTokenizer.current().keyword == Keywords::order_kw*/) {
                  break;
              }
            }
            else theResult.error=syntaxError;
          }
        }
      }
      else theResult.error=syntaxError;
    }
    return theResult;
  }

}

