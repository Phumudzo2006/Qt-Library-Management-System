#ifndef ADDBOOKDIALOG_H
#define ADDBOOKDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include "book.h"

class AddBookDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddBookDialog(QWidget *parent = nullptr);
    Book getBook() const;

private slots:
    void onAccept();

private:
    QLineEdit *m_titleEdit;
    QLineEdit *m_authorEdit;
    QLineEdit *m_isbnEdit;
    QComboBox *m_genreCombo;
    QSpinBox  *m_yearSpin;
    QPushButton *m_addBtn;
    QPushButton *m_cancelBtn;
};

#endif // ADDBOOKDIALOG_H