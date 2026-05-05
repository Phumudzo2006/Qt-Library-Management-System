#ifndef USER_H
#define USER_H

#include <QString>

class User
{
public:
    User();
    User(int id, const QString &name, const QString &email,
         const QString &phone, const QString &memberType);

    // Getters
    int     id()         const;
    QString name()       const;
    QString email()      const;
    QString phone()      const;
    QString memberType() const;

    // Setters
    void setId(int id);
    void setName(const QString &name);
    void setEmail(const QString &email);
    void setPhone(const QString &phone);
    void setMemberType(const QString &memberType);

private:
    int     m_id;
    QString m_name;
    QString m_email;
    QString m_phone;
    QString m_memberType;
};

#endif // USER_H