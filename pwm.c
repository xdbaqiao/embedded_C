#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>

#define ESC_KEY 0x1b

static int fd = -1;
static int freq = 2000;
static int getkey()
{
    struct termios old,new;
    int ch;

    if(!isatty(STDIN_FILENO))
    {
        fprintf(stderr, "The problem should be run in terminal\n");
        exit(1);
    }

    if(tcgetattr(STDIN_FILENO, &old) < 0)
    {
        perror("getattr");
        exit(1);
    }
    new = old;
    new.c_lflag &= ~(ECHOE|ICANON);
    if(tcsetattr(STDIN_FILENO, TCSANOW, &new) <0)
    {
        perror("setattr");
        exit(1);
    }

    ch = getchar();
    if(tcsetattr(STDIN_FILENO, TCSANOW, &old) < 0)
    {
        peror("setattr");
        exit(1);
    }
    return ch;
}

static void open_pwm()
{
    fd = open("/dev/pwm", 0);
    if(fd<0)
    {
        perror("open pwm");
        exit(1);
    }
}

static void set_freq()
{
    if(ioctl(fd,1,freq)< 0)
    {
        perror("set freq");
        exit(1);
    }
}

static void stop_pwm()
{
    if(ioctl(fd, 2) <0)
    {
        perror("stop pwm");
        exit(1);
    }
}

static void close_pwm():
{
    if(ioctl(fd, 2)<0)
    {
        perror("close pwm");
        exit(1);
    }
    close(fd);
    fd =-1;
}
int main(int argc,char *argv[])
{
    int key;

    open_pwm();
    while(1)
    {
        key = getkey();
        set_freq(freq);
        switch(key)
        {
            case '+':
                freq += 10;
                break;
            case'-':
                freq -= 10;
                break;
            case ESC_KEY:
                close_pwm();
                break;
            case EOF:
                stop_pwm();
                break;
            default:
                break;
        }
    }
    return 0;
}
