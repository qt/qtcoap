// Copyright (C) 2019 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "optiondialog.h"
#include "ui_optiondialog.h"

OptionDialog::OptionDialog(const QList<QCoapOption> &options, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionDialog),
    m_options(options)
{
    ui->setupUi(this);
    connect(ui->tableWidget, &QTableWidget::itemSelectionChanged, this, [this]() {
        const auto selection = ui->tableWidget->selectedItems();
        ui->removeButton->setEnabled(!selection.isEmpty());
    });

    fillOptions();
    applyOptionValues();

    auto header = ui->tableWidget->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);
}

OptionDialog::~OptionDialog()
{
    delete ui;
}

QList<QCoapOption> OptionDialog::options() const
{
    return m_options;
}

void OptionDialog::on_addButton_clicked()
{
    const auto option =
            ui->optionComboBox->currentData(Qt::UserRole).value<QCoapOption::OptionName>();
    m_options.push_back(QCoapOption(option, ui->optionValueEdit->text()));

    addTableRow(ui->optionComboBox->currentText(), ui->optionValueEdit->text());
}

void OptionDialog::on_removeButton_clicked()
{
    const auto idx = ui->tableWidget->currentRow();
    if (idx >= 0 && idx < ui->tableWidget->rowCount()) {
        ui->tableWidget->removeRow(idx);
        m_options.removeAt(idx);
    }
}

void OptionDialog::on_clearButton_clicked()
{
    m_options.clear();
    ui->tableWidget->setRowCount(0);
}

void OptionDialog::fillOptions()
{
    ui->tableWidget->setHorizontalHeaderLabels({"Name", "Value"});
    ui->optionComboBox->addItem("None", QCoapOption::Invalid);
    ui->optionComboBox->addItem("Block1", QCoapOption::Block1);
    ui->optionComboBox->addItem("Block2", QCoapOption::Block2);
    ui->optionComboBox->addItem("Content-Format", QCoapOption::ContentFormat);
    ui->optionComboBox->addItem("If-Match", QCoapOption::IfMatch);
    ui->optionComboBox->addItem("If-None-Match", QCoapOption::IfNoneMatch);
    ui->optionComboBox->addItem("Location-Path", QCoapOption::LocationPath);
    ui->optionComboBox->addItem("Location-Query", QCoapOption::LocationQuery);
    ui->optionComboBox->addItem("Max-Age", QCoapOption::MaxAge);
    ui->optionComboBox->addItem("Observe", QCoapOption::Observe);
    ui->optionComboBox->addItem("Proxy-Scheme", QCoapOption::ProxyScheme);
    ui->optionComboBox->addItem("Proxy-Uri", QCoapOption::ProxyUri);
    ui->optionComboBox->addItem("Size1", QCoapOption::Size1);
    ui->optionComboBox->addItem("Size2", QCoapOption::Size2);
    ui->optionComboBox->addItem("Uri-Host", QCoapOption::UriHost);
    ui->optionComboBox->addItem("Uri-Path", QCoapOption::UriPath);
    ui->optionComboBox->addItem("Uri-Port", QCoapOption::UriPort);
    ui->optionComboBox->addItem("Uri-Query", QCoapOption::UriQuery);
}

void OptionDialog::applyOptionValues()
{
    for (const auto &option : std::as_const(m_options)) {
        const int optionIndex = ui->optionComboBox->findData(option.name());
        if (optionIndex != -1)
            addTableRow(ui->optionComboBox->itemText(optionIndex), option.stringValue());
    }
}

void OptionDialog::addTableRow(const QString &name, const QString &value)
{
    const auto rowCount = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(rowCount);

    QTableWidgetItem *optionItem = new QTableWidgetItem(name);
    optionItem->setFlags(optionItem->flags() ^ Qt::ItemIsEditable);
    ui->tableWidget->setItem(rowCount, 0, optionItem);

    QTableWidgetItem *valueItem = new QTableWidgetItem(value);
    valueItem->setFlags(valueItem->flags() ^ Qt::ItemIsEditable);
    ui->tableWidget->setItem(rowCount, 1, valueItem);
}
