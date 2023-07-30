#ifndef WXP_H
#define WXP_H

#include "Patient.h"

class WXP
{
public:
    WXP();
    static QString createPatientMessage(Patient *patient);
    static QString createReadSessionMessage(Patient *patient, QString sessionDate, QString sessionTime);
};

#endif // WXP_H
