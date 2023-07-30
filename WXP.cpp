#include "WXP.h"

#include <QDateTime>
#include <QString>

WXP::WXP()
{

}

///
/// Create the Patient.SRV content to create a patient and start some tests.
/// \brief WXP::createPatientMessage
/// \param patient
/// \return
///
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

///
/// Create the Patient.SRV file to read the session of a patient.
/// \brief WXP::createReadSessionMessage
/// \param patient
/// \param sessionDate
/// \param sessionTime
/// \return
///
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
