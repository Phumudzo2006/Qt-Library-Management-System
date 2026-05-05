#include "adduserdialog.h"
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

AddUserDialog::AddUserDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Register New User");
    setMinimumWidth(350);

    m_nameEdit  = new QLineEdit;
    m_emailEdit = new QLineEdit;
    m_emailEdit->setPlaceholderText("example@email.com");
    m_phoneEdit = new QLineEdit;
    m_phoneEdit->setPlaceholderText("e.g. 0812345678");

    m_typeCombo = new QComboBox;
    m_typeCombo->addItems({"Student", "Staff", "External"});

    m_addBtn    = new QPushButton("Register");
    m_cancelBtn = new QPushButton("Cancel");

    m_addBtn->setStyleSheet(
        "QPushButton { background:#2ecc71; color:white; "
        "border-radius:4px; padding:6px 16px; }"
        "QPushButton:hover { background:#27ae60; }");
    m_cancelBtn->setStyleSheet(
        "QPushButton { background:#e74c3c; color:white; "
        "border-radius:4px; padding:6px 16px; }"
        "QPushButton:hover { background:#c0392b; }");

    QFormLayout *form = new QFormLayout;
    form->addRow("Name*:",       m_nameEdit);
    form->addRow("Email:",       m_emailEdit);
    form->addRow("Phone:",       m_phoneEdit);
    form->addRow("Member Type:", m_typeCombo);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(m_addBtn);
    btnLayout->addWidget(m_cancelBtn);

    QVBoxLayout *main = new QVBoxLayout(this);
    main->addLayout(form);
    main->addSpacing(10);
    main->addLayout(btnLayout);

    connect(m_addBtn,    &QPushButton::clicked, this, &AddUserDialog::onAccept);
    connect(m_cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
}

void AddUserDialog::onAccept()
{
    if (m_nameEdit->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, "Missing Data", "Name is required.");
        return;
    }
    accept();
}

User AddUserDialog::getUser() const
{
    return User(0,
                m_nameEdit->text().trimmed(),
                m_emailEdit->text().trimmed(),
                m_phoneEdit->text().trimmed(),
                m_typeCombo->currentText());
}