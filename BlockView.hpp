#ifndef BlockView_h
#define BlockView_h


#include "View.hpp"
#include "Config.hpp"
#include <iomanip>

namespace ECE141 {
    class BlockView : public View {
        public:
            BlockView(Database* aDB):db(aDB) {}
            virtual bool show(std::ostream &anOutput) {
                // uint32_t theCount = blockio->getBlockCount();
                anOutput<<"+----------------+--------+---------------+"<<std::endl;
                anOutput<<"| Type           | Id     | Extra         |"<<std::endl;
                anOutput<<"+----------------+--------+---------------+"<<std::endl;
                return db->dump(anOutput);
                //Database theDB = Database(aName, OpenDB{});
                //anOutput<<"|"<<db->dump(anOutput)<<std::endl;
                
                //anOutput<<db.blockio->getBlockCount() <<" rows in set ("<< Config::getTimer().elapsed()<< "secs)"<<std::endl;
                
            }

        protected:
            Database* db;
    };
}
#endif
