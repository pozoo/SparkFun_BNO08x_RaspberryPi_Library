// spi_example.cpp
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

int main(int argc, char* argv[]) {
    const char* device = "/dev/spidev0.0";  // SPI bus 0, chip select 0
    uint8_t mode = SPI_MODE_0;
    uint8_t bits = 8;
    uint32_t speed = 500000; // 500 kHz – safe to start
    int fd = open(device, O_RDWR);
    if (fd < 0) {
        std::perror("open");
        return EXIT_FAILURE;
    }

    if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0 ||
        ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0 ||
        ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) {
        std::perror("SPI config");
        close(fd);
        return EXIT_FAILURE;
    }

    uint8_t txBuf[4] = { 0xAA, 0xBB, 0xCC, 0xDD };
    uint8_t rxBuf[sizeof(txBuf)] = {0};

    struct spi_ioc_transfer tr;
    std::memset(&tr, 0, sizeof(tr));
    tr.tx_buf = (unsigned long)txBuf;
    tr.rx_buf = (unsigned long)rxBuf;
    tr.len = sizeof(txBuf);
    tr.speed_hz = speed;
    tr.bits_per_word = bits;

    if (ioctl(fd, SPI_IOC_MESSAGE(1), &tr) < 1) {
        std::perror("SPI transfer");
        close(fd);
        return EXIT_FAILURE;
    }

    std::cout << "Received bytes: ";
    for (size_t i = 0; i < sizeof(rxBuf); ++i)
        std::cout << std::hex << (int)rxBuf[i] << " ";
    std::cout << std::dec << std::endl;

    close(fd);
    return EXIT_SUCCESS;
}
