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
    CupsUtils cupsUtils;

    std::vector<CupsJob> activeJobs = cupsUtils.getActiveJobs();

    for (std::vector<CupsJob>::iterator it = activeJobs.begin();
        it != activeJobs.end(); it++)
    {
        std::cout << (*it).job_id << " : " << (*it).title.c_str() << std::endl;
    }

    return 0;
}
