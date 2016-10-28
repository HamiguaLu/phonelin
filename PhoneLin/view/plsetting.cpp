#include "plsetting.h"
#include "ui_plsetting.h"
#include <QFileDialog>
#include <QSettings>

PlSetting::PlSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlSetting)
{
    ui->setupUi(this);

    QSettings settings("PhoneLinSettings.ini", QSettings::IniFormat);

    ui->syncFolder->setText(settings.value("mediaSyncFolder","").toString());
    ui->cbAutoSyncMedia->setChecked(settings.value("autoSyncMedia",false).toBool());

}

PlSetting::~PlSetting()
{
    delete ui;
}


void PlSetting::on_btnChooseSyncFolder_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this);
    ui->syncFolder->setText(dir);
}

void PlSetting::on_buttonBox_accepted()
{
    QSettings settings("PhoneLinSettings.ini", QSettings::IniFormat);

    settings.setValue("mediaSyncFolder", ui->syncFolder->text());
    settings.setValue("autoSyncMedia", ui->cbAutoSyncMedia->isChecked());

    accept();
}
