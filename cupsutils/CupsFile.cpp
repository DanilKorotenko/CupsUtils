#include "CupsFile.hpp"
#include "CupsUtils.hpp"
#include "useful/utils.hpp"

#include <boost/filesystem.hpp>

namespace CupsUtilities
{

namespace
{
    const std::string printWorkDir = STRINGIZE_VALUE_OF(PRN_WORK_DIR);
}

boost::filesystem::path getWorkDir()
{
    boost::filesystem::path dstFolder(printWorkDir);

    if (!boost::filesystem::exists(dstFolder))
    {
        boost::filesystem::create_directories(dstFolder);
    }

    return dstFolder;
}

CupsFile::CupsFile(const CupsJob& job, int fileId):
         _jobId(job.jobId)
        , _fileId(fileId)
        , _filePath()
{
    init(job);
}

CupsFile::~CupsFile()
{
    boost::system::error_code ec;
    boost::filesystem::remove(_filePath, ec);
}

void CupsFile::init(const CupsJob& job)
{
    std::string docName = std::to_string(_jobId) + "-" + job.title + "-" + std::to_string(_fileId);
    //TODO: add extension to name.
    std::string ext;
//            std::string ext = UTFileExtensionForMIMEType(aPrintJob->format);

    boost::filesystem::path documentPath = getWorkDir() / docName;
    // we must specify docNumber starting from 1
    if (!CupsUtilities::CupsUtils::getDocument(_jobId, _fileId, documentPath.c_str()))
    {
        throw std::runtime_error("Can't receive document: " + docName);
    }

    _filePath = documentPath.string();
}

} // namespace CupsUtilities
