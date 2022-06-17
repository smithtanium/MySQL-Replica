//
//  AboutUs.hpp
//  About your team members...
//
//  Created by rick gessner on 3/23/22.
//

#ifndef AboutMe_hpp
#define AboutMe_hpp

#include <stdio.h>
#include <string>
#include <optional>
#include <vector>

namespace ECE141 {
  
  using StringOpt = std::optional<std::string>;
  
  class AboutUs {
  protected:
      std::vector<std::string> names;
      std::vector<std::string> githubId;
  public:
    AboutUs() {
        names.emplace_back("Joshua Smith");
        names.emplace_back("Juo-Yun Chen");
        githubId.emplace_back("jas003ucsd");
        githubId.emplace_back("jychen36");
    }
    ~AboutUs()=default;
    
    size_t getTeamSize() const {return names.size();} //STUDENT: update this...
    
    StringOpt getName(size_t anIndex) const {
        //return name of student anIndex N (or nullopt)
        if (anIndex < getTeamSize()) {
            return names.at(anIndex);
        }
        else{
            return std::nullopt;
        }

    }
    StringOpt getGithubUsername(size_t anIndex) const {
        //return github username of student anIndex N (or nullopt)
        if (anIndex < getTeamSize()) {
            return githubId.at(anIndex);
        }
        else{
            return std::nullopt;
        }
    }

  };

}

#endif /* about_me */

