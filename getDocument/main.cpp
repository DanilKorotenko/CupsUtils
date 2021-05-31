//
//  main.cpp
//  getDocument
//
//  Created by Danil Korotenko on 4/8/21.
//

#include <iostream>
#include <sstream>

#include "cupsutils.hpp"
#include "UTTypeAdapter.h"

// You should run this utility from sudo, to be able to get document from the
// printing job.

int main(int argc, const char * argv[])
{
    CupsUtilities::CupsUtils cupsUtils;

    if (argc == 4)
    {
        std::cout << "Usage: " << argv[0] << " <job-id> <document-number> <output-file-name>" << std::endl;

        std::string jobIDStr = argv[1];
        int jobID = std::atoi(jobIDStr.c_str());
        std::string documentNumberStr = argv[2];
        int documentNumber = std::atoi(documentNumberStr.c_str());
        std::string outputFileName = argv[3];

        cupsUtils.getDocument(jobID, documentNumber, outputFileName);
    }
    else
    {
        std::cout << "Get all documents from all active jobs." << std::endl;

        std::vector<CupsUtilities::CupsJob::PtrT> activeJobs = cupsUtils.getActiveJobs();

        for (CupsUtilities::CupsJob::PtrT job: activeJobs)
        {
            int numberOfDocuments = cupsUtils.getJobNumberOfDocuments(job->job_id);

            std::cout << "Job ( " << job->job_id << " ) " << job->title.c_str()
            << " documents: " << numberOfDocuments << " format: " << job->format << std::endl;

            for (int docNum = 0; docNum < numberOfDocuments; docNum++)
            {
                std::string ext = UTFileExtensionForMIMEType(job->format);

                // we must specify docNumber starting from 1
                std::stringstream docName;
                docName << job->title << "-" << (docNum + 1) << "." << ext;

                std::cout << "retriving document: " << docName.str() << std::endl;

                // we must specify docNumber starting from 1
                cupsUtils.getDocument(job->job_id, (docNum + 1), docName.str());
            }
        }
    }

    return 0;
}
