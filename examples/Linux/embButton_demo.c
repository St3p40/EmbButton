#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <stdlib.h>
#define EMBBTN_FUNCTIONS_IMPLEMENTATION
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

unsigned char btn_read(void) {
    return key_down;
}

int main(int argc, char **argv)
{
    millis();
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    const char *path = argv[1];
    embButton_t btn;
    embButtonMillis = millis;
    btn.debounceTime=100;
    btn.releaseTime=500;
    btn.holdTime=1000;
    btn.buttonCheck=btn_read;
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }
    printf("\033[?25l\033[2J\033[1;1H"); //Clear screen and hide cursor
    while (1) {
        printf("\033[H"); //Go to 1st line
        puts("EmbButton Example\n");
        char x = 0;
        lseek(fd, 0, SEEK_SET);
        read(fd,&x,1);

        printf("\033[KChecking GPIO value file: %s%s%s\n\n",(x == '1' || x == '0')? GREEN : RED, path, RESET); //Erase printed line before printing new one

        key_down = (x == '1');

        embButtonTick(&btn);

        printf("Raw input: %s%d%s\n", (key_down)? GREEN : RED, key_down, RESET);
        printf("\033[KTime: %s%lu ms%s\n", GREEN, millis(), RESET);

printf("State:  %s  %s  %s  %s\n",
       (btn.s.state == 0)  ? GREEN "AWAIT"    RESET : RED "Await" RESET,
       (btn.s.state == 1) ? GREEN "RELEASED" RESET : RED "Released" RESET,
       (btn.s.state == 2)  ? GREEN "PRESSED"  RESET : RED "Pressed" RESET,
       (btn.s.state == 3)  ? GREEN "HOLD"     RESET : RED "Hold" RESET);

printf("Flags:  %s  %s  %s  %s\n",
       (btn.s.endClicking)  ? GREEN "ENDCLICKS" RESET : RED "Endclicks" RESET,
       (btn.s.released) ? GREEN "RELEASED"  RESET : RED "Released" RESET,
       (btn.s.clicked)  ? GREEN "CLICKED"   RESET : RED "Clicked" RESET,
       (btn.s.held)     ? GREEN "HOLD"      RESET : RED "Hold" RESET);

        printf("\033[KClicks: %s%d %s%s\n", (btn.s.endClicking)? RED : GREEN, btn.s.clicks, (btn.s.lastPressType)? "H" : "C", RESET);
        printf("\033[KPress time: %s%ld%s\n", (btn.s.released)? RED : GREEN,((btn.s.state > 1 && !btn.s.clicked) ^ btn.s.released)?(millis()-btn.s.timer):0,RESET);
        printf("\033[KIdle time: %s%ld%s\n", (btn.s.clicked)? RED : GREEN,((btn.s.state <= 1 && !btn.s.released) ^ btn.s.clicked)?(millis()-btn.s.timer):0,RESET);
        fflush(stdout);
        usleep(100000);
    }
    close(fd);
    printf("\033[?25h"); //Show cursor

    return 0;
}
