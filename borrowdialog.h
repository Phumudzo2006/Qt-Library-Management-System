#ifndef BORROWDIALOG_H
#define BORROWDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include "databasemanager.h"

class BorrowDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BorrowDialog(QWidget *parent = nullptr);
    int selectedBookId() const;
    int selectedUserId() const;

private slots:
    void onAccept();

private:
    QComboBox   *m_bookCombo;
    QComboBox   *m_userCombo;
    QPushButton *m_borrowBtn;
    QPushButton *m_cancelBtn;

    QList<Book> m_books;
    QList<User> m_users;
};

#endif // BORROWDIALOG_H