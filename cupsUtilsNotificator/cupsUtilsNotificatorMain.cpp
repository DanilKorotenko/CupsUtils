//
//  main.cpp
//  cupsUtilsNotificator
//
//  Created by Danil Korotenko on 4/12/21.
//

#include <cups/cups.h>
#include <sys/stat.h>
#include <cups/language.h>
#include <sys/select.h>
#include <stdio.h>
#include <errno.h>

typedef struct _NotificatorMessage
{
    int sequence_number;    /* notify-sequence-number */
    char *subject;          /* Message subject/summary */
    char *text;             /* Message text */
    char *link_url;         /* Link to printer */
    time_t event_time;      /* When the event occurred */
} NotificatorMessage;



///*
// * Local functions...
// */
//static int		compare_rss(_cups_rss_t *a, _cups_rss_t *b);
//static void		delete_message(_cups_rss_t *rss);
//static void		load_rss(cups_array_t *rss, const char *filename);

static NotificatorMessage *newMessage(int sequence_number, const char *subject,
    const char *text, const char *link_url, time_t event_time);

//static const char	*password_cb(const char *prompt);
//static int		save_rss(cups_array_t *rss, const char *filename,
//			         const char *baseurl);

////////////////////////////////////////////////////////////////////////////////

int main(int  argc, char *argv[])
{
//  int		i;			/* Looping var */


    ipp_t       *event;			/* Event from scheduler */
    ipp_state_t state;			/* IPP event state */


//  char		scheme[32],		/* URI scheme ("rss") */
//		username[256],		/* Username for remote RSS */
//		host[1024],		/* Hostname for remote RSS */
//		resource[1024],		/* RSS file */
//		*options;		/* Options */
//  int		port,			/* Port number for remote RSS */
//		max_events;		/* Maximum number of events */
//  http_t	*http;			/* Connection to remote server */
//  http_status_t	status;			/* HTTP GET/PUT status code */
//  char		filename[1024],		/* Local filename */
//		newname[1024];		/* filename.N */

    cups_lang_t *language;		/* Language information */

    ipp_attribute_t *printer_up_time;           /* Timestamp on event */
    ipp_attribute_t *notify_sequence_number;    /* Sequence number */
    ipp_attribute_t *notify_printer_uri;        /* Printer URI */

    char *subject;		/* Subject for notification message */
    char *text;			/* Text for notification message */


//	char	link_url[1024],		/* Link to printer */
//	char	link_scheme[32],	/* Scheme for link */
//	char	link_username[256],	/* Username for link */
//	char	link_host[1024],	/* Host for link */
//	char	link_resource[1024];	/* Resource for link */
//  int		link_port;		/* Link port */
//  cups_array_t	*rss;			/* RSS message array */

    NotificatorMessage *notificatorMessage;

//  char		baseurl[1024];		/* Base URL */

    fd_set input;			/* Input set for select() */

    struct timeval timeout;		/* Timeout for select() */

//  int		changed;		/* Has the RSS data changed? */

    int exit_status;		/* Exit status */


    fprintf(stderr, "DEBUG: argc=%d\n", argc);
    for (int i = 0; i < argc; i ++)
    {
        fprintf(stderr, "DEBUG: argv[%d]=\"%s\"\n", i, argv[i]);
    }

    // Localize for the user's chosen language...
    language = cupsLangDefault();

    // Read events.
    for (exit_status = 0, event = NULL;;)
    {
        // Wait up to 30 seconds for an event...

        timeout.tv_sec  = 30;
        timeout.tv_usec = 0;

        FD_ZERO(&input);
        FD_SET(0, &input); // set stdin fd for waitnig

        if (select(1, &input, NULL, NULL, &timeout) < 0)
        {
            continue;
        }
        else if (!FD_ISSET(0, &input))
        {
            fprintf(stderr, "DEBUG: %s is bored, exiting...\n", argv[1]);
            break;
        }

        // Read the next event...

        event = ippNew();

        while ((state = ippReadFile(0, event)) != IPP_DATA)
        {
            if (state <= IPP_IDLE)
            {
                break;
            }
        }

        if (state == IPP_ERROR)
        {
            fputs("DEBUG: ippReadFile() returned IPP_ERROR!\n", stderr);
        }

        if (state <= IPP_IDLE)
        {
            break;
        }

        // Collect the info from the event...

        printer_up_time        = ippFindAttribute(event, "printer-up-time",
            IPP_TAG_INTEGER);
        notify_sequence_number = ippFindAttribute(event, "notify-sequence-number",
            IPP_TAG_INTEGER);
        notify_printer_uri     = ippFindAttribute(event, "notify-printer-uri",
            IPP_TAG_URI);
        subject                = cupsNotifySubject(language, event);
        text                   = cupsNotifyText(language, event);

        if (printer_up_time && notify_sequence_number && subject && text)
        {
            // Create a new message.

            int sequence_number = ippGetInteger(notify_sequence_number, 0);
            const char *printerURI = ippGetString(notify_printer_uri, 0, NULL);

            time_t event_time = ippGetInteger(printer_up_time, 0);

//static NotificatorMessage *newMessage(int sequence_number, char *subject,
//    char *text, char *link_url, time_t event_time)

            notificatorMessage = newMessage(sequence_number, subject, text,
                printerURI, event_time);

            if (!notificatorMessage)
            {
                fprintf(stderr, "ERROR: Unable to create message: %s\n",
                    strerror(errno));
                exit_status = 1;
                break;
            }

            // print it

        }

        if (subject)
        {
            free(subject);
        }

        if (text)
        {
            free(text);
        }

        ippDelete(event);
        event = NULL;
    }

    ippDelete(event);

    return (exit_status);
}

