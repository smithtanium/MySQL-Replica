
#ifndef LRUCache_h
#define LRUCache_h  

#include <list>
#include <unordered_map>

  
namespace ECE141 {  
  template<typename KeyT, typename ValueT>
  class LRUCache {
  public:
    
    LRUCache(size_t aSize){
        maxsize = aSize;
    };
    //OCF 

    void put(const KeyT &key, const ValueT& value){
        if (maxsize==0) return;
        auto it = theMap.find(key);
        if(it != theMap.end()){
            dq.erase(it->second);
            dq.push_front({key, value});
            it->second = dq.begin();
        }
        else{
            if(dq.size()==maxsize){
                theMap.erase(dq.back().first);
                dq.pop_back();
            }
            dq.push_front({key, value});
            theMap[key] = dq.begin();
        }
        

    };

    void get(const KeyT& key, ValueT& value){
        //if(theMap.find(key)== theMap.end()||maxsize==0) return;
        auto it = theMap.find(key);
        value = it->second->second;
        dq.splice(dq.begin(), dq, it->second);
    };    

  

    bool    contains(const KeyT& key) const{
        return (theMap.find(key) != theMap.end() && maxsize!=0) ;
    };

    size_t  size() const{
        return dq.size();
    }; //current size
    
  protected:
    size_t maxsize; //prevent cache from growing past this size...
    std::list<std::pair<KeyT, ValueT>>dq; 
    std::unordered_map<KeyT , typename std::list<std::pair<KeyT, ValueT>>::iterator> theMap;

    //data members here...
  };
}
#endif