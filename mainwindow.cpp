#include "mainwindow.h"
#include "addbookdialog.h"
#include "adduserdialog.h"
#include "borrowdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QGroupBox>
#include <QSplitter>
#include <QFont>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("📚 Library Management System");
    setMinimumSize(1000, 650);

    if (!DatabaseManager::instance().init()) {
        QMessageBox::critical(this, "Database Error",
                              "Could not open the database.");
    }

    setupUI();
    applyStyles();

    onRefreshBooks();
    onRefreshUsers();
    onRefreshBorrows();
}

MainWindow::~MainWindow() {}

// ── UI Setup ──────────────────────────────────────────────────────────────────

void MainWindow::setupUI()
{
    m_tabs = new QTabWidget(this);
    m_tabs->setTabPosition(QTabWidget::North);

    setupBooksTab();
    setupUsersTab();
    setupBorrowsTab();

    // dashboard
    m_totalBooksLabel    = new QLabel("📖 Books: 0");
    m_totalUsersLabel    = new QLabel("👤 Users: 0");
    m_activeBorrowsLabel = new QLabel("🔄 Active Borrows: 0");

    for (QLabel *l : {m_totalBooksLabel, m_totalUsersLabel, m_activeBorrowsLabel})
        l->setStyleSheet(
            "font-weight:bold; font-size:13px; padding:4px 12px; color:#2c3e50;");

    QHBoxLayout *dashLayout = new QHBoxLayout;
    dashLayout->addWidget(m_totalBooksLabel);
    dashLayout->addWidget(m_totalUsersLabel);
    dashLayout->addWidget(m_activeBorrowsLabel);
    dashLayout->addStretch();

    QGroupBox *dashBox = new QGroupBox("Dashboard");
    dashBox->setLayout(dashLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(dashBox);
    mainLayout->addWidget(m_tabs);

    QWidget *central = new QWidget(this);
    central->setLayout(mainLayout);
    setCentralWidget(central);
}

// ── Books Tab ─────────────────────────────────────────────────────────────────

void MainWindow::setupBooksTab()
{
    m_bookSearchEdit = new QLineEdit;
    m_bookSearchEdit->setPlaceholderText(
        "🔍 Search by title, author, ISBN or genre...");
    connect(m_bookSearchEdit, &QLineEdit::textChanged,
            this, &MainWindow::onSearchBooks);

    m_addBookBtn      = new QPushButton("➕ Add Book");
    m_removeBookBtn   = new QPushButton("🗑 Remove");
    m_refreshBooksBtn = new QPushButton("🔄 Refresh");

    connect(m_addBookBtn,      &QPushButton::clicked, this, &MainWindow::onAddBook);
    connect(m_removeBookBtn,   &QPushButton::clicked, this, &MainWindow::onRemoveBook);
    connect(m_refreshBooksBtn, &QPushButton::clicked, this, &MainWindow::onRefreshBooks);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(m_bookSearchEdit);
    btnLayout->addWidget(m_addBookBtn);
    btnLayout->addWidget(m_removeBookBtn);
    btnLayout->addWidget(m_refreshBooksBtn);

    m_booksTable = new QTableWidget(0, 7);
    m_booksTable->setHorizontalHeaderLabels(
        {"ID","Title","Author","ISBN","Genre","Year","Available"});
    m_booksTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_booksTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_booksTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_booksTable->setAlternatingRowColors(true);
    m_booksTable->verticalHeader()->setVisible(false);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(btnLayout);
    layout->addWidget(m_booksTable);

    QWidget *tab = new QWidget;
    tab->setLayout(layout);
    m_tabs->addTab(tab, "📖 Books");
}

// ── Users Tab ─────────────────────────────────────────────────────────────────

void MainWindow::setupUsersTab()
{
    m_userSearchEdit = new QLineEdit;
    m_userSearchEdit->setPlaceholderText(
        "🔍 Search by name, email or phone...");
    connect(m_userSearchEdit, &QLineEdit::textChanged,
            this, &MainWindow::onSearchUsers);

    m_addUserBtn      = new QPushButton("➕ Register User");
    m_removeUserBtn   = new QPushButton("🗑 Remove");
    m_refreshUsersBtn = new QPushButton("🔄 Refresh");

    connect(m_addUserBtn,      &QPushButton::clicked, this, &MainWindow::onAddUser);
    connect(m_removeUserBtn,   &QPushButton::clicked, this, &MainWindow::onRemoveUser);
    connect(m_refreshUsersBtn, &QPushButton::clicked, this, &MainWindow::onRefreshUsers);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(m_userSearchEdit);
    btnLayout->addWidget(m_addUserBtn);
    btnLayout->addWidget(m_removeUserBtn);
    btnLayout->addWidget(m_refreshUsersBtn);

    m_usersTable = new QTableWidget(0, 5);
    m_usersTable->setHorizontalHeaderLabels(
        {"ID","Name","Email","Phone","Member Type"});
    m_usersTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_usersTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_usersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_usersTable->setAlternatingRowColors(true);
    m_usersTable->verticalHeader()->setVisible(false);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(btnLayout);
    layout->addWidget(m_usersTable);

    QWidget *tab = new QWidget;
    tab->setLayout(layout);
    m_tabs->addTab(tab, "👤 Users");
}

// ── Borrows Tab ───────────────────────────────────────────────────────────────

void MainWindow::setupBorrowsTab()
{
    m_issueBookBtn      = new QPushButton("📤 Issue Book");
    m_returnBookBtn     = new QPushButton("📥 Return Book");
    m_refreshBorrowsBtn = new QPushButton("🔄 Refresh");

    connect(m_issueBookBtn,      &QPushButton::clicked,
            this, &MainWindow::onIssueBook);
    connect(m_returnBookBtn,     &QPushButton::clicked,
            this, &MainWindow::onReturnBook);
    connect(m_refreshBorrowsBtn, &QPushButton::clicked,
            this, &MainWindow::onRefreshBorrows);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(m_issueBookBtn);
    btnLayout->addWidget(m_returnBookBtn);
    btnLayout->addWidget(m_refreshBorrowsBtn);
    btnLayout->addStretch();

    // ── Active borrows table (now with Due Date + Fine columns) ───────────────
    m_borrowsTable = new QTableWidget(0, 6);
    m_borrowsTable->setHorizontalHeaderLabels(
        {"Borrow ID","Book","User","Borrow Date","Due Date","Fine (R)"});
    m_borrowsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_borrowsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_borrowsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_borrowsTable->setAlternatingRowColors(true);
    m_borrowsTable->verticalHeader()->setVisible(false);

    // ── History table ─────────────────────────────────────────────────────────
    m_historyTable = new QTableWidget(0, 5);
    m_historyTable->setHorizontalHeaderLabels(
        {"Borrow ID","Book","User","Borrow Date","Return Date"});
    m_historyTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_historyTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_historyTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_historyTable->setAlternatingRowColors(true);
    m_historyTable->verticalHeader()->setVisible(false);

    QGroupBox *activeGroup = new QGroupBox(
        "Active Borrows  —  Loan period: 14 days  |  Fine: R2.00/day overdue");
    QVBoxLayout *activeLayout = new QVBoxLayout(activeGroup);
    activeLayout->addWidget(m_borrowsTable);

    QGroupBox *historyGroup = new QGroupBox("Return History");
    QVBoxLayout *historyLayout = new QVBoxLayout(historyGroup);
    historyLayout->addWidget(m_historyTable);

    QSplitter *splitter = new QSplitter(Qt::Vertical);
    splitter->addWidget(activeGroup);
    splitter->addWidget(historyGroup);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 1);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(btnLayout);
    layout->addWidget(splitter);

    QWidget *tab = new QWidget;
    tab->setLayout(layout);
    m_tabs->addTab(tab, "🔄 Borrowing");
}

