//
//  cupsutilsimpl.cpp
//  cupsutils
//
//  Created by Danil Korotenko on 4/1/21.
//

#include "CupsUtilsImpl.hpp"
#include <iostream>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstring>

namespace CupsUtilities
{


const char* kOptionNameDeviceURI =                  "device-uri";
const char* kOptionNamePrinterURI =                 "printer-uri-supported";
const char* kOptionNamePrinterOperationPolicy =     "printer-op-policy";

const char* kOptionNameJobHoldUntilDefault =        "job-hold-until-default";
const char* kJobHoldUntilDefaultValueIndefinite =   "indefinite";
const char* kJobHoldUntilDefaultValueNoHold =       "no-hold";

const char* kOptionNamePrinterState =               "printer-state";
const char* kOptionNamePrinterStateReasons =        "printer-state-reasons";
const char* kOptionNamePrinterInfo =                "printer-info";


CupsUtilsImpl::CupsUtilsImpl()
    : _destinationsData({0, nullptr })
{
    getDestinations(CUPS_PRINTER_LOCAL, CUPS_PRINTER_DISCOVERED, &_destinationsData);
}

CupsUtilsImpl::~CupsUtilsImpl()
{
    freeDestinationsData(&_destinationsData);
}

#pragma mark Public
////////////////////////////////////////////////////////////////////////////////
// Public

int CupsUtilsImpl::getJobNumberOfDocuments(int aJobID)
{
    // Job attributes we want
    static const char * const jobAttrs[] = { "number-of-documents" };

    ipp_t *request = ::ippNewRequest(IPP_GET_JOB_ATTRIBUTES);

    // create jobUri
    char jobUri[HTTP_MAX_URI];
    snprintf(jobUri, sizeof(jobUri), "ipp://localhost/jobs/%d", aJobID);

    ::ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_URI, "job-uri", nullptr, jobUri);
    ::ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_NAME, "requesting-user-name", nullptr, cupsUser());
    ::ippAddStrings(request, IPP_TAG_OPERATION, IPP_TAG_KEYWORD,
        "requested-attributes", 2, nullptr, jobAttrs);

    ipp_t *response = ::cupsDoRequest(CUPS_HTTP_DEFAULT, request, "/jobs/");

    int result = 0;
    ipp_status_t lastError = cupsLastError();
    if (lastError > IPP_OK_CONFLICT)
    {
        std::cerr << "IPP error string: " << ::ippErrorString(lastError)
                  << " Cups last error: " << cupsLastErrorString() << std::endl;
        result = 0;
    }
    else
    {
        ipp_attribute_t *attr = ippFindAttribute(response, "number-of-documents", IPP_TAG_INTEGER);
        result = ippGetInteger(attr, 0);
    }

    ippDelete(response);

    return result;
}

bool CupsUtilsImpl::getDocument(int aJobID, int aDocumentNumber,
    const std::string &anOutputFileName)
{
    bool result = true;
    ipp_t *request = ::ippNewRequest(CUPS_GET_DOCUMENT);

    // create jobUri
    char jobUri[HTTP_MAX_URI];
    snprintf(jobUri, sizeof(jobUri), "ipp://localhost/jobs/%d", aJobID);

    ::ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_CHARSET, "attributes-charset", nullptr, "utf-8");
    ::ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_LANGUAGE, "attributes-natural-language", nullptr, "en");
    ::ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_URI, "job-uri", nullptr, jobUri);
    ::ippAddInteger(request, IPP_TAG_OPERATION, IPP_TAG_INTEGER,
        "document-number", aDocumentNumber);

    int fd = open(anOutputFileName.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0600);
    if (fd > 0)
    {
        ipp_t *response = ::cupsDoIORequest(CUPS_HTTP_DEFAULT, request, "/admin/", -1, fd);

        ipp_status_t lastError = cupsLastError();
        if (lastError > IPP_OK_CONFLICT)
        {
            std::cerr << "IPP error string: " << ::ippErrorString(lastError)
                      << " Cups last error: " << cupsLastErrorString() << std::endl;
            result = false;
        }
        fchmod(fd, 0644);
        close(fd);
        ippDelete(response);
    }
    else
    {
        result = false;
    }

    return result;
}

