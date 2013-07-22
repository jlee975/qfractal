#include "resizedialog.h"
#include "ui_resizedialog.h"

ResizeDialog::ResizeDialog(int w, int h, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResizeDialog)
{
    ui->setupUi(this);
    ui->widthSpinBox->setValue(w);
    ui->heightSpinBox->setValue(h);
}

ResizeDialog::~ResizeDialog()
{
	delete ui;
}

QSize ResizeDialog::choice() const
{
	return QSize(ui->widthSpinBox->value(), ui->heightSpinBox->value());
}

