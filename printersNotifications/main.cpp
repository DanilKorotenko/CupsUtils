//
//  main.cpp
//  printersNotifications
//
//  Created by Danil Korotenko on 3/30/21.
//

#include <iostream>
#include <cups/cups.h>

void print_attributes(cups_lang_t *lang, ipp_t *ipp);


int main(int argc, const char * argv[])
{
    std::cout << "Hello, Printer notifications!\n";

    ipp_t *event = NULL;			/* Event from scheduler */

    setbuf(stderr, NULL);

    signal(SIGPIPE, SIG_IGN);

    cups_lang_t *lang = cupsLangDefault(); /* Language info */
    if (lang == NULL)
    {
        return (1);
    }


    for (;;)
    {
        event = ippNew();

        ipp_state_t	state;			/* IPP event state */

        while ((state = ippReadFile(0, event)) != IPP_DATA)
        {
            if (state <= IPP_IDLE)
            {
                break;
            }
        }

        std::cout << "state= "<< state << std::endl;

        if (state == IPP_ERROR)
        {
            std::cout << "ippReadFile() returned IPP_ERROR!" << std::endl;
        }

        if (state <= IPP_IDLE)
        {
            ippDelete(event);
            return (0);
        }

        char *subject = cupsNotifySubject(lang, event);
        char *text    = cupsNotifyText(lang, event);

        std::cout << "subject:  " << subject << std::endl;
        std::cout << "text:     " << text  << std::endl;

        ippDelete(event);
    }
}