// ── Load Data ─────────────────────────────────────────────────────────────────

void MainWindow::loadBooks(const QList<Book> &books)
{
    m_booksTable->setRowCount(0);
    for (const Book &b : books)
    {
        int row = m_booksTable->rowCount();
        m_booksTable->insertRow(row);

        auto item = [](const QString &text) {
            QTableWidgetItem *i = new QTableWidgetItem(text);
            i->setForeground(QColor("#2c3e50"));
            return i;
        };

        m_booksTable->setItem(row, 0, item(QString::number(b.id())));
        m_booksTable->setItem(row, 1, item(b.title()));
        m_booksTable->setItem(row, 2, item(b.author()));
        m_booksTable->setItem(row, 3, item(b.isbn()));
        m_booksTable->setItem(row, 4, item(b.genre()));
        m_booksTable->setItem(row, 5, item(QString::number(b.year())));

        QTableWidgetItem *avail = new QTableWidgetItem(
            b.available() ? "✅ Yes" : "❌ No");
        avail->setForeground(
            b.available() ? QColor("#27ae60") : QColor("#e74c3c"));
        m_booksTable->setItem(row, 6, avail);
    }
    m_totalBooksLabel->setText(
        QString("📖 Books: %1").arg(m_booksTable->rowCount()));
}

