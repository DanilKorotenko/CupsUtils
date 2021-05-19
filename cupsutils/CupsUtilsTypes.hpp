//
//  cupsutils.hpp
//  cupsutils
//
//  Created by Danil Korotenko on 4/1/21.
//

#ifndef CupsUtilsTypes_hpp
#define CupsUtilsTypes_hpp

#include <string>
#include <sstream>

namespace CupsUtilities
{

typedef struct
{
    std::string name;
    std::string value;
} CupsOption;

struct CupsPrinter
{
//    enum class State
//    {
//        Idle = 3,
//        Processing,
//        Stopped
//    };

    std::string name;
//    State       state;
    std::string stateReasons;
};

}

#endif /* CupsUtilsTypes_hpp */
