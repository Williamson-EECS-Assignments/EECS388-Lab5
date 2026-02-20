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
#define BLUE_LED        (GPIO_11)
#define GREEN_LED       (GPIO_12)

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
static void led() {}
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
    delay_us(SERVO_PERIOD - pulseLen_us);

    // NOTE: technically there will be some amount of time between the end of
    // this function call and the time it gets called again but it should be
    // small enough to not worry about it (hopefully, I haven't checked thats
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
        //            - Comment out the for loops and servo() function if you attempt the bonus task
        //led();
#endif // BONUS_TASK
        }
}