void MainWindow::loadUsers(const QList<User> &users)
{
    m_usersTable->setRowCount(0);
    for (const User &u : users)
    {
        int row = m_usersTable->rowCount();
        m_usersTable->insertRow(row);

        auto item = [](const QString &text) {
            QTableWidgetItem *i = new QTableWidgetItem(text);
            i->setForeground(QColor("#2c3e50"));
            return i;
        };

        m_usersTable->setItem(row, 0, item(QString::number(u.id())));
        m_usersTable->setItem(row, 1, item(u.name()));
        m_usersTable->setItem(row, 2, item(u.email()));
        m_usersTable->setItem(row, 3, item(u.phone()));
        m_usersTable->setItem(row, 4, item(u.memberType()));
    }
    m_totalUsersLabel->setText(
        QString("👤 Users: %1").arg(m_usersTable->rowCount()));
}

void MainWindow::loadBorrows()
{
    // ── Active borrows with fines ─────────────────────────────────────────────
    m_borrowsTable->setRowCount(0);
    auto active = DatabaseManager::instance().getActiveBorrowsWithFines();

    for (const auto &r : active)
    {
        int row = m_borrowsTable->rowCount();
        m_borrowsTable->insertRow(row);

        auto item = [](const QString &text) {
            QTableWidgetItem *i = new QTableWidgetItem(text);
            i->setForeground(QColor("#2c3e50"));
            return i;
        };

        m_borrowsTable->setItem(row, 0, item(r["id"]));
        m_borrowsTable->setItem(row, 1, item(r["book"]));
        m_borrowsTable->setItem(row, 2, item(r["user"]));
        m_borrowsTable->setItem(row, 3, item(r["borrow_date"]));
        m_borrowsTable->setItem(row, 4, item(r["due_date"]));

        // fine cell
        QTableWidgetItem *fineItem = new QTableWidgetItem(r["fine"]);
        if (r["overdue"] == "Yes")
        {
            fineItem->setForeground(QColor("#e74c3c"));
            fineItem->setFont(QFont("Arial", 10, QFont::Bold));

            // highlight entire overdue row light red
            for (int col = 0; col < 5; ++col)
                if (m_borrowsTable->item(row, col))
                    m_borrowsTable->item(row, col)
                        ->setBackground(QColor(255, 235, 235));
            fineItem->setBackground(QColor(255, 235, 235));
        }
        else
        {
            fineItem->setForeground(QColor("#27ae60"));
        }
        m_borrowsTable->setItem(row, 5, fineItem);
    }

    // ── History ───────────────────────────────────────────────────────────────
    m_historyTable->setRowCount(0);
    auto history = DatabaseManager::instance().getBorrowHistory();
    for (const auto &r : history)
    {
        int row = m_historyTable->rowCount();
        m_historyTable->insertRow(row);

        auto item = [](const QString &text) {
            QTableWidgetItem *i = new QTableWidgetItem(text);
            i->setForeground(QColor("#2c3e50"));
            return i;
        };

        m_historyTable->setItem(row, 0, item(r["id"]));
        m_historyTable->setItem(row, 1, item(r["book"]));
        m_historyTable->setItem(row, 2, item(r["user"]));
        m_historyTable->setItem(row, 3, item(r["borrow_date"]));
        m_historyTable->setItem(row, 4, item(r["return_date"]));
    }

    m_activeBorrowsLabel->setText(
        QString("🔄 Active Borrows: %1").arg(active.size()));
}

