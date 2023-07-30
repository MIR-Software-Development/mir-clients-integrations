#include "WXP.h"

#include <QDateTime>
#include <QString>

WXP::WXP()
{

}

QString WXP::createPatientMessage(Patient *patient)
{
    QString content = "[Identification]\r\n";

    content += "ID=" + patient->id + "\r\n" +
           "LastName=" + patient->firstName + "\r\n" +
           "FirstName=" + patient->lastName + "\r\n" +
           "BirthDate=" + patient->birthDate + "\r\n" +
           "Gender=" + QString::number(patient->gender) + "\r\n" +
           "EthnicGroup=" + QString::number(patient->ethnicGroup) + "\r\n" +
           "Height=" + QString::number(patient->height) + "\r\n" +
           "Weight=" + QString::number(patient->weight);

    return content;
}

QString WXP::createReadSessionMessage(Patient *patient, QString sessionDate, QString sessionTime)
{
    QString content = "[Identification]\r\n";

    content += "ID="+ patient->id + "\r\n" +
        "LastName=" + patient->lastName + "\r\n" +
            "FirstName="+ patient->firstName + "\r\n" +
            "BirthDate="+ patient->birthDate + "\r\n" +
          "SessionDate=" + sessionDate + "\r\n" +
               "SessionTime=" + sessionTime;

    return content;
}
