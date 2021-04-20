//
//  main.cpp
//  releaseJob
//
//  Created by Danil Korotenko on 4/20/21.
//

#include <iostream>
#include "CupsUtils.hpp"

int main(int argc, const char * argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <job-id>" << std::endl;
        return 0;
    }

    std::string jobIdStr = argv[1];

    int jobId = std::atoi(jobIdStr.c_str());

    CupsUtilities::CupsUtils cupsUtils;

    cupsUtils.releaseJob(jobId);

    return 0;
}
