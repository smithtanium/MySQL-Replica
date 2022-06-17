//
//  BlockIO.hpp
//  RGAssignment2
//
//  Created by rick gessner on 2/27/21.
//

#ifndef BlockIO_hpp
#define BlockIO_hpp

#include <stdio.h>
#include <iostream>
#include <functional>
#include "Errors.hpp"
#include "LRUCache.hpp"
#include "Config.hpp"


namespace ECE141 {
    enum class BlockType {
        data_block='D',
        free_block='F',
        unknown_block='U',
        meta_block ='M',
        entity_block='E',
        index_block='I'
    };

    //a small header that describes the block...
    struct BlockHeader {

        BlockHeader(BlockType aType=BlockType::data_block)
                : type(static_cast<char>(aType)) {}

        BlockHeader(const BlockHeader &aCopy) {
            *this=aCopy;
        }
        void empty() {
            type=static_cast<char>(BlockType::free_block);
        }

        BlockHeader& operator=(const BlockHeader &aCopy) {
            next=aCopy.next;
            count=aCopy.count;
            pos=aCopy.pos;
            entity=aCopy.entity;
            type=aCopy.type;
            return *this;
        }

        char      type;
        uint32_t next;
        size_t count;
        size_t pos;    //char version of block type
        uint32_t entity;

        //other properties?
    };

    const size_t kBlockSize = 1024;
    const size_t kPayloadSize = kBlockSize - sizeof(BlockHeader);

    //block .................
    class Block {
    public:
        Block(BlockType aType=BlockType::data_block);
        Block(const Block &aCopy);

        Block& operator=(const Block &aCopy);

        StatusResult write(std::ostream &aStream);

        BlockHeader   header;
        char          payload[kPayloadSize];
    };

    //------------------------------

    class BlockIO {
    public:

        BlockIO(std::iostream &aStream);

        uint32_t              getBlockCount();

        virtual StatusResult  readBlock(uint32_t aBlockNumber, Block &aBlock);
        virtual StatusResult  writeBlock(uint32_t aBlockNumber, Block &aBlock);


    protected:
        std::iostream &stream;
        LRUCache<size_t, Block> theCache;    
    };
}

#endif /* BlockIO_hpp */
