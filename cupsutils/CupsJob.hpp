//
//  cupsutils.hpp
//  cupsutils
//
//  Created by Danil Korotenko on 4/1/21.
//

#pragma once

#include <string>
#include <sstream>
#include "CupsUtilsTypes.hpp"

namespace CupsUtilities
{

class CupsJob
{
public:
    using PtrT = std::shared_ptr<CupsJob>;

    CupsJob();

    int         job_id;
    std::string title;
    CupsPrinter printer;
    std::string userName;
    std::string format;
    int         size; // Size in kilobytes

//      IPP_JSTATE_PENDING = 3,		/* Job is waiting to be printed */
//  IPP_JSTATE_HELD,			/* Job is held for printing */
//  IPP_JSTATE_PROCESSING,		/* Job is currently printing */
//  IPP_JSTATE_STOPPED,			/* Job has been stopped */
//  IPP_JSTATE_CANCELED,			/* Job has been canceled */
//  IPP_JSTATE_ABORTED,			/* Job has aborted due to error */
//  IPP_JSTATE_COMPLETED			/* Job has completed successfully */
    int         state;

    std::string toString() const;
};

}
