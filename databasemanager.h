#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QList>
#include "book.h"
#include "user.h"

class DatabaseManager
{
public:
    static DatabaseManager &instance();

    bool init();

    // Book operations
    bool        addBook(const Book &book);
    bool        removeBook(int bookId);
    bool        updateBookAvailability(int bookId, bool available);
    QList<Book> getAllBooks();
    QList<Book> searchBooks(const QString &keyword);

    // User operations
    bool        addUser(const User &user);
    bool        removeUser(int userId);
    QList<User> getAllUsers();
    QList<User> searchUsers(const QString &keyword);

    // Borrow operations
    bool   borrowBook(int bookId, int userId);
    bool   returnBook(int borrowId);
    QList<QMap<QString,QString>> getActiveBorrows();
    QList<QMap<QString,QString>> getBorrowHistory();

    // Fine operations
    double calculateFine(const QString &borrowDate);
    QList<QMap<QString,QString>> getActiveBorrowsWithFines();

private:
    DatabaseManager();
    void createTables();
    QSqlDatabase m_db;
};

#endif // DATABASEMANAGER_H