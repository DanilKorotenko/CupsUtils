//
//  main.cpp
//  helloWorldFilter
//
//  Created by Danil Korotenko on 4/2/21.
//

#include <iostream>
#include <signal.h>

// global functions
static void sigterm_handler(int signal);

// global variables
static std::atomic_bool jobCanceled(false);

int main(int argc, const char * argv[])
{
    signal(SIGPIPE, SIG_IGN);
    signal(SIGTERM, sigterm_handler);

    // log to /var/log/cups/error_log
    fprintf(stderr, "DEBUG: Hello World filter\n");

    if (argc < 6 || argc > 7)
    {
        // We don't have the correct number of arguments; write an error message
        // and return.

        fprintf(stderr, "Usage: %s job-id user title copies options [file]", argv[0]);
        return (1);
    }

// CommandLine arguments
    std::string jobID = argv[1];
    std::string userName = argv[2]; // The user printing the job
    std::string jobName = argv[3];
    std::string numberOfCopies = argv[4]; // The number of copies to print
    std::string jobOptions = argv[5]; // The options that were provided when the job was submitted

    std::string filePath;

    if (argc == 7)
    {
        filePath = argv[6]; // The file to print (first program only).
    }

    fprintf(stderr, "DEBUG: job ID: %s", jobID.c_str());
    fprintf(stderr, "DEBUG: userName: %s", userName.c_str());
    fprintf(stderr, "DEBUG: jobName: %s", jobName.c_str());
    fprintf(stderr, "DEBUG: numberOfCopies: %s", numberOfCopies.c_str());
    fprintf(stderr, "DEBUG: jobOptions: %s", jobOptions.c_str());

    if (filePath.empty())
    {
        fprintf(stderr, "DEBUG: file path is empty. STDIN contains data to print.");
    }
    else
    {
        fprintf(stderr, "DEBUG: filePath: %s", filePath.c_str());
    }

//    if (jobCanceled.load())
//    {
//        //exit imediately
//    }


    return 0;
}

/*
 * 'sigterm_handler()' - Handle 'terminate' signals that stop the backend.
 */

static void sigterm_handler(int signal)
{
    fputs("DEBUG: Got SIGTERM. Job canceled.\n", stderr);

    jobCanceled.store(true);

    exit(1);
}
