//
//  BlockIO.cpp
//  RGAssignment2
//
//  Created by rick gessner on 2/27/21.
//

#include "BlockIO.hpp"
#include <cstring>

namespace ECE141 {

  Block::Block(BlockType aType) {
    header = BlockHeader(aType);
  }
  Block::Block(const Block &aCopy) {
    *this=aCopy;
  }

  Block& Block::operator=(const Block &aCopy) {
    std::memcpy(payload, aCopy.payload, kPayloadSize);
    header=aCopy.header;
    return *this;
  }

  StatusResult Block::write(std::ostream &aStream) {
    aStream.write((char*)&header, sizeof(BlockHeader));
    aStream.write(payload,kPayloadSize);

    return StatusResult{Errors::noError};
  }

  //---------------------------------------------------

  BlockIO::BlockIO(std::iostream &aStream) : stream(aStream), theCache(LRUCache<size_t, Block> (Config::getCacheSize(CacheType::block))) {
    
  }

  // USE: write data a given block (after seek) ---------------------------------------
  StatusResult BlockIO::writeBlock(uint32_t aBlockNumber, Block &aBlock) {
    
    if (stream){
      stream.seekp(aBlockNumber*kBlockSize);
      theCache.put(aBlockNumber, aBlock);
      return aBlock.write(stream);
      

    } else {
      return StatusResult{writeError};
    }
  }

  StatusResult BlockIO::readBlock(uint32_t aBlockNumber, Block &aBlock) {
    if (getBlockCount()<=aBlockNumber) return StatusResult{readError};
    if(theCache.contains(aBlockNumber)){
      theCache.get(aBlockNumber, aBlock);
    } 
    else{
      stream.seekg (aBlockNumber*kBlockSize, stream.beg);
      stream.read((char*)&aBlock.header, sizeof(BlockHeader));
      stream.get(aBlock.payload, kPayloadSize);
      theCache.put(aBlockNumber, aBlock);
      
    }
    return StatusResult{noError};
  }

  // USE: count blocks in file ---------------------------------------
  uint32_t BlockIO::getBlockCount()  {
    stream.seekg(0, stream.end);
    return stream.tellg()/kBlockSize; //What should this be?
  }


}
