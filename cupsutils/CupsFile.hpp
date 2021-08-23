
#pragma once

#include <string>
#include <memory>

namespace CupsUtilities
{

class CupsJob;

/**
 *  Create cups file and remove one during destruction
 */
class CupsFile
{
public:
    using PtrT = std::shared_ptr<CupsFile>;

    CupsFile(const CupsJob& job, int fileId);
    ~CupsFile();

    /**
     * Get file path in spool directory /var/gtb-spool/printing/<filepath>
     * @return - file path.
     */
    const std::string& getPath() const;

    /**
     * Get the id of the job that owns this file.
     * @return - job id
     */
    int getJobId() const;

    /**
     * Get file id in job.
     * @return
     */
    int getFileId() const;

private:
    void init(const CupsJob& job);

private:
    int         _jobId;
    int         _fileId;
    std::string _filePath;
};

/**
 * Inlines
 */

inline const std::string& CupsFile::getPath() const
{
    return _filePath;
}

inline int CupsFile::getJobId() const
{
    return _jobId;
}

inline int CupsFile::getFileId() const
{
    return _fileId;
}

} // namespace CupsUtilities
