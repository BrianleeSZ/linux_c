#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#ifndef WIN32
#include <netinet/in.h>
# ifdef _XOPEN_SOURCE_EXTENDED
#  include <arpa/inet.h>
# endif
#include <sys/socket.h>
#endif
#include <ctype.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>
#include <event2/event_compat.h>
#include <event.h>
     struct event ev;
     struct timeval tv;
    
     void time_cb(int fd, short event, void *argc)
     {
         printf("timer wakeup!\n");
         event_add(&ev, &tv);
     }
    
     int main()
     {
         struct event_base *base = event_base_new();
         tv.tv_sec = 10;
         tv.tv_usec = 0;
         evtimer_set(&ev, time_cb, NULL);
         event_base_set(base, &ev);
         event_add(&ev, &tv);
         event_base_dispatch(base);
     }