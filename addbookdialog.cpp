#include "addbookdialog.h"
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QDate>

AddBookDialog::AddBookDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Add New Book");
    setMinimumWidth(350);

    m_titleEdit  = new QLineEdit;
    m_authorEdit = new QLineEdit;
    m_isbnEdit   = new QLineEdit;
    m_isbnEdit->setPlaceholderText("e.g. 978-3-16-148410-0");

    m_genreCombo = new QComboBox;
    m_genreCombo->addItems({"Fiction", "Non-Fiction", "Science",
                            "History", "Technology", "Biography",
                            "Children", "Other"});

    m_yearSpin = new QSpinBox;
    m_yearSpin->setRange(1000, QDate::currentDate().year());
    m_yearSpin->setValue(QDate::currentDate().year());

    m_addBtn    = new QPushButton("Add Book");
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
    form->addRow("Title*:",  m_titleEdit);
    form->addRow("Author*:", m_authorEdit);
    form->addRow("ISBN:",    m_isbnEdit);
    form->addRow("Genre:",   m_genreCombo);
    form->addRow("Year:",    m_yearSpin);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(m_addBtn);
    btnLayout->addWidget(m_cancelBtn);

    QVBoxLayout *main = new QVBoxLayout(this);
    main->addLayout(form);
    main->addSpacing(10);
    main->addLayout(btnLayout);

    connect(m_addBtn,    &QPushButton::clicked, this, &AddBookDialog::onAccept);
    connect(m_cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
}

void AddBookDialog::onAccept()
{
    if (m_titleEdit->text().trimmed().isEmpty() ||
        m_authorEdit->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, "Missing Data",
                             "Title and Author are required.");
        return;
    }
    accept();
}

Book AddBookDialog::getBook() const
{
    return Book(0,
                m_titleEdit->text().trimmed(),
                m_authorEdit->text().trimmed(),
                m_isbnEdit->text().trimmed(),
                m_genreCombo->currentText(),
                m_yearSpin->value(),
                true);
}