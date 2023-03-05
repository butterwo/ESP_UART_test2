#include <Arduino.h>
//#include <HardwareSerial.h>
#include <SoftwareSerial.h>

#include <sys/time.h>

#include <iomanip>
#include <ios>
#include <sstream>

//#define CONTROL_FLAG 0x01

SoftwareSerial uart1;

void setup() {
    // set up serial ports:
    Serial.begin(921600);
    Serial.setTimeout(2000);
    uart1.begin(9600, SWSERIAL_8S1, 23, 19, false);
    //uart1.setRxFIFOFull(1);

    // clear the UART buffers
    while (uart1.available() > 0) {
        uart1.read();
    }
}

void loop() {

    int txdelay = 0;
    bool rxinv = false;
    String str = Serial.readStringUntil('\n');
    if (str.length() > 0) {
        str.trim();
        Serial.println(str);
        for (int i = 1; i < str.length(); i += 2) {
            int byte = std::stoi(std::string(str.substring(i, i + 2).c_str()), 0, 16);
            // uart1.write(str.c_str()[i]);
            Serial.println(str.substring(i, i + 2));
            //uart1.end();
            //uart1.begin(9600, SERIAL_8N1, 4, 2);
            // rxinv = !rxinv;
            // uart1.setRxInvert(rxinv);

            uart1.write(byte, SWSERIAL_PARITY_SPACE);
            //uart1.flush();
            if (str[0] == '*') {
                txdelay = 1500;
            } else {
                txdelay = 500;
            }
            usleep(txdelay);
        }
    }
    // while (!(byte1Received /* || byte2Received */)) {
    //     // read the UART channels
    //     if (uart1.available() > 0) {
    //         byte1 = uart1.read();
    //         byte1Received = true;
    //     }

    // }

    //int elapsed_time = get_elapsed_time();

    // print the formatted output: byte1 byte2 elapsed_time (us)
    // std::ostringstream stream("", std::ios_base::app);
    // if (byte1Received)
    //     stream << std::setfill('0') << std::setw(2) << std::hex << byte1;
    // else
    //     stream << "..";
    // stream << " ";
    // Serial.println(stream.str().c_str());
}