// ── Slots ─────────────────────────────────────────────────────────────────────

void MainWindow::onAddBook()
{
    AddBookDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted)
    {
        if (DatabaseManager::instance().addBook(dlg.getBook()))
            onRefreshBooks();
        else
            QMessageBox::warning(this, "Error", "Failed to add book.");
    }
}

void MainWindow::onRemoveBook()
{
    int row = m_booksTable->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Remove", "Please select a book first.");
        return;
    }
    int id = m_booksTable->item(row, 0)->text().toInt();
    auto r = QMessageBox::question(this, "Confirm",
                                   "Are you sure you want to remove this book?");
    if (r == QMessageBox::Yes)
    {
        DatabaseManager::instance().removeBook(id);
        onRefreshBooks();
    }
}

void MainWindow::onSearchBooks()
{
    QString kw = m_bookSearchEdit->text().trimmed();
    if (kw.isEmpty())
        loadBooks(DatabaseManager::instance().getAllBooks());
    else
        loadBooks(DatabaseManager::instance().searchBooks(kw));
}

void MainWindow::onRefreshBooks()
{
    loadBooks(DatabaseManager::instance().getAllBooks());
}

void MainWindow::onAddUser()
{
    AddUserDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted)
    {
        if (DatabaseManager::instance().addUser(dlg.getUser()))
            onRefreshUsers();
        else
            QMessageBox::warning(this, "Error", "Failed to register user.");
    }
}

void MainWindow::onRemoveUser()
{
    int row = m_usersTable->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Remove", "Please select a user first.");
        return;
    }
    int id = m_usersTable->item(row, 0)->text().toInt();
    auto r = QMessageBox::question(this, "Confirm",
                                   "Are you sure you want to remove this user?");
    if (r == QMessageBox::Yes)
    {
        DatabaseManager::instance().removeUser(id);
        onRefreshUsers();
    }
}

void MainWindow::onSearchUsers()
{
    QString kw = m_userSearchEdit->text().trimmed();
    if (kw.isEmpty())
        loadUsers(DatabaseManager::instance().getAllUsers());
    else
        loadUsers(DatabaseManager::instance().searchUsers(kw));
}

void MainWindow::onRefreshUsers()
{
    loadUsers(DatabaseManager::instance().getAllUsers());
}

void MainWindow::onIssueBook()
{
    BorrowDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted)
    {
        if (DatabaseManager::instance().borrowBook(
                dlg.selectedBookId(), dlg.selectedUserId()))
        {
            onRefreshBooks();
            onRefreshBorrows();
            QMessageBox::information(this, "Success",
                                     "Book issued successfully!\n"
                                     "Due in 14 days.");
        }
        else
            QMessageBox::warning(this, "Error", "Failed to issue book.");
    }
}

