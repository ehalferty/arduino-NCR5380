//Arduino NCR5380 Library
//Copyright 2020 Edward Halferty
// Portions of this file copied from Linux source, copyright various people such as:
// Linus Torvalds, Drew Eckhardt, Ray Van Tassle, Ingmar Baumgart, Ronald van Cuijlenborg, Alan Cox, and others.

#ifndef ncr5380_h
#define ncr5380_h

#include <avr/pgmspace.h>
#include "arduino.h"
#include "linux_ncr5380.h"

#define NUM_POLL_ITERATIONS 1000

#define SET_PIN_NUMBERS() _cs_=cs_;_drq=drq;_irq=irq;_ior_=ior_;_ready=ready;_dack_=dack_;_eop_=eop_;_reset_=reset_;\
_iow_=iow_;_a0=a0;_a1=a1;_a2=a2;_d0=d0;_d1=d1;_d2=d2;_d3=d3;_d4=d4;_d5=d5;_d6=d6;_d7=d7;

#define SET_INITIAL_PIN_DIRECTIONS() pinMode(_cs_, OUTPUT);pinMode(_drq, INPUT);pinMode(_irq, INPUT);\
pinMode(_ior_, OUTPUT);pinMode(_ready, INPUT);pinMode(_dack_, OUTPUT);pinMode(_eop_, OUTPUT);pinMode(_reset_, OUTPUT);\
pinMode(_iow_, OUTPUT);pinMode(_a0, OUTPUT);pinMode(_a1, OUTPUT);pinMode(_a2, OUTPUT);pinMode(_d0, OUTPUT);\
pinMode(_d1, OUTPUT);pinMode(_d2, OUTPUT);pinMode(_d3, OUTPUT);pinMode(_d4, OUTPUT);pinMode(_d5, OUTPUT);\
pinMode(_d6, OUTPUT);pinMode(_d7, OUTPUT);

#define SET_INITIAL_PIN_VALUES() digitalWrite(_cs_, HIGH);/* Initially disabled */ digitalWrite(_reset_, HIGH);\
digitalWrite(_dack_, HIGH);digitalWrite(_eop_, HIGH);

#define SET_DATA_DIRECTION(x) pinMode(_d0, x);pinMode(_d1, x);pinMode(_d2, x);pinMode(_d3, x);\
pinMode(_d4, x);pinMode(_d5, x);pinMode(_d6, x);pinMode(_d7, x);

#define SET_DATA(x) digitalWrite(_d0, x & 0x01);digitalWrite(_d1, (x >> 1) & 0x01);digitalWrite(_d2, (x >> 2) & 0x01);\
digitalWrite(_d3, (x >> 3) & 0x01);digitalWrite(_d4, (x >> 4) & 0x01);digitalWrite(_d5, (x >> 5) & 0x01);\
digitalWrite(_d6, (x >> 6) & 0x01);digitalWrite(_d7, (x >> 7) & 0x01);
#define SET_ADDR(x) digitalWrite(_a0, x & 0x01);digitalWrite(_a1, (x >> 1) & 0x01);digitalWrite(_a2, (x >> 2) & 0x01);
#define SET_WRITE_PINS()   digitalWrite(_iow_, LOW); digitalWrite(_cs_,  LOW);
#define CLEAR_WRITE_PINS() digitalWrite(_cs_,  HIGH);digitalWrite(_iow_, HIGH);
#define SET_READ_PINS()    digitalWrite(_ior_, LOW); digitalWrite(_cs_,  LOW);
#define CLEAR_READ_PINS()  digitalWrite(_cs_,  HIGH);digitalWrite(_ior_, HIGH);
#define PULSE_WRITE_PINS() delay(1);SET_WRITE_PINS();delay(1);CLEAR_WRITE_PINS();delay(1);
#define PULSE_RESET_PIN() digitalWrite(_reset_, LOW); delay(100); digitalWrite(_reset_, HIGH); delay(100);

#define RESET_BUS() NCR5380_write(INITIATOR_COMMAND_REG, ICR_ASSERT_RST);delay(1);NCR5380_write(INITIATOR_COMMAND_REG, 0);
#define CLEAR_INTERRUPT_CONDITIONS() (void)NCR5380_read(RESET_PARITY_INTERRUPT_REG);

#define ID_MASK 1 << scsiId
#define ID_HIGHER_MASK 0b11111111 << scsiId + 1

// This doesn't cover all inquiry result data, just the fields I thought people would care about. Please add the rest
// if you need them!
struct InquiryData {
    int peripheralQualifier;
    int deviceTypeCode;
    bool removableMediaBit;
    int ansiScsiVersion;
    int additionnalDataLength;
    char vendorIdStr[9];
    char productIdStr[17];
    char productRevStr[5];
    char vendorSpecificInfoStr[21];
    char vendorSpecificData[128];
};

class NCR5380 {
public:
    NCR5380(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int);
    void begin();
    byte readCurrentScsiDataReg();
    void setLoggingEnabled(bool);
    void setVerboseLoggingEnabled(bool);
    void setScsiId(int);
    void test();
    InquiryData inquiryResult;
private:
    int _cs_;
    int _drq;
    int _irq;
    int _ior_;
    int _ready;
    int _dack_;
    int _eop_;
    int _reset_;
    int _iow_;
    int _a0;
    int _a1;
    int _a2;
    int _d0;
    int _d1;
    int _d2;
    int _d3;
    int _d4;
    int _d5;
    int _d6;
    int _d7;
    bool loggingEnabled = false;
    bool verboseLoggingEnabled = false;
    int scsiId = 7;
    void NCR5380_write(byte, byte);
    byte NCR5380_read(byte);
    bool NCR5380_arbitrate();
    bool NCR5380_select(int);
    bool NCR5380_poll_politely(int, byte, byte);
    bool NCR5380_poll_politely2(int, byte, byte, int, byte, byte);
    bool NCR5380_transfer_pio(byte *, int *, byte **);
    bool NCR5380_command(byte *, int);
    bool NCR5380_data_in(byte *, int);
    int NCR5380_data_in_variable_length(byte *, int);
    bool NCR5380_inquiry(int);
    byte NCR5380_wait_phase(byte);
};

#endif