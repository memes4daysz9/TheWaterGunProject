
#include "lib/ssd1306.h"
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

// Assuming we have equivalent libraries for hardware interaction
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
/**
 * @desc    Main function
 *
 * @param   void
 *
 * @return  int
 */
int main(void)
{
    // Initialize hardware
    stdio_init_all();
    
    // Initialize PWM
    gpio_set_function(20, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(20);
    PumpPWM = slice_num;

    // Initialize I2C
    i2c = i2c0;
    i2c_init(i2c, 200000);
    gpio_set_function(0, GPIO_FUNC_I2C);
    gpio_set_function(1, GPIO_FUNC_I2C);
    gpio_pull_up(0);
    gpio_pull_up(1);

    // Initialize OLED
    ssd1306_init(&oled, WIDTH, HEIGHT, 0x3C, i2c);

    // Initialize GPIO
    isCharging = 14;
    SelectMode = 15;
    gpio_init(isCharging);
    gpio_init(SelectMode);
    gpio_set_dir(isCharging, GPIO_IN);
    gpio_set_dir(SelectMode, GPIO_IN);

    // Create thread for screen functions
    pthread_t screen_thread;
    pthread_create(&screen_thread, NULL, ScreenFunctions, NULL);

    // Main loop
    while (true) {
        operationMode();
        if (gpio_get(isCharging)) {
            Charging();
        }
        // Add more functionality here
        sleep(1);
    }  initScreen();
  SSD1306_NormalScreen (SSD1306_ADDR);//not sure what would happen if i were to remove it so
  // RETURN
  // -------------------------------------------------------------------------------------
  return 0;
}

void ClearScreen(){SSD1306_ClearScreen ();}
void DrawLine(uint8_t A, uint8_t B, uint8_t C, uint8_t D){SSD1306_DrawLine(A,B,C,D)}
void SetPos(uint8_t A, uint8_t B){SSD1306_SetPosition (A,B);}
void DrawString(char * A){SSD1306_DrawString (A);}
void Update(){SSD1306_UpdateScreen (SSD1306_ADDR);}
void initScreen(){SSD1306_Init (SSD1306_ADDR);}


void charging(){

}


#define WIDTH 128
#define HEIGHT 32

// Global variables
uint PumpPWM;
int BatteryPercentage = 100;
i2c_inst_t *i2c;
SSD1306_t oled;
uint isCharging;
uint SelectMode;
bool Turbo = false;
char lines[6][20] = {"no", "no", "no", "no", "no", "no"};

void WriteToLine(const char *StrToWrite, int lineNum) {
    int yValue = (lineNum * 5) + 2;
    strncpy(lines[lineNum], StrToWrite, 19);
    lines[lineNum][19] = '\0';
}

void *ScreenFunctions(void *arg) {
    while (true) {
        for (int x = 0; x < 6; x++) {
            WriteToLine(lines[x], x);
        }
    }
    return NULL;
}

uint PWMpercentage(int percentvalue) {
    float first = percentvalue / 100.0f;
    return (uint)(65535 * first);
}

void Charging() {
    int TimeLeft = 252;
    int Seconds = 1;
    WriteToLine("Charging...", 1);
    while (true) {
        char buffer[50];
        snprintf(buffer, sizeof(buffer), "Time Left till full: %d min", TimeLeft);
        WriteToLine(buffer, 2);
        WriteToLine("Charging Wattage: 140 Watts", 3);

        if (gpio_get(isCharging)) {
            break;
        } else {
            if (Seconds == 60) {
                TimeLeft--;
                Seconds = 0;
            } else {
                sleep(1);
                Seconds++;
            }
        }
    }
}

void operationMode() {
    Turbo = gpio_get(SelectMode);
}

void ClearScreen() {
    ssd1306_clear(&oled);
}

