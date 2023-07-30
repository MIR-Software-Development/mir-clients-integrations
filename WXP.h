#ifndef WXP_H
#define WXP_H

#include "Patient.h"

class WXP
{
public:
    WXP();
    static QString createPatientMessage(Patient *patient);
};

#endif // WXP_H
