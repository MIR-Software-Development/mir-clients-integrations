#include "HL7.h"

#include <QDateTime>

HL7::HL7()
{

}

QString HL7::createPatientMessage(Patient *patient)
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString currentDate = currentDateTime.toString("yyyyMMddhhmmss");

    QString hl7 = "MSH|^~\\&|HL7emulator||winspiroPRO||" + currentDate + "|| ORM^O01||D|2.1\r\n" +
                  "PID|1|" + patient->id + "| " + patient->id + "||" +
                  patient->firstName + "^" + patient->lastName + "||" +
                  patient->getBirthDateToUsFormat() + "|" + QString::number(patient->gender) + "||" +
                  QString::number(patient->ethnicGroup) + "||||||||" + patient->id + "\r\n" +
                  "ORC|NW|123456||||N\r\nOBR|1|123456||Spirometry||" + currentDate + "||||||||||^" + patient->firstName + "^" + patient->lastName + "^^^^|||||||||||^^^^^R\r\n" +
                  "OBX|1|NM|H||" + QString::number(patient->height) + "|CM|||||||||||\r\n" +
                  "OBX|2|NM|W||" + QString::number(patient->weight) + "|KG|||||||||||";

    if (!patient->notes.isEmpty())
    {
        hl7 += "\r\nNTE|1||" + patient->notes;
    }

    return hl7;
}
