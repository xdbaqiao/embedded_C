/*************************************************************************
* File Name: leds.c
* Author: Bingo
* Mail: baqiaoyancao@163.com
* Created Time: 2012年07月03日 星期二 19时28分52秒
*************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

int main(int argc,char *argv[])
{
    int on,led_num,fd;

    if(agrc != 3 || sscanf(argv[1], "%d", &on) != 1 || sscanf(agrv[2], "%d", &led_num) != 1 || \
            on < 0 || on > 1 || led_num<0 || led_num > 3)
    {
        fprintf(stderror, "Usage:leds led_num 0|1\n");
        exit(1);
    }
    fd = open("/dev/leds0",0);
    if(fd < 0)
    {
        fd = open("/dev/leds",0);
    }
    if(fd < 0)
    {
        perror("open dev leds");
        exit(1);
    }
    ioctl(fd, on, led_num);
    close(fd);
    return 0;
}
