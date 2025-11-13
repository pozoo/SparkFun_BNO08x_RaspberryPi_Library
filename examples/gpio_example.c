#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>

#define CONSUMER "gpio-example"
#define GPIO_PIN 17   // BCM numbering (line offset)

int main(void)
{
    struct gpiod_chip *chip;
    struct gpiod_line *line;
    int ret;

    // Open the GPIO chip (usually /dev/gpiochip0)
    chip = gpiod_chip_open_by_name("gpiochip0");
    if (!chip) {
        perror("Open chip failed");
        return 1;
    }

    // Get line (GPIO number)
    line = gpiod_chip_get_line(chip, GPIO_PIN);
    if (!line) {
        perror("Get line failed");
        gpiod_chip_close(chip);
        return 1;
    }

    // Request line as output, initial value 0
    ret = gpiod_line_request_output(line, CONSUMER, 0);
    if (ret < 0) {
        perror("Request line as output failed");
        gpiod_chip_close(chip);
        return 1;
    }

    // Toggle the pin 5 times
    for (int i = 0; i < 5; i++) {
        gpiod_line_set_value(line, 1);
        usleep(500000);
        gpiod_line_set_value(line, 0);
        usleep(500000);
    }

    // Clean up
    gpiod_line_release(line);
    gpiod_chip_close(chip);
    return 0;
}
