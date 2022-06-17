
//
//  TabularView.hpp
//
//  Created by rick gessner on 4/1/22.
//  Copyright © 2022 rick gessner. All rights reserved.
//

#ifndef TabularView_h
#define TabularView_h

#include <iostream>
#include <sstream>
#include <iomanip>
#include <map>
#include <vector>
#include <string>
#include "View.hpp"
#include "Row.hpp"
#include "Config.hpp"
#include "Entity.hpp"
#include "Storage.hpp"
#include "Database.hpp"
#include "BasicTypes.hpp"
#include "Helpers.hpp"


namespace ECE141 {

    //STUDENT: Feel free to use this, or build your own...


    // USE: general tabular view (with columns)
    class TabularView : public View {
    public:

        TabularView(std::ostream &output, Entity &entity, RowCollection &rows, Query *aQuery): output(output), entity(entity), rows(rows), query(aQuery){}

        // USE: create header for tablular view...
        TabularView& showHeader() {
            output<<std::string("+");
            if (query->getAttributeNames().empty()) {
                for (const auto& theAtt:entity.getAttributes()){
                    widths.push_back(22);
                    output<<std::string(widths.back(), '-');
                }
            }
            else {
                for (const auto& theAtt:query->getAttributeNames()) {
                    widths.push_back(22);
                    output<<std::string(widths.back(), '-');
                }
            }
            output<<std::string("+");
            output << "\n";

            if (query->getAttributeNames().empty()) {
                for (const auto &theAtt:entity.getAttributes()) {
                    output << "| " << std::left << std::setw(20) << theAtt.getName();
                }
            }
            else {
                for (const auto& theAtt:query->getAttributeNames()) {
                    output << "| " << std::left << std::setw(20) << theAtt;
                }
            }
            output << " |\n";

            output<<std::string("+");
            for (auto& i: widths){
                output<<std::string(i, '-');
            }
            output<<std::string("+\n");
            return *this;
        }

        // USE: create header for tablular view...
        TabularView& showFooter() {
            output<<std::string("+");
            for (auto& i: widths){
                output<<std::string(i, '-');
            }
            output<<std::string("+\n");
            return *this;
        }

        // USE: this function shows all the fields in a row...
        TabularView& showRow(Row &aRow) {
            if (query->getAttributeNames().empty()) {
                for (auto& theAttribute: entity.getAttributes()){
                    Value theVal = aRow.getData()[theAttribute.getName()];
                    output<<std::left<<"| " <<std::setw(20)<< Helpers::to_string(theVal);
                }
            }
            else {
                for (auto& theAttribute: query->getAttributeNames()){
                    Value theVal = aRow.getData()[theAttribute];
                    output<<std::left<<"| " <<std::setw(20)<< Helpers::to_string(theVal);
                }
            }


//            for (auto& theAttribute: entity.getAttributes()){
//                Value theVal = aRow.getData()[theAttribute.getName()];
//                output<<std::left<<"| " <<std::setw(20)<< Helpers::to_string(theVal);
//            }
            output<<" |\n";
            return *this;
        }

        // USE: this is the main show() for the view, where it presents all the rows...
        bool show(std::ostream &anOutput) {
            //std::string theSeparator=getSeparator();
            showHeader();
            if (rows.size()) {
                for(auto &theRow : rows) {
                    showRow(*theRow);
                }
                showFooter();
            }
            anOutput << rows.size() << " rows in set (" << std::fixed << Config::getTimer().elapsed() << "sec.)" <<std::endl;
            anOutput.flush();
            return true;
        }

    protected:
        Entity              &entity; //if necessary?
        RowCollection       &rows;
        std::string         separator;
        std::vector<int>    widths;
        std::ostream        &output;
        Query               *query;
    };

}

#endif /* TabularView_h */
