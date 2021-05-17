//
//  main.cpp
//  cancelJob
//
//  Created by Danil Korotenko on 5/17/21.
//

#include <iostream>
#include "CupsUtils.hpp"

int main(int argc, const char * argv[])
{
    if (argc > 2)
    {
        std::cout << "Cancel single job: " << argv[0] << " <job-id>" << std::endl;
        std::cout << "Cancel all jobs: " << argv[0] << std::endl;
        return 0;
    }
    else if (argc == 2)
    {
        std::string jobIdStr = argv[1];

        int jobId = std::atoi(jobIdStr.c_str());

        CupsUtilities::CupsUtils cupsUtils;
        cupsUtils.cancelJob(jobId);
    }
    else if (argc == 1)
    {
        CupsUtilities::CupsUtils cupsUtils;
        cupsUtils.cancelAllJobs();
    }

    return 0;
}
