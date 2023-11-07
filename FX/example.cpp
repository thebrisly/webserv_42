#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <stropts.h>
#include <poll.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define NORMAL_DATA 1
#define HIPRI_DATA 2

int poll_two_normal(int fd1,int fd2)
{
    struct pollfd poll_list[2];

    int retval;
    poll_list[0].fd = fd1;
    poll_list[1].fd = fd2;

    poll_list[0].events = POLLIN|POLLPRI;
    poll_list[1].events = POLLIN|POLLPRI;

    while(1)
    {
        retval = poll(poll_list,(unsigned long)2,-1);
        /* Retval will always be greater than 0 or -1 in this case. Since we're doing it while blocking */

        if(retval < 0)
        {
            fprintf(stderr,"Error while polling: %s\n",strerror(errno));
            return -1;
        }

        if(((poll_list[0].revents&POLLHUP) == POLLHUP) ||
           ((poll_list[0].revents&POLLERR) == POLLERR) ||
           ((poll_list[0].revents&POLLNVAL) == POLLNVAL) ||
           ((poll_list[1].revents&POLLHUP) == POLLHUP) ||
           ((poll_list[1].revents&POLLERR) == POLLERR) ||
           ((poll_list[1].revents&POLLNVAL) == POLLNVAL)) 
          return 0;


        if((poll_list[0].revents&POLLIN) == POLLIN)
          handle(poll_list[0].fd,NORMAL_DATA);

        if((poll_list[0].revents&POLLPRI) == POLLPRI)
          handle(poll_list[0].fd,HIPRI_DATA);

        if((poll_list[1].revents&POLLIN) == POLLIN)
          handle(poll_list[1].fd,NORMAL_DATA);

        if((poll_list[1].revents&POLLPRI) == POLLPRI)
          handle(poll_list[1].fd,HIPRI_DATA);
    }
}