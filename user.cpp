#include "user.h"

User::User()
    : m_id(0), m_name(""), m_email(""),
    m_phone(""), m_memberType("Student")
{}

User::User(int id, const QString &name, const QString &email,
           const QString &phone, const QString &memberType)
    : m_id(id), m_name(name), m_email(email),
    m_phone(phone), m_memberType(memberType)
{}

int     User::id()         const { return m_id; }
QString User::name()       const { return m_name; }
QString User::email()      const { return m_email; }
QString User::phone()      const { return m_phone; }
QString User::memberType() const { return m_memberType; }

void User::setId(int id)                      { m_id = id; }
void User::setName(const QString &n)          { m_name = n; }
void User::setEmail(const QString &e)         { m_email = e; }
void User::setPhone(const QString &p)         { m_phone = p; }
void User::setMemberType(const QString &m)    { m_memberType = m; }