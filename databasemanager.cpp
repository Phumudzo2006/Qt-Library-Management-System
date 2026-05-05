#include "databasemanager.h"
#include <QDate>

DatabaseManager &DatabaseManager::instance()
{
    static DatabaseManager instance;
    return instance;
}

DatabaseManager::DatabaseManager()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("library.db");
}

bool DatabaseManager::init()
{
    if (!m_db.open()) {
        qDebug() << "DB Error:" << m_db.lastError().text();
        return false;
    }
    createTables();
    return true;
}

void DatabaseManager::createTables()
{
    QSqlQuery q;

    q.exec(R"(CREATE TABLE IF NOT EXISTS books (
        id        INTEGER PRIMARY KEY AUTOINCREMENT,
        title     TEXT NOT NULL,
        author    TEXT NOT NULL,
        isbn      TEXT,
        genre     TEXT,
        year      INTEGER,
        available INTEGER DEFAULT 1
    ))");

    q.exec(R"(CREATE TABLE IF NOT EXISTS users (
        id          INTEGER PRIMARY KEY AUTOINCREMENT,
        name        TEXT NOT NULL,
        email       TEXT,
        phone       TEXT,
        member_type TEXT DEFAULT 'Student'
    ))");

    q.exec(R"(CREATE TABLE IF NOT EXISTS borrows (
        id          INTEGER PRIMARY KEY AUTOINCREMENT,
        book_id     INTEGER,
        user_id     INTEGER,
        borrow_date TEXT,
        return_date TEXT,
        returned    INTEGER DEFAULT 0,
        FOREIGN KEY(book_id) REFERENCES books(id),
        FOREIGN KEY(user_id) REFERENCES users(id)
    ))");
}

// ── Books ─────────────────────────────────────────────────────────────────────

bool DatabaseManager::addBook(const Book &book)
{
    QSqlQuery q;
    q.prepare("INSERT INTO books (title,author,isbn,genre,year,available) "
              "VALUES (:t,:a,:i,:g,:y,1)");
    q.bindValue(":t", book.title());
    q.bindValue(":a", book.author());
    q.bindValue(":i", book.isbn());
    q.bindValue(":g", book.genre());
    q.bindValue(":y", book.year());
    return q.exec();
}

bool DatabaseManager::removeBook(int bookId)
{
    QSqlQuery q;
    q.prepare("DELETE FROM books WHERE id=:id");
    q.bindValue(":id", bookId);
    return q.exec();
}

bool DatabaseManager::updateBookAvailability(int bookId, bool available)
{
    QSqlQuery q;
    q.prepare("UPDATE books SET available=:a WHERE id=:id");
    q.bindValue(":a",  available ? 1 : 0);
    q.bindValue(":id", bookId);
    return q.exec();
}

QList<Book> DatabaseManager::getAllBooks()
{
    QList<Book> books;
    QSqlQuery q("SELECT id,title,author,isbn,genre,year,available FROM books");
    while (q.next())
        books << Book(q.value(0).toInt(), q.value(1).toString(),
                      q.value(2).toString(), q.value(3).toString(),
                      q.value(4).toString(), q.value(5).toInt(),
                      q.value(6).toBool());
    return books;
}

QList<Book> DatabaseManager::searchBooks(const QString &keyword)
{
    QList<Book> books;
    QSqlQuery q;
    q.prepare("SELECT id,title,author,isbn,genre,year,available FROM books "
              "WHERE title LIKE :k OR author LIKE :k OR isbn LIKE :k OR genre LIKE :k");
    q.bindValue(":k", "%" + keyword + "%");
    q.exec();
    while (q.next())
        books << Book(q.value(0).toInt(), q.value(1).toString(),
                      q.value(2).toString(), q.value(3).toString(),
                      q.value(4).toString(), q.value(5).toInt(),
                      q.value(6).toBool());
    return books;
}

// ── Users ─────────────────────────────────────────────────────────────────────

bool DatabaseManager::addUser(const User &user)
{
    QSqlQuery q;
    q.prepare("INSERT INTO users (name,email,phone,member_type) "
              "VALUES (:n,:e,:p,:m)");
    q.bindValue(":n", user.name());
    q.bindValue(":e", user.email());
    q.bindValue(":p", user.phone());
    q.bindValue(":m", user.memberType());
    return q.exec();
}

bool DatabaseManager::removeUser(int userId)
{
    QSqlQuery q;
    q.prepare("DELETE FROM users WHERE id=:id");
    q.bindValue(":id", userId);
    return q.exec();
}

QList<User> DatabaseManager::getAllUsers()
{
    QList<User> users;
    QSqlQuery q("SELECT id,name,email,phone,member_type FROM users");
    while (q.next())
        users << User(q.value(0).toInt(), q.value(1).toString(),
                      q.value(2).toString(), q.value(3).toString(),
                      q.value(4).toString());
    return users;
}

