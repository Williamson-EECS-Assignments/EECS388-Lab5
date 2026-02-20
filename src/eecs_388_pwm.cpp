/******************************************************************************
 *   Header Includes
 *******************************************************************************/
#include <Arduino.h>
#include <stdint.h>
#include <stdio.h>

#include "eecs_388_lib.h"

#define SERVO_PULSE_MAX (2400)    /* 2400 us */
#define SERVO_PULSE_MIN (544)     /* 544 us */
#define SERVO_PERIOD    (20000)   /* 20000 us (20ms) */

#define MIN_ANGLE       (0)       /* degrees */
#define MAX_ANGLE       (180)     /* degrees */
#define DEGREE_STEP     (30)      /* degrees */

#define RED_LED         (GPIO_13)
#define GREEN_LED       (GPIO_12)
#define BLUE_LED        (GPIO_11)

#define RGB(_r, _g, _b) gpio_write(RED_LED, _r); \
                        gpio_write(GREEN_LED, _g); \
                        gpio_write(BLUE_LED, _b)

// #define BONUS_TASK

// Bonus Task - Uncomment and fill out this function if you attempt the bonus task
//           +---------------+ 
//           |   RGB LED     |
//           +---------------+
//           |   |     |     |
//          R   GND    G     B
//          |    |     |     |
//         [--] [---] [--]  [-]
//          |    |     |     |
//         340Ω  |    340Ω  340Ω
//          |    |     |     MCU
//         MCU   |    MCU    Pin
//         Pin  GND   Pin
//              Pin
#ifdef BONUS_TASK
static void led() {
    constexpr double R[] = {0.0, 1.0, 0.3, 0.8, 1.0};
    constexpr double G[] = {1.0, 0.1, 0.8, 0.0, 0.6};
    constexpr double B[] = {1.0, 1.0, 0.0, 0.2, 1.0};
    constexpr double TIME_PER = SERVO_PERIOD / 3; // each LED is afforded 1/3 of the cycle to "do its thing"

    for (int i = 0; i < 5; i++) {
        /* control the led for 1/2 sec duration */
        for (int i = 0; i < 25; i++) {
            int rDelay = TIME_PER * R[i],
                gDelay = TIME_PER * G[i],
                bDelay = TIME_PER * B[i];

            // turn on red for however long was calculated
            RGB(ON, OFF, OFF);
            delay_us(rDelay);
            // turn on green for however long was calculated
            RGB(OFF, ON, OFF);
            delay_us(gDelay);
            // turn on blue for however long was calculated
            RGB(OFF, OFF, ON);
            delay_us(bDelay);
        }
    }
}
#endif // BONUS_TASK

// bounded formula to get the pulse length from the angle
//  this is an approximation
static double getPulseLength(int angle) {
    // formula to get the pulse length from the angle
    double p = ((double)angle / 0.18) + 1000;
    // bound p; SERVO_PULSE_MIN <= p <= SERVO_PULSE_MAX (i.e. [544, 2400])
    return max(SERVO_PULSE_MIN, min(p, SERVO_PULSE_MAX));
}

/******************************************************************************
 *   Function: servo() - Servo Motor Pulse
 *      Pre condition:
 *          Assumes GPIO pin used has been configured for output
 *      Post condition:
 *          Calculates a duty cycle in microseconds for the amount of on time
 *          needed to achieve the angle passed in
 *******************************************************************************/
static void servo(int gpio, int angle) {
    // YOUR CODE HERE
    // Basically, you need to take the input angle "angle" and generate the corresponding pwm signal
    // To generate the pwm signals, use gpio_write(), delay_us(), and delay_ms().

    double pulseLen_us = getPulseLength(angle);

    //    ╭ 1-2ms
    //   ┎----┓
    //   |    |
    //   |    |______________
    //   ╚----- 20 ms-------╝
    // turn on the gpio line for the given time
    gpio_write(gpio, ON);
    delay_us(pulseLen_us);
    // turn off for the remaining time
    gpio_write(gpio, OFF);
    delay_us(10000); // can't call for longer than 16ms so we just take 10ms in one call
    delay_us(SERVO_PERIOD - pulseLen_us - 10000); // then finish off the remaining

    // NOTE: technically there will be some amount of time between the end of
    // this function call and the time it gets called again but it should be
    // small enough to not worry about (hopefully, I haven't checked, thats
    // just a gut feeling...)
}

/******************************************************************************
 *   Function: setup() - Initializes the Arduino System
 *      Pre condition:
 *          Hardware must be properly connected (sensors, LEDS, etc.)
 *      Post condition:
 *          Runs initialization calls one time on power up
 *          Input/Output (IO) pins are configured
 *******************************************************************************/
void setup() {
    gpio_mode(GPIO_6, GPIO_OUTPUT);

    // Bonus Task - Uncomment and fill out this function if you attempt the bonus task
#ifdef BONUS_TASK
    gpio_mode(RED_LED, GPIO_OUTPUT);
    gpio_mode(BLUE_LED, GPIO_OUTPUT);
    gpio_mode(GREEN_LED, GPIO_OUTPUT);
#endif // BONUS_TASK
}

/******************************************************************************
 *   Function: loop() - Main execution loop
 *      Pre condition: 
 *          setup() has been executed and system is initialized
 *      Post condition: 
 *          Performs a single iteration of the system's function
 *          Repeates indefinetely unless the board is reset or powered off
 *******************************************************************************/
void loop() {
    int gpio = GPIO_6;

    while (true) {

#ifndef BONUS_TASK
        /**
         * Sweeps through angles 0 -> 180 by 30 to test the 'servo' function 
         *
         * The inner loop calls the function 50 times, causing each angle
         * to be held on the servo for ~1 second.
         *
         * Do you understand why 1 second?
         */
        for (int angle = MIN_ANGLE; angle <= MAX_ANGLE; angle += DEGREE_STEP) {
            /* control the servo for 1 sec duration */
            for (int i = 0; i < 50; i++) {
                servo(gpio, angle);
            }
        }
#endif // !BONUS_TASK

#ifdef BONUS_TASK
        // Bonus Task - Uncomment and fill out this function if you attempt the bonus task
        led();
#endif // BONUS_TASK
        }
}