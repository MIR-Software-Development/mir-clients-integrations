#ifndef HL7_H
#define HL7_H

#include <QString>
#include "Patient.h"


class HL7
{
public:
    HL7();
    static QString createPatientMessage(Patient *patient);
};

#endif // HL7_H
