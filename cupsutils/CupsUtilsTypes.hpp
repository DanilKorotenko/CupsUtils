//
//  cupsutils.hpp
//  cupsutils
//
//  Created by Danil Korotenko on 4/1/21.
//

#pragma once

#include <string>
#include <sstream>
#include <memory>
#include <cups/cups.h>

namespace CupsUtilities
{

typedef struct
{
    std::string name;
    std::string value;
} CupsOption;

struct CupsPrinter
{
    using PtrT = std::shared_ptr<CupsPrinter>;

    CupsPrinter() = default;
    CupsPrinter(const CupsPrinter&) = default;
    CupsPrinter(CupsPrinter&&) = default;

    CupsPrinter& operator=(const CupsPrinter&) = default;
    CupsPrinter& operator=(CupsPrinter&&) = default;

    std::string toString() const
    {
        std::stringstream result;
        result << "Printer name: "    << name << std::endl
               << "URI: "             << uri << std::endl
               << "Info: "            << printerInfo << std::endl
               << "Policy: "          << policy << std::endl
               << "Jobs hold until: " << jobsHoldUntil << std::endl
               << "State reasons: "   << stateReasons <<  std::endl;
        return result.str();
    }

    bool isModified(const CupsPrinter& printer) const
    {
        if (uri != printer.uri)
        {
            return false;
        }

        bool result = policy == printer.policy && jobsHoldUntil == printer.jobsHoldUntil;
        return !result;
    }

    bool isStateChanged(const CupsPrinter& printer) const
    {
        if (uri != printer.uri)
        {
            return false;
        }

        return stateReasons != printer.stateReasons;
    }

    std::string  name;
    std::string  printerInfo;
    std::string  policy;
    std::string  jobsHoldUntil;
    std::string  uri;
    std::string  stateReasons;
    bool         newPrinter = false; // flag to skip modification event triggered by setup DLP.
};

class CupsJob
{
public:
    using PtrT = std::shared_ptr<CupsJob>;

    CupsJob()
            : jobId(0)
            , title("untitled")
            , printer()
            , userName("unknown")
            , format("application/octet-stream")
            , size(0)
            , state(IPP_JOB_PENDING)
    {
    }

    std::string toString() const
    {
        std::stringstream result;

        result << "Job id: " << jobId << std::endl
               << "Job title: " << title << std::endl
               << "Printer name: " << printer.name << std::endl
               << "Printer info: " << printer.printerInfo << std::endl
               << "Printer uri: " << printer.uri << std::endl
               << "Printer state reasons: " << printer.stateReasons << std::endl
               << "User: " << userName << std::endl
               << "Format: " << format << std::endl
               << "Size: " << size << std::endl
               << "State: " << state << std::endl;

        return result.str();
    }

    int          jobId;
    std::string  title;
    CupsPrinter  printer;
    std::string  userName;
    std::string  format;
    int          size; // Size in kilobytes
    ipp_jstate_e state;
};

} // namespace CupsUtilities
