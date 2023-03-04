#include <Arduino.h>
#include <HardwareSerial.h>
#include <sys/time.h>

#include <iomanip>
#include <ios>
#include <sstream>

HardwareSerial uart1(1);
//HardwareSerial uart2(2);

int64_t get_time_us();
int32_t get_elapsed_time();

void setup() {
    // set up serial ports:
    Serial.begin(921600);
    uart1.begin(9600, SERIAL_8N1, 4, 2);
    //uart2.begin(9600, SERIAL_8N1, 16, 17);
    // interrupt on every received byte:
    uart1.setRxFIFOFull(1);
    //uart2.setRxFIFOFull(1);

    // clear the UART buffers
    while (uart1.available() > 0) {
        uart1.read();
    }
    while (uart2.available() > 0) {
        uart2.read();
    }
}

void loop() {

    int byte1 = 0;
    int byte2 = 0;
    bool byte1Received = false;
    bool byte2Received = false;

    while (!(byte1Received || byte2Received)) {
        // read the UART channels
        if (uart1.available() > 0) {
            byte1 = uart1.read();
            byte1Received = true;
        }

        if (uart2.available() > 0) {
            byte2 = uart2.read();
            byte2Received = true;
        }
    }

    int elapsed_time = get_elapsed_time();

    // print the formatted output: byte1 byte2 elapsed_time (us)
    std::ostringstream stream("", std::ios_base::app);
    if (byte1Received)
        stream << std::setfill('0') << std::setw(2) << std::hex << byte1;
    else
        stream << "..";
    stream << " ";
    if (byte2Received)
        stream << std::setfill('0') << std::setw(2) << std::hex << byte2;
    else
        stream << "..";
    stream << " " << std::setfill(' ') << std::setw(6) << std::dec << elapsed_time;
    Serial.println(stream.str().c_str());
}

int64_t get_time_us() {
    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    return (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;
}

int get_elapsed_time() {
    static int64_t timestamp = 0;
    int64_t now = get_time_us();
    int64_t elapsed_time = now - timestamp;
    timestamp = now;
    return (int)std::min(elapsed_time, (int64_t)999999);
}