//
//  FolderView.hpp
//  Assignment2
//
//  Created by rick gessner on 2/15/21.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#ifndef FolderView_h
#define FolderView_h

#include "FolderReader.hpp"
#include "View.hpp"
#include <iomanip>

namespace ECE141 {

  // USE: provide view class that lists db files in storage path...
  class FolderView : public View {
  public:
    FolderView(const std::string aPath, const char *anExtension="db")
      : reader(aPath), extension(anExtension) {}
   
    virtual bool show(std::ostream &anOutput) {
      anOutput<<"+--------------------+"<<std::endl;
      anOutput<<"| Database           |"<<std::endl;
      anOutput<<"+--------------------+"<<std::endl;
      size_t theCount{0};
      reader.each(".db",[&](const std::string &aName ){ 
        anOutput<<"| "<<std::left<<std::setw(19)<<aName<<"|"<<std::endl;
        theCount++;
        return true;
      });
      anOutput<<"+--------------------+"<<std::endl;
      anOutput<<theCount<< " rows in set ("<< std::fixed << Config::getTimer().elapsed()<< "secs)"<<std::endl;
      return true;
    }
    
    FolderReader  reader;
    const char    *extension;
    
  };

}

#endif /* FolderView_h */