QList<User> DatabaseManager::searchUsers(const QString &keyword)
{
    QList<User> users;
    QSqlQuery q;
    q.prepare("SELECT id,name,email,phone,member_type FROM users "
              "WHERE name LIKE :k OR email LIKE :k OR phone LIKE :k");
    q.bindValue(":k", "%" + keyword + "%");
    q.exec();
    while (q.next())
        users << User(q.value(0).toInt(), q.value(1).toString(),
                      q.value(2).toString(), q.value(3).toString(),
                      q.value(4).toString());
    return users;
}

// ── Borrows ───────────────────────────────────────────────────────────────────

bool DatabaseManager::borrowBook(int bookId, int userId)
{
    QSqlQuery q;
    q.prepare("INSERT INTO borrows (book_id,user_id,borrow_date,returned) "
              "VALUES (:b,:u,:d,0)");
    q.bindValue(":b", bookId);
    q.bindValue(":u", userId);
    q.bindValue(":d", QDate::currentDate().toString("yyyy-MM-dd"));
    if (!q.exec()) return false;
    return updateBookAvailability(bookId, false);
}

bool DatabaseManager::returnBook(int borrowId)
{
    QSqlQuery q;
    q.prepare("SELECT book_id FROM borrows WHERE id=:id");
    q.bindValue(":id", borrowId);
    q.exec();
    if (!q.next()) return false;
    int bookId = q.value(0).toInt();

    q.prepare("UPDATE borrows SET returned=1, return_date=:d WHERE id=:id");
    q.bindValue(":d",  QDate::currentDate().toString("yyyy-MM-dd"));
    q.bindValue(":id", borrowId);
    if (!q.exec()) return false;
    return updateBookAvailability(bookId, true);
}

QList<QMap<QString,QString>> DatabaseManager::getActiveBorrows()
{
    QList<QMap<QString,QString>> list;
    QSqlQuery q(
        "SELECT br.id, b.title, u.name, br.borrow_date "
        "FROM borrows br "
        "JOIN books b ON br.book_id = b.id "
        "JOIN users u ON br.user_id = u.id "
        "WHERE br.returned = 0");
    while (q.next()) {
        QMap<QString,QString> row;
        row["id"]          = q.value(0).toString();
        row["book"]        = q.value(1).toString();
        row["user"]        = q.value(2).toString();
        row["borrow_date"] = q.value(3).toString();
        list << row;
    }
    return list;
}

QList<QMap<QString,QString>> DatabaseManager::getBorrowHistory()
{
    QList<QMap<QString,QString>> list;
    QSqlQuery q(
        "SELECT br.id, b.title, u.name, br.borrow_date, br.return_date "
        "FROM borrows br "
        "JOIN books b ON br.book_id = b.id "
        "JOIN users u ON br.user_id = u.id "
        "WHERE br.returned = 1");
    while (q.next()) {
        QMap<QString,QString> row;
        row["id"]          = q.value(0).toString();
        row["book"]        = q.value(1).toString();
        row["user"]        = q.value(2).toString();
        row["borrow_date"] = q.value(3).toString();
        row["return_date"] = q.value(4).toString();
        list << row;
    }
    return list;
}

// ── Fines ─────────────────────────────────────────────────────────────────────

double DatabaseManager::calculateFine(const QString &borrowDate)
{
    QDate borrow = QDate::fromString(borrowDate, "yyyy-MM-dd");
    QDate due    = borrow.addDays(14);  // 14 day loan period
    QDate today  = QDate::currentDate();

    if (today <= due)
        return 0.0;  // not overdue

    int daysOverdue = due.daysTo(today);
    return daysOverdue * 2.0;  // R2.00 per day
}

QList<QMap<QString,QString>> DatabaseManager::getActiveBorrowsWithFines()
{
    QList<QMap<QString,QString>> list;
    QSqlQuery q(
        "SELECT br.id, b.title, u.name, br.borrow_date "
        "FROM borrows br "
        "JOIN books b ON br.book_id = b.id "
        "JOIN users u ON br.user_id = u.id "
        "WHERE br.returned = 0");

    while (q.next()) {
        QMap<QString,QString> row;
        QString borrowDate = q.value(3).toString();
        double fine        = calculateFine(borrowDate);
        QDate  due         = QDate::fromString(borrowDate, "yyyy-MM-dd").addDays(14);

        row["id"]          = q.value(0).toString();
        row["book"]        = q.value(1).toString();
        row["user"]        = q.value(2).toString();
        row["borrow_date"] = borrowDate;
        row["due_date"]    = due.toString("yyyy-MM-dd");
        row["fine"]        = QString("R %1").arg(QString::number(fine, 'f', 2));
        row["overdue"]     = fine > 0 ? "Yes" : "No";
        list << row;
    }
    return list;
}