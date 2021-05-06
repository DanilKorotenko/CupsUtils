//
//  cupsutils.hpp
//  cupsutils
//
//  Created by Danil Korotenko on 4/1/21.
//

#ifndef CupsUtilsTypes_hpp
#define CupsUtilsTypes_hpp

#include <string>

namespace CupsUtilities
{

typedef struct
{
    std::string name;
    std::string value;
} CupsOption;

typedef struct
{
    int job_id;
    std::string title;
    std::string destinationName;
    std::string userName;
    std::string format;
} CupsJob;

}

#endif /* CupsUtilsTypes_hpp */
