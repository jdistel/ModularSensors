/*
 *SodaqUBeeR410M.h
 *This file is part of the EnviroDIY modular sensors library for Arduino
 *
 *Initial library developement done by Sara Damiano (sdamiano@stroudcenter.org).
 *
 *This file is the Sodaq UBee based on the u-blox SARA R410M LTE-M Cellular Module
*/

// Header Guards
#ifndef SodaqUBeeR410M_h
#define SodaqUBeeR410M_h

// Debugging Statement
// #define MS_SODAQUBEER410M_DEBUG
// #define MS_SODAQUBEER410M_DEBUG_DEEP

#ifdef MS_SODAQUBEER410M_DEBUG
#define MS_DEBUGGING_STD "SodaqUBeeR410M"
#endif

#define TINY_GSM_MODEM_SARAR4
#ifndef TINY_GSM_RX_BUFFER
#define TINY_GSM_RX_BUFFER 64
#endif

// Status should be monitored on the V_INT pin
// V_INT becomes active mid-way through on-pulse so it should be instantly visible
#define R410M_STATUS_LEVEL HIGH
#define R410M_STATUS_TIME_MS 0

// R4 series are reset with a >10 SECOND low pulse on the RESET_N pin
#define R410M_RESET_LEVEL LOW
#define R410M_RESET_PULSE_MS 10000L

// Module is switched on by a 0.15-3.2 second LOW pulse on the PWR_ON pin
#define R410M_WAKE_LEVEL LOW
#define R410M_WAKE_PULSE_MS 200
// Time after power on before PWR_ON can be used ??? Unclear in documentation!
#define R410M_WARM_UP_TIME_MS 250
// Time until system and digital pins are operational (~4.5s)
#define R410M_ATRESPONSE_TIME_MS 4500L

// Power down time "can largely vary depending
// on the application / network settings and the concurrent module
// activities."  Vint/status pin should be monitored and power not withdrawn
// until that pin reads low.  Giving 15sec here in case it is not monitored.
#define R410M_DISCONNECT_TIME_MS 15000L

// Included Dependencies
#include "ModSensorDebugger.h"
#undef MS_DEBUGGING_STD
#include "TinyGsmClient.h"
#include "LoggerModem.h"

#ifdef MS_SODAQUBEER410M_DEBUG_DEEP
#include <StreamDebugger.h>
#endif

class SodaqUBeeR410M : public loggerModem
{

public:
    // Constructor/Destructor
    #if F_CPU == 8000000L
    // At this slow baud rate, we need to begin and end serial communication,
    // so we need a Serial instance rather than a stream
    SodaqUBeeR410M(HardwareSerial* modemStream,
                   int8_t powerPin, int8_t statusPin,
                   int8_t modemResetPin, int8_t modemSleepRqPin,
                   const char *apn);
    #else
    SodaqUBeeR410M(Stream* modemStream,
                   int8_t powerPin, int8_t statusPin,
                   int8_t modemResetPin, int8_t modemSleepRqPin,
                   const char *apn);
    #endif
    ~SodaqUBeeR410M();

    bool modemWake(void) override;

    virtual bool connectInternet(uint32_t maxConnectionTime = 50000L) override;
    virtual void disconnectInternet(void) override;

    virtual uint32_t getNISTTime(void) override;

    virtual bool getModemSignalQuality(int16_t &rssi, int16_t &percent) override;
    virtual bool getModemBatteryStats(uint8_t &chargeState, int8_t &percent, uint16_t &milliVolts) override;
    virtual float getModemChipTemperature(void) override;

    bool modemHardReset(void) override;

#ifdef MS_SODAQUBEER410M_DEBUG_DEEP
    StreamDebugger _modemATDebugger;
#endif

    TinyGsm gsmModem;
    TinyGsmClient gsmClient;

#if F_CPU == 8000000L
    HardwareSerial *_modemSerial;
#endif

protected:
    virtual bool isInternetAvailable(void) override;
    virtual bool modemSleepFxn(void) override;
    virtual bool modemWakeFxn(void) override;
    virtual bool extraModemSetup(void) override;

private:
    const char *_apn;

};

#endif  // Header Guard
