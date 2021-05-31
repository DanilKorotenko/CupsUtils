//
//  main.cpp
//  getActiveJobs
//
//  Created by Danil Korotenko on 4/13/21.
//

#include <iostream>
#include "CupsUtils.hpp"

int main(int argc, const char * argv[])
{
    CupsUtilities::CupsUtils cupsUtils;

    std::vector<CupsUtilities::CupsJob::PtrT> activeJobs = cupsUtils.getActiveJobs();

    for (CupsUtilities::CupsJob::PtrT job: activeJobs)
    {
        std::cout << job->toString() << std::endl;
    }

    return 0;
}
