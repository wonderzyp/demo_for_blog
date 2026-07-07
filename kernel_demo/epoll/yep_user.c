#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/select.h>
#include <sys/epoll.h>
#include <errno.h>

#define DEV_PATH "/dev/poll_select_demo"
#define MAX_EPOLL_EVENTS 8

static void read_device(int fd)
{
    char buf[256];
    ssize_t n;

    memset(buf, 0, sizeof(buf));

    n = read(fd, buf, sizeof(buf) - 1);
    if (n < 0) {
        perror("read");
        return;
    }

    printf("read: %s", buf);
}

static int select_demo(int fd)
{
    while (1) {
        fd_set rfds;
        int ret;

        FD_ZERO(&rfds);
        FD_SET(fd, &rfds);

        printf("user: waiting by select()...\n");

        ret = select(fd + 1, &rfds, NULL, NULL, NULL);
        if (ret < 0) {
            if (errno == EINTR)
                continue;
            perror("select");
            return EXIT_FAILURE;
        }

        if (FD_ISSET(fd, &rfds)) {
            printf("user: select() says fd is readable\n");
            read_device(fd);
        }
    }

    return EXIT_SUCCESS;
}

static int  poll_demo(int fd)
{
    struct pollfd pfd;

    pfd.fd = fd;
    pfd.events = POLLIN;

    while (1) {
        int ret;

        printf("user: waiting by poll()...\n");

        ret = poll(&pfd, 1, -1);
        if (ret < 0) {
            if (errno == EINTR)
                continue;
            perror("poll");
            return EXIT_FAILURE;
        }

        if (pfd.revents & POLLIN) {
            printf("user: poll() says fd is readable\n");
            read_device(fd);
        }

        if (pfd.revents & (POLLERR | POLLHUP | POLLNVAL)) {
            printf("user: poll error revents=0x%x\n", pfd.revents);
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

static int epoll_demo(int fd)
{
    int epfd;
    int ret;
    struct epoll_event ev;
    struct epoll_event events[MAX_EPOLL_EVENTS];

    epfd = epoll_create1(0);
    if (epfd < 0) {
        perror("epoll_create1");
        return EXIT_FAILURE;
    }

    memset(&ev, 0, sizeof(ev));
    ev.events = EPOLLIN;
    ev.data.fd = fd;

    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
    if (ret < 0) {
        perror("epoll_ctl");
        close(epfd);
        return EXIT_FAILURE;
    }

    while (1) {
        int i;
        int nfds;

        printf("user: waiting by epoll_wait()...\n");

        nfds = epoll_wait(epfd, events, MAX_EPOLL_EVENTS, -1);
        if (nfds < 0) {
            if (errno == EINTR)
                continue;
            perror("epoll_wait");
            close(epfd);
            return EXIT_FAILURE;
        }

        for (i = 0; i < nfds; i++) {
            if (events[i].events & EPOLLIN) {
                printf("user: epoll says fd is readable\n");
                read_device(events[i].data.fd);
            }

            if (events[i].events & (EPOLLERR | EPOLLHUP)) {
                printf("user: epoll error events=0x%x\n",
                       events[i].events);
                close(epfd);
                return EXIT_FAILURE;
            }
        }
    }

    close(epfd);
    return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
    int fd;
    int ret;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <select|poll|epoll>\n", argv[0]);
        return EXIT_FAILURE;
    }

    fd = open(DEV_PATH, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "select") == 0)
        ret = select_demo(fd);
    else if (strcmp(argv[1], "poll") == 0)
        ret = poll_demo(fd);
    else if (strcmp(argv[1], "epoll") == 0)
        ret = epoll_demo(fd);
    else {
        fprintf(stderr, "Unknown mode: %s\n", argv[1]);
        ret = EXIT_FAILURE;
    }

    close(fd);
    return ret;
}