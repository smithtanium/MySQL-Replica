#ifndef DBView_h
#define DBView_h


#include "View.hpp"
#include "Config.hpp"
#include <iomanip>

namespace ECE141 {
    class DBView : public View {
        public:
            DBView(int aNum) { num = aNum;}
            virtual bool show(std::ostream &anOutput) {
                anOutput<<"Query OK, "<<num<< " rows affected ("<< std::fixed << Config::getTimer().elapsed()<< " secs)"<<std::endl;
                return true;
            }
        protected:
            int num;
    };
}
#endif