void CupsUtilsImpl::cancelJob(int aJobId)
{
    ::cupsCancelJob(nullptr, aJobId);
}

bool CupsUtilsImpl::releaseJob(int aJobId)
{
    ipp_t *request = ::ippNewRequest(IPP_RELEASE_JOB);

    // create jobUri
    char jobUri[HTTP_MAX_URI];
    snprintf(jobUri, sizeof(jobUri), "ipp://localhost/jobs/%d", aJobId);

    ::ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_URI, "job-uri", nullptr, jobUri);
    ::ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_NAME, "requesting-user-name", nullptr, cupsUser());
    ::ippDelete(::cupsDoRequest(CUPS_HTTP_DEFAULT, request, "/jobs/"));

    ipp_status_t lastError = cupsLastError();
    if (lastError > IPP_OK_CONFLICT)
    {
        std::cerr << "IPP error string: " << ::ippErrorString(lastError)
                  << " Cups last error: " << cupsLastErrorString() << std::endl;
        return false;
    }

    return true;
}

std::string CupsUtilsImpl::lastErrorString()
{
    std::stringstream ss;
    ipp_status_t lastError = ::cupsLastError();
    ss << "lastError code: " << lastError << std::endl;
    if (lastError > IPP_OK_CONFLICT)
    {
        ss << "IPP error string: " << ippErrorString(lastError) << std::endl;
        ss << "Cups last error: " << cupsLastErrorString() << std::endl;
    }

    return ss.str();
}

std::vector<CupsPrinter> CupsUtilsImpl::getPrinters()
{
    std::vector<CupsPrinter> result;
    for (int i = 0; i < _destinationsData.numberOfDestinations; i++)
    {
        result.emplace_back(getPrinterWithDestination(&_destinationsData.destinations[i]));
    }
    return result;
}

CupsPrinter CupsUtilsImpl::getPrinterWithName(std::string aPrinterName)
{
    auto printerDestination = ::cupsGetDest(aPrinterName.c_str(), nullptr,
        _destinationsData.numberOfDestinations, _destinationsData.destinations);

    return getPrinterWithDestination(printerDestination);
}

std::string getPrinterPolicy(cups_dest_t *printerDestination)
{
    std::string retValue;
    std::string printerURI = cupsGetOption(kOptionNamePrinterURI, printerDestination->num_options,
                                           printerDestination->options);

    ipp_t* request = ::ippNewRequest(IPP_GET_PRINTER_ATTRIBUTES);
    ::ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_URI, "printer-uri", nullptr, printerURI.c_str());
    ::ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_NAME, "requesting-user-name", nullptr, cupsUser());
    ipp_t* response = ::cupsDoRequest(CUPS_HTTP_DEFAULT, request, "/admin/");

    ipp_status_t lastError = cupsLastError();
    if (lastError == IPP_OK)
    {
        ipp_attribute_t *attr = ippFindAttribute(response, "printer-op-policy", IPP_TAG_ZERO);
        if (const char* result = ippGetString(attr, 0, nullptr))
        {
            retValue.assign(result);
        }
    }
    ::ippDelete(response);

    return retValue;
}

CupsPrinter CupsUtilsImpl::getPrinterWithDestination(cups_dest_t *printerDestination)
{
    CupsPrinter printer;

    printer.name = printerDestination->name;
    printer.stateReasons = getOptionValueForPrinterWithName(
        printerDestination, kOptionNamePrinterStateReasons);
    printer.printerInfo = getOptionValueForPrinterWithName(
        printerDestination, kOptionNamePrinterInfo);
    printer.policy        = getPrinterPolicy(printerDestination);
    printer.jobsHoldUntil = getOptionValueForPrinterWithName(printerDestination, "job-hold-until");
    printer.uri           = getOptionValueForPrinterWithName(printerDestination, kOptionNamePrinterURI);

    return printer;
}

