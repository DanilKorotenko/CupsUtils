//
//  main.cpp
//  cupsNotificationsListenerOSX
//
//  Created by Danil Korotenko on 4/12/21.
//

#include <iostream>
#include <notify.h>
#include <dispatch/dispatch.h>

static const char *NotifyErrorToString(uint32_t noteErr)
{
    const char *result;

    static const char * kErrors[] =
    {
        "NOTIFY_STATUS_OK",
        "NOTIFY_STATUS_INVALID_NAME",
        "NOTIFY_STATUS_INVALID_TOKEN",
        "NOTIFY_STATUS_INVALID_PORT",
        "NOTIFY_STATUS_INVALID_FILE",
        "NOTIFY_STATUS_INVALID_SIGNAL",
        "NOTIFY_STATUS_INVALID_REQUEST",
        "NOTIFY_STATUS_NOT_AUTHORIZED"
    };

    if (noteErr < (sizeof(kErrors) / sizeof(*kErrors)))
    {
        result = kErrors[noteErr];
    }
    else if (noteErr == NOTIFY_STATUS_FAILED)
    {
        result = "NOTIFY_STATUS_FAILED";
    }
    else
    {
        result = "unknown error";
    }
    return result;
}

static void PrintNotifyError(const char *operation, const char *noteName, uint32_t noteErr)
{
    fprintf(
        stderr,
        "%s: %s: %s (%u)\n",
        noteName,
        operation,
        NotifyErrorToString(noteErr),
        (unsigned int) noteErr
    );
}

static void PrintToken(
    int             token,
    size_t          noteCount,
    const int *     tokens,
    const char **   noteNames
)
    // For a given token, search the tokens array looking for a match.  If
    // you find one, print the associated notification name.  If you don't
    // find one, print a default string.
{
    size_t  noteIndex;
    bool    found;

    found = false;
    for (noteIndex = 0; noteIndex < noteCount; noteIndex++) {
        if (token == tokens[noteIndex]) {
            fprintf(stdout, "%s (%d)\n", noteNames[noteIndex], token);
            found = true;
        }
    }
    if ( ! found ) {
        fprintf(stdout, "??? (%d)\n", token);
    }
    fflush(stdout);
}

static int ListenUsingGCD(size_t noteCount, const char **noteNames)
{
    int         retVal;
    uint32_t    noteErr;
    size_t      noteIndex;
    int         noteTokens[noteCount];
    const int * noteTokensPtr;

    // We need to capture the base of the noteTokens array, but the compiler won't let us do that
    // because it's of variable size.  In our specific case this isn't a problem because, if things
    // go well, we never leave this routine but rather block forever in dispatch_main().  In a real
    // program you'd have to be a bit more careful (but then again, in a real program you wouldn't be
    // registering for an unbounded number of arbitrary notifications :-).

    noteTokensPtr = &noteTokens[0];

    noteErr = NOTIFY_STATUS_OK;

    for (noteIndex = 0; noteIndex < noteCount; noteIndex++)
    {
        noteErr = notify_register_dispatch(noteNames[noteIndex],
            &noteTokens[noteIndex], dispatch_get_main_queue(), ^(int token)
            {
                PrintToken(
                    token,
                    noteCount,
                    noteTokensPtr,
                    noteNames
                );
            });

        if (noteErr != NOTIFY_STATUS_OK)
        {
            break;
        }
    }

    if (noteErr != NOTIFY_STATUS_OK)
    {
        PrintNotifyError("registration failed", noteNames[noteIndex], noteErr);
        retVal = EXIT_FAILURE;
    }
    else
    {
        fprintf(stdout, "Listening using GCD:\n");
        fflush(stdout);
        dispatch_main();
        assert(0);              // dispatch_main() should never return
        retVal = EXIT_FAILURE;
    }
    return retVal;
}

static void PrintUsage(void)
{
    fprintf(stderr, "usage: %s <cups-notification-name> [<cups-notification-name>] ...\n", getprogname());

    fprintf(stderr, "Cups notifications avaliable (taken from cups source):\n");
    fprintf(stderr, "\tcom.apple.printerPrefsChange\n");
    fprintf(stderr, "\tcom.apple.printerListChange\n");
    fprintf(stderr, "\tcom.apple.printerHistoryChange\n");
    fprintf(stderr, "\tcom.apple.jobChange\n");
}

int main(int argc, const char * argv[])
{
    // if
    int retVal = ListenUsingGCD( ((size_t) argc) - 1, &argv[1]);

    return retVal;
}
