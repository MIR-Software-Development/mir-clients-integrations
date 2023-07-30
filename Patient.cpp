#include "Patient.h"
#include <QString>
#include <QStringList>

///
/// Init a patient instance.
/// \brief Patient::Patient
/// \param id
/// \param firstName
/// \param lastName
/// \param birthDate
/// \param gender
/// \param height
/// \param weight
/// \param ethnicGroup
/// \param notes
///
Patient::Patient(QString id, QString firstName, QString lastName, QString birthDate, int gender, int height, int weight, int ethnicGroup, QString notes)
{
    this->id = id;
    this->firstName = firstName;
    this->lastName = lastName;
    this->birthDate = birthDate;
    this->gender = gender;
    this->height = height;
    this->weight = weight;
    this->ethnicGroup = ethnicGroup;
    this->notes = notes;
}

///
/// Format the birth date of the patient with the following format: YYMMDD.
/// \brief Patient::getBirthDateToUsFormat
/// \return
///
QString Patient::getBirthDateToUsFormat()
{
    QStringList elements = this->birthDate.split("/");

    return elements[2] + elements[1] + elements[0];
}
