#include "Vled_scroll.h"
#include "verilated.h"
#include <unistd.h>   // usleep
#include <termios.h>  // terminal settings
#include <fcntl.h>    // non-blocking input
#include <stdio.h>
#include <signal.h>
#include <stdint.h>

static struct termios og_settings;

// Print LEDs such that * is on . is off
void print_leds(uint8_t leds) {
    for (int i = 7; i >= 0; i--) {
        printf("%c", (leds & (1 << i)) ? '*' : '.');
    }
    printf("\n");
}

// Restore keyboard input
void restore_terminal() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &og_settings);
}

// Enable non-blocking keyboard input
void setup_terminal() {
    tcgetattr(STDIN_FILENO, &og_settings);
    struct termios new_settings;
    tcgetattr(0, &new_settings);
    new_settings.c_lflag &= ~ICANON; // disable buffered I/O
    new_settings.c_lflag &= ~ECHO;   // disable echo
    tcsetattr(0, TCSANOW, &new_settings);
    fcntl(0, F_SETFL, O_NONBLOCK);   // non-blocking input
    
    std::atexit(restore_terminal);
}

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);

    Vled_scroll* top = new Vled_scroll;

    setup_terminal();

    top->clk = 0;
    top->eval();

    int delay = 200000; // 200ms
    int paused = 0;

    printf("Controls: 'p' pause/resume | 'q' quit | '0', '1', '2' switch between modes\n");
    printf("Mode %d\n", top->mode);

    while (1) {
        // Read keypress
        char ch = getchar();
        if (ch != EOF) {
            if (ch == 'p') {
                paused = !paused;
            } else if (ch >= '0' && ch <= '2') {
            	top->mode = ch - '0';
            	printf("Switched to mode %d\n", top->mode);
            } else if (ch == 'q') {
            	restore_terminal();
                break;
            }
        }

        if (!paused) {
            top->clk = !top->clk;
            top->eval();
            
            if (top->clk) { 
                print_leds(top->leds);
                usleep(delay);
            }
        } else {
            usleep(100000); // wait while paused
        }
    }

    delete top;
    return 0;
}


