//
// Created by smith on 5/2/2022.
//

#include "Query.hpp"

namespace ECE141 {
    Query::Query(std::string &aTableName, Tokenizer &aTokenizer) : tableName(aTableName), filter(nullptr), limitTo(SIZE_MAX) {
    }

    void Query::operator = (const Query &aQuery) {
        tableName = aQuery.tableName;
        attributeNames = aQuery.attributeNames;
//        filter = aQuery.filter;
    }


//    std::string Query::getAttributeName() {
//        return attributeName;
//    }

    bool Query::isOrder(){
        return orderBy != "";
    }

    StatusResult Query::addFilters(Tokenizer &aTokenizer, Entity *aEntity) {
        StatusResult aStatus{noError};
        auto *aFilter = new Filters();
        aStatus = aFilter->parse(aTokenizer, *aEntity);
        filter = aFilter;
        return aStatus;
    }

    StatusResult Query::setOrderBy(std::string &aName) {
        orderBy = aName;
        return StatusResult(noError);
    }

    StatusResult Query::setLimit(size_t aValue) {
        limitTo = aValue;
        return StatusResult(noError);
    }

    StatusResult Query::setAttributeList(StringList &aList) {
        attributeNames = aList;
        return StatusResult(noError);
    }
}
