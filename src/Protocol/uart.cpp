#include <uart.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>
#include <mutex>

#define SERIAL_PORT_PATH "/dev/ttyS0"
static struct termios g_tty; // for storing serial port settings
static int g_fd = -1;        // file descriptor
static UARTData g_uart_data = {0};
static std::mutex g_data_mutex;
// opens file
int file_open_and_get_descriptor(const char *fname)
{
    int fd;
    // O_RDWR read/write mode
    fd = open(fname, O_RDWR | O_NONBLOCK);
    if (fd < 0)
    {
        printf("Could not open file %s...%d\r\n", fname, fd);
    }
    return fd;
}
// write file(send data out the serial port)
int file_write_data(int fd, uint8_t *buff, uint32_t len_buff)
{
    return write(fd, buff, len_buff);
}
// read file(read from serial portinto a buffer)
int file_read_data(int fd, uint8_t *buff, uint32_t len_buff)
{

    fd_set readfds;
    struct timeval timeout;
    int bytes_read = 0;
    int ret;

    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);

    ret = select(fd + 1, &readfds, NULL, NULL, &timeout); // wait for  data

    if (ret > 0)
    {
        bytes_read = read(fd, buff, len_buff); // reads it
        if (bytes_read < 0)
        {
            perror("error reading data");
            return -1;
        }
    }
    else if (ret == 0)
    {
        printf("No data available");
        return 0;
    }
    else
    {
        perror("select() function failed");
        return -1;
    }
    return bytes_read;
}
void uart_init()
{
    printf("Intializing uart ... \r\n");
    g_fd = file_open_and_get_descriptor(SERIAL_PORT_PATH);
    if (g_fd < 0)
    {
        printf("failed to open serail prot\r\n");
        return;
    }
    if (tcgetattr(g_fd, &g_tty))
    {
        printf("Failed to get port attributes\r\n");
        close(g_fd);
        g_fd = -1;
        return;
    }

    cfsetispeed(&g_tty, B115200);
    cfsetospeed(&g_tty, B115200);

    cfmakeraw(&g_tty);

    if (tcsetattr(g_fd, TCSANOW, &g_tty))
    {
        printf("failed to set port attributes\r\n");
        close(g_fd);
        g_fd = -1;
        return;
    }
    printf("Uart successful on %s at baud rate of 115200\r\n", SERIAL_PORT_PATH);
}
static void parse_uart_data(uint8_t *buffer, int length, UARTData *data)
{

    if (length < 2)
        return; 

    uint8_t msg_type = buffer[0];

    switch (msg_type)
    {
    case 0x01:
        if (length >= 5)
        {
            //temp for brake
            uint16_t front_temp = (buffer[1] << 8) | buffer[2];
            uint16_t rear_temp = (buffer[3] << 8) | buffer[4];
            data->brake_temperature = front_temp;
            printf("Brake Temps - Front: %u, Rear: %u\n", front_temp, rear_temp);
        }
        break;

    case 0x02: // Coolant temperature data
        if (length >= 3)
        {
            
            data->coolant_temperature = (buffer[1] << 8) | buffer[2];
            printf("Coolant Temp: %u\n", data->coolant_temperature);
        }
        break;

    case 0x03: // Motor Controller data
        if (length >= 10)
        {
            data->motor_voltage = (buffer[1] << 8) | buffer[2];
            data->motor_current = (buffer[3] << 8) | buffer[4];
            data->motor_temperature = (buffer[5] << 8) | buffer[6];
            data->motor_rpm = (buffer[7] << 8) | buffer[8];
            data->motor_status = buffer[9];

            printf("Motor - V:%u, I:%u, RPM:%u, Temp:%u, Status:0x%02X\n",
                   data->motor_voltage, data->motor_current, data->motor_rpm,
                   data->motor_temperature, data->motor_status);
        }
        break;

    case 0x04: //can
        if (length >= 11)
        {
            data->can_id_1 = (buffer[1] << 8) | buffer[2];
            memcpy(data->can_data, &buffer[3], 8);

            printf("CAN ID: 0x%04X, Data: ", data->can_id_1);
            for (int i = 0; i < 8; i++)
            {
                printf("%02X ", data->can_data[i]);
            }
            printf("\n");
        }
        break;

    case 0x0F: // Fault data
        if (length >= 3)
        {
            // Fault code 
            data->fault_code = buffer[1];
            // Fault severity: 0=None, 1=Warning, 2=Critical
            data->fault_severity = buffer[2];
            data->has_active_fault = (data->fault_code != 0);

            printf("Fault - Code: 0x%02X, Severity: %u %s\n",
                   data->fault_code, data->fault_severity,
                   data->has_active_fault ? "(ACTIVE)" : "(CLEAR)");
        }
        break;

    default:
        printf("Unknown message type: 0x%02X\n", msg_type);
        break;
    }
}
void uart_update()
{
    if (g_fd < 0)
    {
        return; // UART not initialized
    }

    uint8_t rx_buffer[256];
    int bytes = file_read_data(g_fd, rx_buffer, sizeof(rx_buffer));

    if (bytes > 0)
    {
        std::lock_guard<std::mutex> lock(g_data_mutex);

        // Store raw data
        memcpy(g_uart_data.raw_buffer, rx_buffer, bytes);
        g_uart_data.bytes_received = bytes;
        g_uart_data.data_ready = true;

        // Debug print
        printf("Received %d bytes: ", bytes);
        for (int i = 0; i < bytes; i++)
        {
            printf("%02X ", rx_buffer[i]);
        }
        printf("\n");

        // Parse the UART data
        parse_uart_data(rx_buffer, bytes, &g_uart_data);
    }
}

// Get current UART data 
UARTData uart_get_data()
{
    std::lock_guard<std::mutex> lock(g_data_mutex);
    return g_uart_data;
}

// Check if new data is available
bool uart_has_new_data()
{
    std::lock_guard<std::mutex> lock(g_data_mutex);
    bool ready = g_uart_data.data_ready;
    g_uart_data.data_ready = false; 
    return ready;
}

// Cleanup UART
void uart_cleanup()
{
    if (g_fd >= 0)
    {
        close(g_fd);
        g_fd = -1;
        printf("UART closed\r\n");
    }
}

// close
int file_close(int fd)
{
    return close(fd);
}
void testCase()
{
    printf("Running test case... \n");

    uint8_t fake_rx[] = {0xAB, 0xCD, 0xEF, 0x01, 0x23};
    int fake_len = sizeof(fake_rx);

    printf("Received %d bytes:\n", fake_len);

    for (int i = 0; i < fake_len; i++)
    {
        printf("%02X ", fake_rx[i]);
    }
    printf("\n");
    printf("Test case complete.\n");
}
