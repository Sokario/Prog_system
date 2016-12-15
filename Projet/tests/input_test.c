#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <string.h>
#include <stdio.h>

/*void grab(int fd, int act) {
   int r =  ioctl(fd, EVIOCGRAB, act);
   printf("grab: %d\n", r);
}

int main(void)
{
//    const char *dev = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";
    const char *dev = "/dev/input/event3";
    struct input_event ev;
    ssize_t n;
    char name[256] = "Unknown";
    int fd;

    fd = open(dev, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Cannot open %s: %s.\n", dev, strerror(errno));
        return EXIT_FAILURE;
    }

//    grab(fd, 1);

    ioctl (fd, EVIOCGNAME (sizeof (name)), name);
    printf ("Reading From : %s (%s)n\n", dev, name);

    while (1) {
        n = read(fd, &ev, sizeof ev);
        if (n == (ssize_t)-1) {
            if (errno == EINTR)
                continue;
            else
                break;
        } else
        if (n != sizeof ev) {
            errno = EIO;
            break;
        }
        if (ev.type == EV_KEY && ev.value >= 0 && ev.value <= 2)
            printf("%d 0x%04x (%d)\n", ev.value, (int)ev.code, (int)ev.code);

    }

//    grab(fd, 0);
    fflush(stdout);
    fprintf(stderr, "%s.\n", strerror(errno));
    return EXIT_FAILURE;
}*/

/*int main(void){
  int c;
  // use system call to make terminal send all keystrokes directly to stdin
  system ("/bin/stty raw");
  while((c=getchar())!= '.') {
    // type a period to break out of the loop, since CTRL-D won't work raw
    putchar(c);
  }
  // use system call to set terminal behaviour to more normal behaviour
  system ("/bin/stty cooked");
  return 0;
}*/

int main(int argc, char const *argv[])
{
    printf("value : %d", '\n');
    return 0;
}