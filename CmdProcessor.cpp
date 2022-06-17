//
//  CommandProcessor.cpp
//  ECEDatabase
//
//  Created by rick gessner on 3/30/18.
//  Copyright © 2018 rick gessner. All rights reserved.
//

#include <memory>
#include "CmdProcessor.hpp"


namespace ECE141 {
    
  CmdProcessor::CmdProcessor(std::ostream &anOutput)
    : output(anOutput){
  }


  CmdProcessor::~CmdProcessor() = default;

  
  CmdProcessor* CmdProcessor::recognizes(Tokenizer &aTokenizer) {
    return nullptr;
  }

    bool CmdProcessor::isKnown(Tokenizer &aTokenizer) {
        return false;
    }
}
