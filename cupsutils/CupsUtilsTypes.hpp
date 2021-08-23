//
//  cupsutils.hpp
//  cupsutils
//
//  Created by Danil Korotenko on 4/1/21.
//

#pragma once

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
    std::string printerInfo;
//    std::string uri;
//    State       state;
    std::string stateReasons;
};

}

} // namespace CupsUtilities