static NotificatorMessage *newMessage(int sequence_number, const char *subject,
    const char *text, const char *link_url, time_t event_time)
{
    NotificatorMessage *notificatorMessage;

    if ((notificatorMessage = (NotificatorMessage *)calloc(1, sizeof(NotificatorMessage))) == NULL)
    {
        return (NULL);
    }

    notificatorMessage->sequence_number = sequence_number;
    notificatorMessage->subject         = (char *)subject;
    notificatorMessage->text            = (char *)text;
    notificatorMessage->link_url        = (char *)link_url;
    notificatorMessage->event_time      = event_time;

    return (notificatorMessage);
}


/*
 * 'save_rss()' - Save messages to a RSS file.
 */

static int				/* O - 1 on success, 0 on failure */
save_rss(cups_array_t *rss,		/* I - RSS messages */
         const char   *filename,	/* I - File to save to */
	 const char   *baseurl)		/* I - Base URL */
{
  FILE		*fp;			/* File pointer */
  _cups_rss_t	*msg;			/* Current message */
  char		date[1024];		/* Current date */
  char		*href;			/* Escaped base URL */


  if ((fp = fopen(filename, "w")) == NULL)
  {
    fprintf(stderr, "ERROR: Unable to create %s: %s\n", filename,
            strerror(errno));
    return (0);
  }

  fchmod(fileno(fp), 0644);

  fputs("<?xml version=\"1.0\"?>\n", fp);
  fputs("<rss version=\"2.0\">\n", fp);
  fputs("  <channel>\n", fp);
  fputs("    <title>CUPS RSS Feed</title>\n", fp);

  href = xml_escape(baseurl);
  fprintf(fp, "    <link>%s</link>\n", href);
  free(href);

  fputs("    <description>CUPS RSS Feed</description>\n", fp);
  fputs("    <generator>" CUPS_SVERSION "</generator>\n", fp);
  fputs("    <ttl>1</ttl>\n", fp);

  fprintf(fp, "    <pubDate>%s</pubDate>\n",
          httpGetDateString2(time(NULL), date, sizeof(date)));

  for (msg = (_cups_rss_t *)cupsArrayLast(rss);
       msg;
       msg = (_cups_rss_t *)cupsArrayPrev(rss))
  {
    char *subject = xml_escape(msg->subject);
    char *text = xml_escape(msg->text);

    fputs("    <item>\n", fp);
    fprintf(fp, "      <title>%s</title>\n", subject);
    fprintf(fp, "      <description>%s</description>\n", text);
    if (msg->link_url)
      fprintf(fp, "      <link>%s</link>\n", msg->link_url);
    fprintf(fp, "      <pubDate>%s</pubDate>\n",
            httpGetDateString2(msg->event_time, date, sizeof(date)));
    fprintf(fp, "      <guid>%d</guid>\n", msg->sequence_number);
    fputs("    </item>\n", fp);

    free(subject);
    free(text);
  }

  fputs(" </channel>\n", fp);
  fputs("</rss>\n", fp);

  return (!fclose(fp));
}
