//
//  main.cpp
//  getDocument
//
//  Created by Danil Korotenko on 4/8/21.
//

#include <iostream>

#include "cupsutils.hpp"

// You should run this utility from sudo, to be able to get document from the
// printing job.

int main(int argc, const char * argv[])
{
    if (argc != 5)
    {
        std::cout << "Usage: " << argv[0] << " <printer-uri> <job-id> <document-number> <output-file-name>" << std::endl;
        return 0;
    }

    std::string printerURI = argv[1];
    std::string jobIDStr = argv[2];
    std::string documentNumberStr = argv[3];
    std::string outputFileName = argv[4];

    CupsUtilities::CupsUtils cupsUtils;

    cupsUtils.getDocument(printerURI, jobIDStr, documentNumberStr, outputFileName);

    return 0;
}
