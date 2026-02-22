#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <stdlib.h>
#include "../../src/embButton.h"

static char key_down = 0;

unsigned long millis(void)
{
    static struct timeval start;
    static int initialized = 0;
    struct timeval now;

    if (!initialized) {
        gettimeofday(&start, NULL);
        initialized = 1;
    }

    gettimeofday(&now, NULL);

    long seconds  = now.tv_sec  - start.tv_sec;
    long useconds = now.tv_usec - start.tv_usec;

    if (useconds < 0) {
        seconds--;
        useconds += 1000000;
    }

    return (unsigned long)(seconds * 1000UL +
                           useconds / 1000UL);
}

char btn_read(void) {
    return key_down;
}

void clearScreen() {
  printf("\033[2J\033[1;1H");
}

int main(int argc, char **argv)
{
    millis();
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    const char *path = argv[1];
    embButton_t btn = {0};
    btn.debounceTime=175;
    btn.releaseTime=500;
    btn.holdTime=1000;
    btn.millisFunc=millis;
    btn.buttonCheck=btn_read;
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }
    while (1) {
        clearScreen();
        puts("EmbButton Example\n");
        printf("Checking GPIO value file: %s\n\n", path);
        char x = 0;
        lseek(fd, 0, SEEK_SET);
        read(fd,&x,1);
        key_down = (x == '1');

        embButtonTick(&btn);

        printf("Raw input: %d\n", key_down);
        printf("Time: %lu ms\n", millis());

        printf("State: %s %s %s %s\n",(btn.state == 0)?"\033[92mAWAIT\033[0m":"Await", (btn.state == -1)?"\033[92mRELEASED\033[0m":"Released", (btn.state == 1)?"\033[92mPRESSED\033[0m":"Pressed", (btn.state == 2)?"\033[92mHOLD\033[0m":"Hold");
        printf("Flags: %s %s %s %s\n",(btn.isClicked)?"\033[92mCLICKED\033[0m":"Clicked", (btn.isReleased)?"\033[92mRELEASED\033[0m":"Released", (btn.isHold)?"\033[92mHOLD\033[0m":"Hold", (btn.endClicks)?"\033[92mENDCLICKS\033[0m":"endClicks");
        printf("Clicks: %d\n", btn.clicks);
        //printf("Pr3ss time%d",(millis()-btn.timer));
        fflush(stdout);
        usleep(50000);
    }
    close(fd);

    return 0;
}
