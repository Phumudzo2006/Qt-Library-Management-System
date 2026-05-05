#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include "databasemanager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Books
    void onAddBook();
    void onRemoveBook();
    void onSearchBooks();
    void onRefreshBooks();

    // Users
    void onAddUser();
    void onRemoveUser();
    void onSearchUsers();
    void onRefreshUsers();

    // Borrows
    void onIssueBook();
    void onReturnBook();
    void onRefreshBorrows();

private:
    void setupUI();
    void setupBooksTab();
    void setupUsersTab();
    void setupBorrowsTab();
    void applyStyles();

    void loadBooks(const QList<Book> &books);
    void loadUsers(const QList<User> &users);
    void loadBorrows();

    // main container
    QTabWidget *m_tabs;

    // ── Books tab ─────────────────────────────────────────────────────────────
    QTableWidget *m_booksTable;
    QLineEdit    *m_bookSearchEdit;
    QPushButton  *m_addBookBtn;
    QPushButton  *m_removeBookBtn;
    QPushButton  *m_refreshBooksBtn;

    // ── Users tab ─────────────────────────────────────────────────────────────
    QTableWidget *m_usersTable;
    QLineEdit    *m_userSearchEdit;
    QPushButton  *m_addUserBtn;
    QPushButton  *m_removeUserBtn;
    QPushButton  *m_refreshUsersBtn;

    // ── Borrows tab ───────────────────────────────────────────────────────────
    QTableWidget *m_borrowsTable;
    QTableWidget *m_historyTable;
    QPushButton  *m_issueBookBtn;
    QPushButton  *m_returnBookBtn;
    QPushButton  *m_refreshBorrowsBtn;

    // dashboard labels
    QLabel *m_totalBooksLabel;
    QLabel *m_totalUsersLabel;
    QLabel *m_activeBorrowsLabel;
};

#endif // MAINWINDOW_H