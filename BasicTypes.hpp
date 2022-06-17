//
//  BasicTypes.hpp
//  RGAssignement1
//
//  Created by rick gessner on 3/9/21.
//

#ifndef BasicTypes_h
#define BasicTypes_h
#include <iostream>
#include <string>
#include <variant>
#include <map>
#include "BasicTypes.hpp"

namespace ECE141 {

    enum class DataTypes {
        no_type='N',  bool_type='B', datetime_type='D',
        float_type='F', int_type='I', varchar_type='V',
    };

    static std::ostream &operator<<(std::ostream &aOS, const DataTypes &aDataType) {
        aOS << (char) aDataType;
        return aOS;
    }

    static std::istream &operator>>(std::istream &aIS, DataTypes &aDataType) {
        char a;
        aIS >> a;
        aDataType = (DataTypes) a;
        return aIS;
    }

    using StringList = std::vector<std::string>;
    using StringMap = std::map<std::string, std::string>;
    using StringOpt = std::optional<std::string>;
    using IntOpt = std::optional<uint32_t>;
    using Value = std::variant<bool, int, double, std::string>;
    using KeyValues = std::map<const std::string, Value>;
    using IndexKey = std::variant<uint32_t, std::string>;
}
#endif /* BasicTypes_h */
