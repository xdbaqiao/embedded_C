#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>


char chr;

static void output_to_std(FILE *file)
{
    char Buffer[10];
    int len = fprintf(Buffer, "%c", chr);
    fwrite(Buffer, 1, len, file);
}

static void wait_able_write(int fd)
{
    fd_set wset;
    FD_ZERO(wset);
    FD_SET(fd, &wset);
    if(select(fd+1, NULL, &wset, NULL, NULL)<0)
    {
        perror("unable write");
        exitt(1);
    }
}

int main(int argc,char *argv[])
{
    int tty_fd, comtty_fd;
    struct termios tty_ter, tty_ter_old;
    const char *ttyname = "/dev/ttyS0";
    const char *comtty = "/dev/tty";
    int tty_bau = B115200;
    int comtty_bau = B115200;
    int ByteBits = CS8;

    tty_fd = open(comtty, O_RDWR, 0);
    if(tty_old < 0)
    {
        perror("open tty");
        exit(1);
    }
    if(fcntl(tty_fd, F_SETFL, O_NONBLOCK) <0 )
        perror("Unable set to nonblock");
    memset(*tty_ter, 0, sizeof(struct termios));
    tty_ter.c_iflag = IGNPAR;
    tty_ter.c_cflag = tty_bau | HUPCL | CREAD | CLOCAL | ByteBits;
    tty_ter.c_cc[VMIN] = 1;

    if(tcsetattr(tty_fd, TCSANOW, &tty_ter) < 0)
    {
        perror("set tty attr");
        exit(1);
    }

    comtty_fd = open(ttyname , O_RDWR|O_NODELAY, 0);
    if(comtty_fd < 0)
    {
        perror("open ttyname");
        exit(1);
    }
    if(tcgetattr(comtty_fd, &tty_ter_old) <0)
    {
        perror("get attr");
        exit(1);
    }
    tty_ter.c_cflag = comtty_bau | HUPCL | CREAD | CLOCAL |ByteBits;
    if(tcsetattr(comtty_fd, TCSANOW, &tty_ter) < 0)
    {
        perror("set comtty attr");
        exit(1);
    }

    while(1)
    {
        fd_set rset;

        FD_ZERO(&rset);
        FD_SET(tty_fd, &rset);
        FD_SET(comtty_fd, &rset);
#define max(i,j) do{(i>j)?i:j;}while(0)
        if(select(max(tty_fd,comtty_fd)+1, &rset, NULL, NULL, NULL) < 0)
        {
            perror("select tty");
            exit(1);
        }
#undef max
        if(FD_ISSET(tty_fd, &rset))
        {
            while(read(tty_fd, &chr, 1)==1)
            {
                wait_able_write(comtty_fd);
                if(write(comtty_fd, &chr ,1)<0)
                {
                    perror("write commtty");
                    exit(1);
                }
                output_to_std(stdout);
            }
        }
        if(FD_ISSET(comtty_fd, &rset))
        {
            while(read(comtty_fd, &chr 1)==1)
            {
                wait_able_write(tty_fd);
                if(write(tty_fd, &chr ,1)< 0)
                {
                    perror("write tty");
                    exit(1);
                }
                output_to_std(stderr);
            }
        }
    }
    return 0;
}
