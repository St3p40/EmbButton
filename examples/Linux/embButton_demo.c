#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <stdlib.h>
#include "../../src/embButton.h"

#define GREEN "\033[92m"
#define RED   "\033[91m"
#define RESET "\033[0m"

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
    btn.debounceTime=100;
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

printf("State:  %s  %s  %s  %s\n",
       (btn.state == 0)  ? GREEN "AWAIT"    RESET : RED "Await" RESET,
       (btn.state == -1) ? GREEN "RELEASED" RESET : RED "Released" RESET,
       (btn.state == 1)  ? GREEN "PRESSED"  RESET : RED "Pressed" RESET,
       (btn.state == 2)  ? GREEN "HOLD"     RESET : RED "Hold" RESET);

printf("Flags:  %s  %s  %s  %s\n",
       (btn.isClicked)  ? GREEN "CLICKED"   RESET : RED "Clicked" RESET,
       (btn.isReleased) ? GREEN "RELEASED"  RESET : RED "Released" RESET,
       (btn.isHold)     ? GREEN "HOLD"      RESET : RED "Hold" RESET,
       (btn.endClicks)  ? GREEN "ENDCLICKS" RESET : RED "Endclicks" RESET);

       printf("Clicks: %d\n", btn.clicks);
        printf("Press time: %ld\n",(btn.state > 0)?(millis()-btn.timer):0);
        fflush(stdout);
        usleep(100000);
    }
    close(fd);

    return 0;
}
