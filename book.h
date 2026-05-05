#ifndef BOOK_H
#define BOOK_H

#include <QString>

class Book
{
public:
    Book();
    Book(int id, const QString &title, const QString &author,
         const QString &isbn, const QString &genre, int year, bool available);

    // Getters
    int     id()        const;
    QString title()     const;
    QString author()    const;
    QString isbn()      const;
    QString genre()     const;
    int     year()      const;
    bool    available() const;

    // Setters
    void setId(int id);
    void setTitle(const QString &title);
    void setAuthor(const QString &author);
    void setIsbn(const QString &isbn);
    void setGenre(const QString &genre);
    void setYear(int year);
    void setAvailable(bool available);

private:
    int     m_id;
    QString m_title;
    QString m_author;
    QString m_isbn;
    QString m_genre;
    int     m_year;
    bool    m_available;
};

#endif // BOOK_H