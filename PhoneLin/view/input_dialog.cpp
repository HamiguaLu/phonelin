

#include "input_dialog.h"

InputDialog::InputDialog(QWidget *parent)
    :QDialog(parent)
{
    label = new QLabel(tr("New Name"));
    lineEdit = new QLineEdit;
    label->setBuddy(lineEdit);

    findButton = new QPushButton(tr("OK"));
    findButton->setDefault(true);
    findButton->setEnabled(false);

    closeButton = new QPushButton(tr("Cancel"));

    connect(lineEdit,SIGNAL(textChanged(const QString &)),this,SLOT(enableFindButton(const QString &)));
    connect(findButton,SIGNAL(clicked()),this,SLOT(findClicked()));
    connect(closeButton,SIGNAL(clicked()),this,SLOT(close()));


	QHBoxLayout *topLeftLayout = new QHBoxLayout;
    topLeftLayout->addWidget(label);
    topLeftLayout->addWidget(lineEdit);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(findButton);
    rightLayout->addWidget(closeButton);
    rightLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLeftLayout);
    mainLayout->addLayout(rightLayout);

    setLayout(mainLayout);
    setWindowTitle(tr("New"));
    setFixedHeight(sizeHint().height());
}


void InputDialog::setInfo(QString info)
{
	label->setText(info);
}

void InputDialog::setTitle(QString title)
{
	setWindowTitle(title);
}



void InputDialog::findClicked()
{
    m_newFileName = lineEdit->text();
    if (m_newFileName.isEmpty())
    {
    	return;
    }

	close();

}

void InputDialog::enableFindButton(const QString &text)
{
    findButton->setEnabled(!text.isEmpty());
}