void MainWindow::onReturnBook()
{
    int row = m_borrowsTable->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Return",
                                 "Please select an active borrow to return.");
        return;
    }

    int    borrowId = m_borrowsTable->item(row, 0)->text().toInt();
    QString fine    = m_borrowsTable->item(row, 5)->text();
    QString book    = m_borrowsTable->item(row, 1)->text();

    // warn about outstanding fine before returning
    if (fine != "R 0.00")
    {
        auto answer = QMessageBox::warning(this, "Outstanding Fine",
                                           QString("Book: %1\nOutstanding fine: %2\n\n"
                                                   "Confirm return and clear fine?").arg(book).arg(fine),
                                           QMessageBox::Yes | QMessageBox::No);
        if (answer != QMessageBox::Yes)
            return;
    }

    if (DatabaseManager::instance().returnBook(borrowId))
    {
        onRefreshBooks();
        onRefreshBorrows();
        QMessageBox::information(this, "Success", "Book returned successfully!");
    }
    else
        QMessageBox::warning(this, "Error", "Failed to return book.");
}

void MainWindow::onRefreshBorrows()
{
    loadBorrows();
}

// ── Styles ────────────────────────────────────────────────────────────────────

void MainWindow::applyStyles()
{
    setStyleSheet(R"(
        QMainWindow { background: #f0f2f5; }

        QTabWidget::pane { border: 1px solid #ddd; background: white; }
        QTabBar::tab {
            padding: 8px 20px; font-size: 13px;
            background: #dfe6e9; border-radius: 4px 4px 0 0;
            color: #2c3e50;
        }
        QTabBar::tab:selected {
            background: white; font-weight: bold; color: #2c3e50;
        }

        QTableWidget {
            border: 1px solid #ddd;
            gridline-color: #eee;
            font-size: 12px;
            color: #2c3e50;
            background: white;
        }
        QTableWidget::item { color: #2c3e50; background: white; }
        QTableWidget::item:alternate { background: #f8f9fa; color: #2c3e50; }
        QTableWidget::item:selected { background: #3498db; color: white; }
        QHeaderView::section {
            background: #2c3e50; color: white;
            padding: 6px; font-weight: bold;
        }

        QPushButton {
            background: #3498db; color: white;
            border-radius: 4px; padding: 6px 14px;
            font-size: 12px;
        }
        QPushButton:hover { background: #2980b9; }

        QLineEdit {
            border: 1px solid #ddd; border-radius: 4px;
            padding: 6px; font-size: 12px;
            color: #2c3e50; background: white;
        }
        QLineEdit:focus { border-color: #3498db; }

        QGroupBox {
            font-weight: bold; font-size: 12px;
            border: 1px solid #ddd; border-radius: 6px;
            margin-top: 8px; padding-top: 8px;
            color: #2c3e50;
        }

        QLabel { color: #2c3e50; }
    )");

    m_addBookBtn->setStyleSheet(
        "QPushButton{background:#2ecc71;color:white;"
        "border-radius:4px;padding:6px 14px;}"
        "QPushButton:hover{background:#27ae60;}");
    m_removeBookBtn->setStyleSheet(
        "QPushButton{background:#e74c3c;color:white;"
        "border-radius:4px;padding:6px 14px;}"
        "QPushButton:hover{background:#c0392b;}");
    m_addUserBtn->setStyleSheet(
        "QPushButton{background:#2ecc71;color:white;"
        "border-radius:4px;padding:6px 14px;}"
        "QPushButton:hover{background:#27ae60;}");
    m_removeUserBtn->setStyleSheet(
        "QPushButton{background:#e74c3c;color:white;"
        "border-radius:4px;padding:6px 14px;}"
        "QPushButton:hover{background:#c0392b;}");
    m_issueBookBtn->setStyleSheet(
        "QPushButton{background:#3498db;color:white;"
        "border-radius:4px;padding:6px 14px;}"
        "QPushButton:hover{background:#2980b9;}");
    m_returnBookBtn->setStyleSheet(
        "QPushButton{background:#9b59b6;color:white;"
        "border-radius:4px;padding:6px 14px;}"
        "QPushButton:hover{background:#8e44ad;}");
}