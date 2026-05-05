#ifndef ADDUSERDIALOG_H
#define ADDUSERDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include "user.h"

class AddUserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddUserDialog(QWidget *parent = nullptr);
    User getUser() const;

private slots:
    void onAccept();

private:
    QLineEdit   *m_nameEdit;
    QLineEdit   *m_emailEdit;
    QLineEdit   *m_phoneEdit;
    QComboBox   *m_typeCombo;
    QPushButton *m_addBtn;
    QPushButton *m_cancelBtn;
};

#endif // ADDUSERDIALOG_H