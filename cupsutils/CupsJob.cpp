//
//  cupsutils.hpp
//  cupsutils
//
//  Created by Danil Korotenko on 4/1/21.
//

#include "CupsJob.hpp"

namespace CupsUtilities
{

    CupsJob::CupsJob()
        : job_id(0)
        , title("untitled")
        , printer()
        , userName(unknown)
        , format("application/octet-stream")
        , size(0)
        , state(3)
    {
    }

    std::string CupsJob::toString() const
    {
        std::stringstream result;

        result << "{ \n\t"
            << "job_id: " << job_id << ", \n\t"
            << "title: " << title << ", \n\t"
            << "printer.name: " << printer.name << ", \n\t"
            << "printer.stateReasons: " << printer.stateReasons << ", \n\t"
            << "userName: " << userName << ", \n\t"
            << "format: " << format << ", \n\t"
            << "size: " << size << ", \n\t"
            << "state: " << state << "\n\t"
            << " }" << std::endl;

        return result.str();
    }

}
