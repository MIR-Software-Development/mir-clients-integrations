#ifndef PATIENT_H
#define PATIENT_H

#include <QString>

class Patient
{
public:
    Patient(QString id, QString firstName, QString lastName, QString birthDate, int gender, int height, int weight, int ethnicGroup, QString notes = "");

    QString id;
    QString firstName;
    QString lastName;
    QString birthDate;
    int gender;
    int height;
    int weight;
    int ethnicGroup;
    QString notes;

    QString getBirthDateToUsFormat();
};

#endif // PATIENT_H
