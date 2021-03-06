/*
 * decaf.c -- keep your screen on
 * author: Dylan Lom <djl@dylanlom.com>
 * compile: cc -Wall -Wextra -pedantic -o decaf decaf.c -lX11 -lXext
 *
 * Description: Disables the X11 screensaver until a relevant signal is
 * recieved, or input pipe (if any) is closed.
 */

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/extensions/dpms.h>

Display *display;
CARD16 standby, suspend, off;
int timeout, interval, prefer_blanking, allow_exposures;

void
done()
{
    if (DPMSCapable(display)) {
        DPMSSetTimeouts(display, standby, suspend, off);
    }

    XSetScreenSaver(display, timeout, interval, prefer_blanking,
                    allow_exposures);

    XFlush(display);
    XCloseDisplay(display);
    exit(0);
}

int
main(void)
{
    display = XOpenDisplay(NULL);
    if (!display) {
        perror("XOpenDisplay");
        exit(1);
    }

    if (DPMSCapable(display)) {
        DPMSGetTimeouts(display, &standby, &suspend, &off);
        DPMSSetTimeouts(display, 0, 0, 0);
    }

    XGetScreenSaver(display, &timeout, &interval, &prefer_blanking,
                    &allow_exposures);
    XSetScreenSaver(display, 0, interval, prefer_blanking, allow_exposures);

    XFlush(display);

    struct sigaction act;
    act.sa_handler = done;
    // Ignore all other signals when first interrupt recieved.
    if (sigfillset(&act.sa_mask) == -1) {
        perror("sigfillset");
        exit(1);
    }

    if (sigaction(SIGHUP, &act, NULL) == -1
            || sigaction(SIGINT, &act, NULL) == -1
            || sigaction(SIGPIPE, &act, NULL) == -1
            || sigaction(SIGQUIT, &act, NULL) == -1
            || sigaction(SIGTERM, &act, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    char c = fgetc(stdin);
    if (c != EOF) {
        putchar(c);
        char buf[1024];
        while (fgets(buf, 1024, stdin)) {
            printf("%s", buf);
        }

        done();
    } else {
        pause();
    }
}
