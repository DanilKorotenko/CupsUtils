//
//  cupsutils.cpp
//  cupsutils
//
//  Created by Danil Korotenko on 4/1/21.
//

#include "CupsUtils.hpp"
#include "CupsUtilsImpl.hpp"

namespace CupsUtilities
{

CupsUtils::CupsUtils()
    : _impl(new CupsUtilsImpl())
{

}

CupsUtils::~CupsUtils()
{
    _impl.reset();
}

int CupsUtils::getJobNumberOfDocuments(int aJobID)
{
    return CupsUtilsImpl::getJobNumberOfDocuments(aJobID);
}

bool CupsUtils::getDocument(int aJobID, int aDocumentNumber,
    const std::string &anOutputFileName)
{
    return CupsUtilsImpl::getDocument(aJobID, aDocumentNumber, anOutputFileName);
}

void CupsUtils::cancelJob(int aJobId)
{
    CupsUtilsImpl::cancelJob(aJobId);
}

bool CupsUtils::releaseJob(int aJobId)
{
    return CupsUtilsImpl::releaseJob(aJobId);
}

std::vector<CupsOption> CupsUtils::loadJobProperties(const std::string& jobHistFile)
{
    return CupsUtilsImpl::loadJobProperties(jobHistFile);
}

std::string CupsUtils::lastErrorString()
{
    return CupsUtilsImpl::lastErrorString();
}

std::vector<CupsPrinter> CupsUtils::getPrinters()
{
    if (_impl)
    {
        return _impl->getPrinters();
    }
    return std::vector<CupsPrinter>();
}

std::vector<CupsOption> CupsUtils::getListOfOptionsForPrinterWithName(
    std::string aPrinterName)
{
    if (_impl)
    {
        return _impl->getOptionsForPrinterWithName(aPrinterName);
    }
    std::vector<CupsOption> result;
    return result;
}

std::string CupsUtils::getOptionValueForPrinterWithName(std::string aPrinterName, std::string anOptionName)
{
    if (_impl)
    {
        return _impl->getOptionValueForPrinterWithName(aPrinterName, anOptionName);
    }
    std::string result;
    return result;
}

bool CupsUtils::setOptionForPrinterWithName(std::string aPrinterName,
    const CupsOption &anOption)
{
    if (_impl)
    {
        return _impl->setOptionForPrinterWithName(aPrinterName, anOption);
    }
    return false;
}

bool CupsUtils::checkURI(std::string anUri)
{
    if (_impl)
    {
        return _impl->checkURI(anUri);
    }
    return false;
}

std::vector<CupsJob::PtrT> CupsUtils::getActiveJobs()
{
    if (_impl)
    {
        return _impl->getActiveJobs();
    }
    std::vector<CupsJob::PtrT> result;
    return result;
}

bool CupsUtils::setJobHoldUntilDefaultIndefinite(const CupsPrinter& printer)
{
    return setOptionForPrinterWithName(printer.name,{ CupsUtilities::kOptionNameJobHoldUntilDefault,
                                                      CupsUtilities::kJobHoldUntilDefaultValueIndefinite });
}

bool CupsUtils::unsetJobHoldUntilDefaultIndefinite(const CupsPrinter& printer)
{
    return setOptionForPrinterWithName(printer.name,{ CupsUtilities::kOptionNameJobHoldUntilDefault,
                                                      CupsUtilities::kJobHoldUntilDefaultValueNoHold });
}

bool CupsUtils::setPrinterOperationPolicy(const CupsPrinter& printer, const std::string& policyName)
{
    return setOptionForPrinterWithName(printer.name, { CupsUtilities::kOptionNamePrinterOperationPolicy,
                                                       policyName });
}

} // namespace CupsUtilities