std::vector<CupsOption> CupsUtilsImpl::getOptionsForPrinterWithName(std::string aPrinterName)
{
    std::vector<CupsOption> result;

    auto printerDestination = ::cupsGetDest(aPrinterName.c_str(), nullptr,
        _destinationsData.numberOfDestinations, _destinationsData.destinations);

    for (int i = 0; i < printerDestination->num_options; i++)
    {
        auto& option = printerDestination->options[i];
        CupsOption cupsOption = { option.name, option.value };
        result.push_back(cupsOption);
    }

    return result;
}

std::string CupsUtilsImpl::getOptionValueForPrinterWithName(
    cups_dest_t *aPrinterDestination, std::string anOptionName)
{
    auto optionValue = ::cupsGetOption(anOptionName.c_str(),
        aPrinterDestination->num_options, aPrinterDestination->options);

    std::string result;
    if (optionValue != nullptr)
    {
        result = optionValue;
    }

    return result;
}

std::string CupsUtilsImpl::getOptionValueForPrinterWithName(
    std::string aPrinterName, std::string anOptionName)
{
    auto printerDestination = ::cupsGetDest(aPrinterName.c_str(), nullptr,
        _destinationsData.numberOfDestinations, _destinationsData.destinations);

    auto optionValue = ::cupsGetOption(anOptionName.c_str(),
        printerDestination->num_options, printerDestination->options);

    std::string result;
    if (optionValue != nullptr)
    {
        result = optionValue;
    }

    return result;
}

bool CupsUtilsImpl::setOptionForPrinterWithName(std::string aPrinterName,
    const CupsOption &anOption)
{
    auto printerDestination = ::cupsGetDest(printerName.c_str(), nullptr,
        _destinationsData.numberOfDestinations, _destinationsData.destinations);

    printerDestination->num_options = ::cupsRemoveOption(anOption.name.c_str(),
        printerDestination->num_options,
        &printerDestination->options);

    printerDestination->num_options = ::cupsAddOption(anOption.name.c_str(),
        anOption.value.c_str(),
        printerDestination->num_options, &printerDestination->options);

    std::string printerURI = ::cupsGetOption(kOptionNamePrinterURI,printerDestination->num_options,
        printerDestination->options);

    bool result = setPrinterOptions(printerDestination->num_options,
        printerDestination->options,
        printerURI.c_str());

    return result;
}

bool CupsUtilsImpl::checkURI(std::string anUri)
{
    char scheme[HTTP_MAX_URI];      /* Method portion of URI */
    char username[HTTP_MAX_URI];    /* Username portion of URI */
    char host[HTTP_MAX_URI];        /* Host portion of URI */
    char resource[HTTP_MAX_URI];    /* Resource portion of URI */
    int port;

    http_uri_status_t uri_status = ::httpSeparateURI(HTTP_URI_CODING_ALL,
        anUri.c_str(),
        scheme, sizeof(scheme),
        username, sizeof(username),
        host, sizeof(host), &port,
        resource, sizeof(resource));

    if (uri_status != HTTP_URI_OK)
    {
        std::cerr << "URI status: " << uri_status << std::endl;
        return false;
    }

    return true;
}

std::vector<CupsJob::PtrT> CupsUtilsImpl::getActiveJobs()
{
    std::vector<CupsJob::PtrT> result;

    cups_job_t *jobs = nullptr;
    int numJobs = ::cupsGetJobs(&jobs, nullptr, 0, CUPS_WHICHJOBS_ACTIVE);

    for (int i = 0; i < numJobs; i++)
    {
        cups_job_t cupsJob = jobs[i];

        CupsJob::PtrT job = std::shared_ptr<CupsJob>(new CupsJob());
        job->jobId = cupsJob.id;
        job->title = cupsJob.title;

        CupsPrinter printer = getPrinterWithName(cupsJob.dest);
        job->printer = printer;
        job->userName = cupsJob.user;
        job->format = cupsJob.format;
        job->size = cupsJob.size;
        job->state = cupsJob.state;

        result.push_back(job);
    }

    cupsFreeJobs(numJobs, jobs);
    return result;
}

