#include "book.h"

Book::Book()
    : m_id(0), m_title(""), m_author(""),
    m_isbn(""), m_genre(""), m_year(2000), m_available(true)
{}

Book::Book(int id, const QString &title, const QString &author,
           const QString &isbn, const QString &genre, int year, bool available)
    : m_id(id), m_title(title), m_author(author),
    m_isbn(isbn), m_genre(genre), m_year(year), m_available(available)
{}

int     Book::id()        const { return m_id; }
QString Book::title()     const { return m_title; }
QString Book::author()    const { return m_author; }
QString Book::isbn()      const { return m_isbn; }
QString Book::genre()     const { return m_genre; }
int     Book::year()      const { return m_year; }
bool    Book::available() const { return m_available; }

void Book::setId(int id)                  { m_id = id; }
void Book::setTitle(const QString &t)     { m_title = t; }
void Book::setAuthor(const QString &a)    { m_author = a; }
void Book::setIsbn(const QString &i)      { m_isbn = i; }
void Book::setGenre(const QString &g)     { m_genre = g; }
void Book::setYear(int y)                 { m_year = y; }
void Book::setAvailable(bool a)           { m_available = a; }