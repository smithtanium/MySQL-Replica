//
// Created by smith on 5/2/2022.
//

#ifndef ASSIGNMENT5_GROUP11_QUERY_HPP
#define ASSIGNMENT5_GROUP11_QUERY_HPP

#include "Tokenizer.hpp"
#include "BasicTypes.hpp"
#include "Filters.hpp"


namespace ECE141 {

    class Query {
    public:
        Query(std::string &aTableName, Tokenizer &aTokenizer);

        void operator = (const Query &aQuery);

        std::string getAttributeName();

        bool isOrder();
        //getFilters(); //returns filter object
        //uint32_t limitNumber(); //returns limit by size

        std::string getTableName() {return tableName;};
        StatusResult addFilters(Tokenizer &aTokenizer, Entity *aEntity);
        Filters* getFilters() {return filter;};

        StringList getAttributeNames() {return attributeNames;};
        StatusResult setAttributeList(StringList &aList);

        size_t getLimit() {return limitTo;};
        StatusResult setLimit(size_t aValue);

        std::string getOrderBy() {return orderBy;};
        StatusResult setOrderBy(std::string &aName);

    protected:
        std::string tableName;
        StringList attributeNames;
        Filters *filter;
        size_t limitTo;
        std::string orderBy;
//        std::string attributeName;
    };
}

#endif //ASSIGNMENT5_GROUP11_QUERY_HPP
