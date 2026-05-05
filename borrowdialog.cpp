#include "borrowdialog.h"
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

BorrowDialog::BorrowDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Issue Book");
    setMinimumWidth(380);

    m_bookCombo = new QComboBox;
    m_userCombo = new QComboBox;

    // load only available books
    m_books = DatabaseManager::instance().getAllBooks();
    for (const Book &b : m_books)
        if (b.available())
            m_bookCombo->addItem(
                QString("[%1] %2 — %3").arg(b.id()).arg(b.title()).arg(b.author()));

    // load all users
    m_users = DatabaseManager::instance().getAllUsers();
    for (const User &u : m_users)
        m_userCombo->addItem(
            QString("[%1] %2 (%3)").arg(u.id()).arg(u.name()).arg(u.memberType()));

    m_borrowBtn = new QPushButton("Issue Book");
    m_cancelBtn = new QPushButton("Cancel");

    m_borrowBtn->setStyleSheet(
        "QPushButton { background:#3498db; color:white; "
        "border-radius:4px; padding:6px 16px; }"
        "QPushButton:hover { background:#2980b9; }");
    m_cancelBtn->setStyleSheet(
        "QPushButton { background:#e74c3c; color:white; "
        "border-radius:4px; padding:6px 16px; }"
        "QPushButton:hover { background:#c0392b; }");

    QFormLayout *form = new QFormLayout;
    form->addRow("Book:",  m_bookCombo);
    form->addRow("User:",  m_userCombo);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(m_borrowBtn);
    btnLayout->addWidget(m_cancelBtn);

    QVBoxLayout *main = new QVBoxLayout(this);
    main->addLayout(form);
    main->addSpacing(10);
    main->addLayout(btnLayout);

    connect(m_borrowBtn, &QPushButton::clicked, this, &BorrowDialog::onAccept);
    connect(m_cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
}

void BorrowDialog::onAccept()
{
    if (m_bookCombo->count() == 0) {
        QMessageBox::warning(this, "No Books",
                             "No available books to issue.");
        return;
    }
    if (m_userCombo->count() == 0) {
        QMessageBox::warning(this, "No Users",
                             "No registered users found.");
        return;
    }
    accept();
}

int BorrowDialog::selectedBookId() const
{
    // extract id from "[id] title" format
    QString text = m_bookCombo->currentText();
    return text.mid(1, text.indexOf(']') - 1).toInt();
}

int BorrowDialog::selectedUserId() const
{
    QString text = m_userCombo->currentText();
    return text.mid(1, text.indexOf(']') - 1).toInt();
}