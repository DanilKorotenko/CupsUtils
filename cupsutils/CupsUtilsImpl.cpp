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

    printerDestination->num_options = cupsAddOption(anOption.name.c_str(),
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

int			/* O - 1 on match, 0 otherwise */
_cups_isupper(int ch)			/* I - Character to test */
{
  return (ch >= 'A' && ch <= 'Z');
}

int			/* O - Converted character */
_cups_tolower(int ch)			/* I - Character to convert */
{
  return (_cups_isupper(ch) ? ch - 'A' + 'a' : ch);
}

/*
 * '_cups_strcasecmp()' - Do a case-insensitive comparison.
 */

int				/* O - Result of comparison (-1, 0, or 1) */
_cups_strcasecmp(const char *s,	/* I - First string */
                 const char *t)	/* I - Second string */
{
  while (*s != '\0' && *t != '\0')
  {
    if (_cups_tolower(*s) < _cups_tolower(*t))
      return (-1);
    else if (_cups_tolower(*s) > _cups_tolower(*t))
      return (1);

    s ++;
    t ++;
  }

  if (*s == '\0' && *t == '\0')
    return (0);
  else if (*s != '\0')
    return (1);
  else
    return (-1);
}

/*
 * 'http_copy_decode()' - Copy and decode a URI.
 */

static const char *			/* O - New source pointer or NULL on error */
http_copy_decode(char       *dst,	/* O - Destination buffer */
                 const char *src,	/* I - Source pointer */
		 int        dstsize,	/* I - Destination size */
		 const char *term,	/* I - Terminating characters */
		 int        decode)	/* I - Decode %-encoded values */
{
  char	*ptr,				/* Pointer into buffer */
	*end;				/* End of buffer */
  int	quoted;				/* Quoted character */


 /*
  * Copy the src to the destination until we hit a terminating character
  * or the end of the string.
  */

  for (ptr = dst, end = dst + dstsize - 1;
       *src && (!term || !strchr(term, *src));
       src ++)
    if (ptr < end)
    {
      if (*src == '%' && decode)
      {
        if (isxdigit(src[1] & 255) && isxdigit(src[2] & 255))
	{
	 /*
	  * Grab a hex-encoded character...
	  */

          src ++;
	  if (isalpha(*src))
	    quoted = (tolower(*src) - 'a' + 10) << 4;
	  else
	    quoted = (*src - '0') << 4;

          src ++;
	  if (isalpha(*src))
	    quoted |= tolower(*src) - 'a' + 10;
	  else
	    quoted |= *src - '0';

          *ptr++ = (char)quoted;
	}
	else
	{
	 /*
	  * Bad hex-encoded character...
	  */

	  *ptr = '\0';
	  return (NULL);
	}
      }
      else if ((*src & 255) <= 0x20 || (*src & 255) >= 0x7f)
      {
        *ptr = '\0';
        return (NULL);
      }
      else
	*ptr++ = *src;
    }

  *ptr = '\0';

  return (src);
}

http_uri_status_t			/* O - Result of separation */
myHttpSeparateURI(
    http_uri_coding_t decoding,		/* I - Decoding flags */
    const char        *uri,		/* I - Universal Resource Identifier */
    char              *scheme,		/* O - Scheme (http, https, etc.) */
    int               schemelen,	/* I - Size of scheme buffer */
    char              *username,	/* O - Username */
    int               usernamelen,	/* I - Size of username buffer */
    char              *host,		/* O - Hostname */
    int               hostlen,		/* I - Size of hostname buffer */
    int               *port,		/* O - Port number to use */
    char              *resource,	/* O - Resource/filename */
    int               resourcelen)	/* I - Size of resource buffer */
{
  char			*ptr,		/* Pointer into string... */
			*end;		/* End of string */
  const char		*sep;		/* Separator character */
  http_uri_status_t	status;		/* Result of separation */


 /*
  * Initialize everything to blank...
  */

  if (scheme && schemelen > 0)
    *scheme = '\0';

  if (username && usernamelen > 0)
    *username = '\0';

  if (host && hostlen > 0)
    *host = '\0';

  if (port)
    *port = 0;

  if (resource && resourcelen > 0)
    *resource = '\0';

 /*
  * Range check input...
  */

  if (!uri || !port || !scheme || schemelen <= 0 || !username ||
      usernamelen <= 0 || !host || hostlen <= 0 || !resource ||
      resourcelen <= 0)
    return (HTTP_URI_STATUS_BAD_ARGUMENTS);

  if (!*uri)
    return (HTTP_URI_STATUS_BAD_URI);

 /*
  * Grab the scheme portion of the URI...
  */

  status = HTTP_URI_STATUS_OK;

  if (!strncmp(uri, "//", 2))
  {
   /*
    * Workaround for HP IPP client bug...
    */

    strlcpy(scheme, "ipp", (size_t)schemelen);
    status = HTTP_URI_STATUS_MISSING_SCHEME;
  }
  else if (*uri == '/')
  {
   /*
    * Filename...
    */

    strlcpy(scheme, "file", (size_t)schemelen);
    status = HTTP_URI_STATUS_MISSING_SCHEME;
  }
  else
  {
   /*
    * Standard URI with scheme...
    */

    for (ptr = scheme, end = scheme + schemelen - 1;
         *uri && *uri != ':' && ptr < end;)
      if (strchr("ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                 "abcdefghijklmnopqrstuvwxyz"
		 "0123456789-+.", *uri) != NULL)
        *ptr++ = *uri++;
      else
        break;

    *ptr = '\0';

    if (*uri != ':' || *scheme == '.' || !*scheme)
    {
      *scheme = '\0';
      return (HTTP_URI_STATUS_BAD_SCHEME);
    }

    uri ++;
  }

 /*
  * Set the default port number...
  */

  if (!strcmp(scheme, "http"))
    *port = 80;
  else if (!strcmp(scheme, "https"))
    *port = 443;
  else if (!strcmp(scheme, "ipp") || !strcmp(scheme, "ipps"))
    *port = 631;
  else if (!_cups_strcasecmp(scheme, "lpd"))
    *port = 515;
  else if (!strcmp(scheme, "socket"))	/* Not yet registered with IANA... */
    *port = 9100;
  else if (strcmp(scheme, "file") && strcmp(scheme, "mailto") && strcmp(scheme, "tel"))
    status = HTTP_URI_STATUS_UNKNOWN_SCHEME;

 /*
  * Now see if we have a hostname...
  */

  if (!strncmp(uri, "//", 2))
  {
   /*
    * Yes, extract it...
    */

    uri += 2;

   /*
    * Grab the username, if any...
    */

    if ((sep = strpbrk(uri, "@/")) != NULL && *sep == '@')
    {
     /*
      * Get a username:password combo...
      */

      uri = http_copy_decode(username, uri, usernamelen, "@",
                             decoding & HTTP_URI_CODING_USERNAME);

      if (!uri)
      {
        *username = '\0';
        return (HTTP_URI_STATUS_BAD_USERNAME);
      }

      uri ++;
    }

   /*
    * Then the hostname/IP address...
    */

    if (*uri == '[')
    {
     /*
      * Grab IPv6 address...
      */

      uri ++;
      if (*uri == 'v')
      {
       /*
        * Skip IPvFuture ("vXXXX.") prefix...
        */

        uri ++;

        while (isxdigit(*uri & 255))
          uri ++;

        if (*uri != '.')
        {
	  *host = '\0';
	  return (HTTP_URI_STATUS_BAD_HOSTNAME);
        }

        uri ++;
      }

      uri = http_copy_decode(host, uri, hostlen, "]",
                             decoding & HTTP_URI_CODING_HOSTNAME);

      if (!uri)
      {
        *host = '\0';
        return (HTTP_URI_STATUS_BAD_HOSTNAME);
      }

     /*
      * Validate value...
      */

      if (*uri != ']')
      {
        *host = '\0';
        return (HTTP_URI_STATUS_BAD_HOSTNAME);
      }

      uri ++;

      for (ptr = host; *ptr; ptr ++)
        if (*ptr == '+')
	{
	 /*
	  * Convert zone separator to % and stop here...
	  */

	  *ptr = '%';
	  break;
	}
	else if (*ptr == '%')
	{
	 /*
	  * Stop at zone separator (RFC 6874)
	  */

	  break;
	}
	else if (*ptr != ':' && *ptr != '.' && !isxdigit(*ptr & 255))
	{
	  *host = '\0';
	  return (HTTP_URI_STATUS_BAD_HOSTNAME);
	}
    }
    else
    {
     /*
      * Validate the hostname or IPv4 address first...
      */

      for (ptr = (char *)uri; *ptr; ptr ++)
        if (strchr(":?/", *ptr))
	  break;
        else if (!strchr("abcdefghijklmnopqrstuvwxyz"	/* unreserved */
			 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"	/* unreserved */
			 "0123456789"			/* unreserved */
	        	 "-._~"				/* unreserved */
			 "%"				/* pct-encoded */
			 "!$&'()*+,;="			/* sub-delims */
			 "\\", *ptr))			/* SMB domain */
	{
	  *host = '\0';
	  return (HTTP_URI_STATUS_BAD_HOSTNAME);
	}

     /*
      * Then copy the hostname or IPv4 address to the buffer...
      */

      uri = http_copy_decode(host, uri, hostlen, ":?/",
                             decoding & HTTP_URI_CODING_HOSTNAME);

      if (!uri)
      {
        *host = '\0';
        return (HTTP_URI_STATUS_BAD_HOSTNAME);
      }
    }

   /*
    * Validate hostname for file scheme - only empty and localhost are
    * acceptable.
    */

    if (!strcmp(scheme, "file") && strcmp(host, "localhost") && host[0])
    {
      *host = '\0';
      return (HTTP_URI_STATUS_BAD_HOSTNAME);
    }

   /*
    * See if we have a port number...
    */

    if (*uri == ':')
    {
     /*
      * Yes, collect the port number...
      */

      if (!isdigit(uri[1] & 255))
      {
        *port = 0;
        return (HTTP_URI_STATUS_BAD_PORT);
      }

      *port = (int)strtol(uri + 1, (char **)&uri, 10);

      if (*port <= 0 || *port > 65535)
      {
        *port = 0;
        return (HTTP_URI_STATUS_BAD_PORT);
      }

      if (*uri != '/' && *uri)
      {
        *port = 0;
        return (HTTP_URI_STATUS_BAD_PORT);
      }
    }
  }

 /*
  * The remaining portion is the resource string...
  */

  if (*uri == '?' || !*uri)
  {
   /*
    * Hostname but no path...
    */

    status    = HTTP_URI_STATUS_MISSING_RESOURCE;
    *resource = '/';

   /*
    * Copy any query string...
    */

    if (*uri == '?')
      uri = http_copy_decode(resource + 1, uri, resourcelen - 1, NULL,
                             decoding & HTTP_URI_CODING_QUERY);
    else
      resource[1] = '\0';
  }
  else
  {
    uri = http_copy_decode(resource, uri, resourcelen, "?",
                           decoding & HTTP_URI_CODING_RESOURCE);

    if (uri && *uri == '?')
    {
     /*
      * Concatenate any query string...
      */

      char *resptr = resource + strlen(resource);

      uri = http_copy_decode(resptr, uri,
                             resourcelen - (int)(resptr - resource), NULL,
                             decoding & HTTP_URI_CODING_QUERY);
    }
  }

  if (!uri)
  {
    *resource = '\0';
    return (HTTP_URI_STATUS_BAD_RESOURCE);
  }

 /*
  * Return the URI separation status...
  */

  return (status);
}

} // namespace CupsUtilities
