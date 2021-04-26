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

const char *kDeviceURIOptionName = "device-uri";
const char *kPrinterURIOptionName = "printer-uri-supported";
const char *kPrinterOperationPolicyOptionName = "printer-op-policy";
const char *kJobHoldUntilDefaultOptionName = "job-hold-until-default";
const char *kJobHoldUntilDefaultOptionValueIndefinite = "indefinite";

CupsUtils::CupsUtils()
    : _impl(new CupsUtilsImpl())
{

}

CupsUtils::~CupsUtils()
{
    _impl.reset();
}

std::vector<std::string> CupsUtils::getPrintersNames()
{
    if (_impl)
    {
        return _impl->getPrintersNames();
    }
    std::vector<std::string> result;
    return result;
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

bool CupsUtils::getDocument(
    const std::string &aPrinterURI,
    const std::string &aJobIDStr,
    const std::string &aDocumentNumberStr,
    const std::string &anOutputFileName)
{
    if (_impl)
    {
        return _impl->getDocument(aPrinterURI, aJobIDStr, aDocumentNumberStr,
            anOutputFileName);
    }
    return false;
}

std::vector<CupsJob> CupsUtils::getActiveJobs()
{
    if (_impl)
    {
        return _impl->getActiveJobs();
    }
    std::vector<CupsJob> result;
    return result;
}

void CupsUtils::cancelJob(int aJobId)
{
    if (_impl)
    {
        _impl->cancelJob(aJobId);
    }
}

bool CupsUtils::releaseJob(int aJobId)
{
    if (_impl)
    {
        _impl->releaseJob(aJobId);
    }
    return false;
}

}