#pragma mark Private
////////////////////////////////////////////////////////////////////////////////
// Private

int CupsUtilsImpl::destinationsCallback(CupsDestinationsData *destinations_data,
    unsigned flags, cups_dest_t *dest)
{
    if (flags & CUPS_DEST_FLAGS_REMOVED)
    {
        // Remove destination from array...

        destinations_data->numberOfDestinations =
            ::cupsRemoveDest(dest->name, dest->instance,
                destinations_data->numberOfDestinations,
                &(destinations_data->destinations));
    }
    else
    {
        // Add destination to array...

        destinations_data->numberOfDestinations =
            ::cupsCopyDest(dest, destinations_data->numberOfDestinations,
                    &(destinations_data->destinations));
    }

    return (1);
}

void CupsUtilsImpl::getDestinations(cups_ptype_t type, cups_ptype_t mask,
    CupsDestinationsData* destinationsDataOut)
{
    if (!::cupsEnumDests(CUPS_DEST_FLAGS_NONE, 5000, nullptr, type,
        mask, (cups_dest_cb_t)CupsUtilsImpl::destinationsCallback,
        aDestinationsData))
    {
        // An error occurred, free all of the destinations and
        // return...
        ::cupsFreeDests(destinationsDataOut->numberOfDestinations, destinationsDataOut->destinations);

        destinationsDataOut->numberOfDestinations = 0;
        destinationsDataOut->destinations = nullptr;

    }
}

void CupsUtilsImpl::freeDestinationsData(CupsDestinationsData *aDestinationsData)
{
    cupsFreeDests(aDestinationsData->numberOfDestinations,
        aDestinationsData->destinations);
    *aDestinationsData = {0, nullptr};
}

bool CupsUtilsImpl::setPrinterOptions(
    int numOptions,
    cups_option_t *options,
    const char *printerUri)
{
    ipp_t *request = ippNewRequest(CUPS_ADD_MODIFY_PRINTER);

    ::ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_URI, "printer-uri", nullptr, printerUri);
//    ::ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_NAME, "requesting-user-name", nullptr, cupsUser());

    cupsEncodeOptions2(request, numOptions, options, IPP_TAG_PRINTER);

    ippDelete(cupsDoRequest(CUPS_HTTP_DEFAULT, request, "/admin/"));

    ipp_status_t lastError = cupsLastError();
    if (lastError > IPP_OK_CONFLICT)
    {
        std::cerr << "IPP error string: " << ::ippErrorString(lastError)
                  << " Cups last error: " << cupsLastErrorString() << std::endl;
        return false;
    }

    return true;
}

std::vector<CupsOption> CupsUtilsImpl::loadJobProperties(const std::string& jobHistFile)
{
    std::vector<CupsOption> result;

    cups_file_t		*fp;		/* Job file */
    ipp_t           *jobAttrs;
    ipp_attribute_t	*attr;

    if ((fp = ::cupsFileOpen(jobHistFile.c_str(), "r")) == nullptr)
    {
        throw std::runtime_error("CupsFileOpen error");
    }

    if ((jobAttrs = ippNew()) == nullptr)
    {
        cupsFileClose(fp);
        throw std::runtime_error("Ran out of memory for job attributes.");
    }

    if (ippReadIO(fp, (ipp_iocb_t)cupsFileRead, 1, nullptr, jobAttrs) != IPP_DATA)
    {
        cupsFileClose(fp);
        ippDelete(jobAttrs);
        throw std::runtime_error("ippReadIO error");
    }

    cupsFileClose(fp);

    attr = ippFindAttribute(jobAttrs, "job-name", IPP_TAG_NAME);
    if (const char* value = ippGetString(attr, 0, nullptr))
    {
        result.push_back({"job-name", value});
    }

    attr = ippFindAttribute(jobAttrs, "job-originating-user-name", IPP_TAG_NAME);
    if (const char* value = ippGetString(attr, 0, nullptr))
    {
        result.push_back({"job-originating-user-name", value});
    }

    ippDelete(jobAttrs);

    return result;
}

} // namespace CupsUtilities
