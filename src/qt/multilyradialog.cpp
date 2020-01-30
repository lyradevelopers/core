#include "multilyradialog.h"
#include "addressbookpage.h"
#include "base58.h"
#include "init.h"
#include "ui_multilyradialog.h"
#include "walletmodel.h"
#include <QLineEdit>
#include <QStyle>
#include <QMessageBox>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;

MultiLyraDialog::MultiLyraDialog(QWidget* parent) : QDialog(parent),
                                                    ui(new Ui::MultiLyraDialog),
                                                    model(0)
{
    ui->setupUi(this);

    updateCheckBoxes();
}

MultiLyraDialog::~MultiLyraDialog()
{
    delete ui;
}

void MultiLyraDialog::setModel(WalletModel* model)
{
    this->model = model;
}

void MultiLyraDialog::setAddress(const QString& address)
{
    setAddress(address, ui->multiLyraAddressEdit);
}

void MultiLyraDialog::setAddress(const QString& address, QLineEdit* addrEdit)
{
    addrEdit->setText(address);
    addrEdit->setFocus();
}

void MultiLyraDialog::updateCheckBoxes()
{
    ui->multiLyraStakeCheckBox->setChecked(pwalletMain->fMultiLyraStake);
    ui->multiLyraMasternodeCheckBox->setChecked(pwalletMain->fMultiLyraMasternodeReward);
}

void MultiLyraDialog::on_addressBookButton_clicked()
{
    if (model && model->getAddressTableModel()) {
        AddressBookPage dlg(AddressBookPage::ForSelection, AddressBookPage::LyraingTab, this);
        dlg.setModel(model->getAddressTableModel());
        if (dlg.exec())
            setAddress(dlg.getReturnValue(), ui->multiLyraAddressEdit);
    }
}

void MultiLyraDialog::on_viewButton_clicked()
{
    std::pair<std::string, int> pMultiLyra;
    std::string strMultiLyraPrint = "";
    if (pwalletMain->isMultiLyraEnabled()) {
        if (pwalletMain->fMultiLyraStake)
            strMultiLyraPrint += "MultiLyra Active for Stakes\n";
        else if (pwalletMain->fMultiLyraStake)
            strMultiLyraPrint += "MultiLyra Active for Masternode Rewards\n";
    } else
        strMultiLyraPrint += "MultiLyra Not Active\n";

    for (int i = 0; i < (int)pwalletMain->vMultiLyra.size(); i++) {
        pMultiLyra = pwalletMain->vMultiLyra[i];
        strMultiLyraPrint += pMultiLyra.first.c_str();
        strMultiLyraPrint += " - ";
        strMultiLyraPrint += boost::lexical_cast<string>(pMultiLyra.second);
        strMultiLyraPrint += "% \n";
    }
    ui->message->setProperty("status", "ok");
    ui->message->style()->polish(ui->message);
    ui->message->setText(QString(strMultiLyraPrint.c_str()));
    return;
}

void MultiLyraDialog::on_addButton_clicked()
{
    bool fValidConversion = false;
    std::string strAddress = ui->multiLyraAddressEdit->text().toStdString();
    if (!CBitcoinAddress(strAddress).IsValid()) {
        ui->message->setProperty("status", "error");
        ui->message->style()->polish(ui->message);
        ui->message->setText(tr("The entered address:\n") + ui->multiLyraAddressEdit->text() + tr(" is invalid.\nPlease check the address and try again."));
        ui->multiLyraAddressEdit->setFocus();
        return;
    }
    int nMultiLyraPercent = ui->multiLyraPercentEdit->text().toInt(&fValidConversion, 10);
    int nSumMultiLyra = 0;
    for (int i = 0; i < (int)pwalletMain->vMultiLyra.size(); i++)
        nSumMultiLyra += pwalletMain->vMultiLyra[i].second;
    if (nSumMultiLyra + nMultiLyraPercent > 100) {
        ui->message->setProperty("status", "error");
        ui->message->style()->polish(ui->message);
        ui->message->setText(tr("The total amount of your MultiLyra vector is over 100% of your stake reward\n"));
        ui->multiLyraAddressEdit->setFocus();
        return;
    }
    if (!fValidConversion || nMultiLyraPercent > 100 || nMultiLyraPercent <= 0) {
        ui->message->setProperty("status", "error");
        ui->message->style()->polish(ui->message);
        ui->message->setText(tr("Please Enter 1 - 100 for percent."));
        ui->multiLyraPercentEdit->setFocus();
        return;
    }
    std::pair<std::string, int> pMultiLyra;
    pMultiLyra.first = strAddress;
    pMultiLyra.second = nMultiLyraPercent;
    pwalletMain->vMultiLyra.push_back(pMultiLyra);
    ui->message->setProperty("status", "ok");
    ui->message->style()->polish(ui->message);
    std::string strMultiLyraPrint = "";
    for (int i = 0; i < (int)pwalletMain->vMultiLyra.size(); i++) {
        pMultiLyra = pwalletMain->vMultiLyra[i];
        strMultiLyraPrint += pMultiLyra.first.c_str();
        strMultiLyraPrint += " - ";
        strMultiLyraPrint += boost::lexical_cast<string>(pMultiLyra.second);
        strMultiLyraPrint += "% \n";
    }
    CWalletDB walletdb(pwalletMain->strWalletFile);
    walletdb.WriteMultiLyra(pwalletMain->vMultiLyra);
    ui->message->setText(tr("MultiLyra Vector\n") + QString(strMultiLyraPrint.c_str()));
    return;
}

void MultiLyraDialog::on_deleteButton_clicked()
{
    std::vector<std::pair<std::string, int> > vMultiLyraTemp = pwalletMain->vMultiLyra;
    std::string strAddress = ui->multiLyraAddressEdit->text().toStdString();
    bool fRemoved = false;
    for (int i = 0; i < (int)pwalletMain->vMultiLyra.size(); i++) {
        if (pwalletMain->vMultiLyra[i].first == strAddress) {
            pwalletMain->vMultiLyra.erase(pwalletMain->vMultiLyra.begin() + i);
            fRemoved = true;
        }
    }
    CWalletDB walletdb(pwalletMain->strWalletFile);
    if (!walletdb.EraseMultiLyra(vMultiLyraTemp))
        fRemoved = false;
    if (!walletdb.WriteMultiLyra(pwalletMain->vMultiLyra))
        fRemoved = false;

    if (fRemoved)
        ui->message->setText(tr("Removed ") + QString(strAddress.c_str()));
    else
        ui->message->setText(tr("Could not locate address\n"));

    updateCheckBoxes();

    return;
}

void MultiLyraDialog::on_activateButton_clicked()
{
    std::string strRet = "";
    if (pwalletMain->vMultiLyra.size() < 1)
        strRet = "Unable to activate MultiLyra, check MultiLyra vector\n";
    else if (!(ui->multiLyraStakeCheckBox->isChecked() || ui->multiLyraMasternodeCheckBox->isChecked())) {
        strRet = "Need to select to lyra on stake and/or masternode rewards\n";
    } else if (CBitcoinAddress(pwalletMain->vMultiLyra[0].first).IsValid()) {
        pwalletMain->fMultiLyraStake = ui->multiLyraStakeCheckBox->isChecked();
        pwalletMain->fMultiLyraMasternodeReward = ui->multiLyraMasternodeCheckBox->isChecked();

        CWalletDB walletdb(pwalletMain->strWalletFile);
        if (!walletdb.WriteMSettings(pwalletMain->fMultiLyraStake, pwalletMain->fMultiLyraMasternodeReward, pwalletMain->nLastMultiLyraHeight))
            strRet = "MultiLyra activated but writing settings to DB failed";
        else
            strRet = "MultiLyra activated";
    } else
        strRet = "First Address Not Valid";
    ui->message->setProperty("status", "ok");
    ui->message->style()->polish(ui->message);
    ui->message->setText(tr(strRet.c_str()));
    return;
}

void MultiLyraDialog::on_disableButton_clicked()
{
    std::string strRet = "";
    pwalletMain->setMultiLyraDisabled();
    CWalletDB walletdb(pwalletMain->strWalletFile);
    if (!walletdb.WriteMSettings(false, false, pwalletMain->nLastMultiLyraHeight))
        strRet = "MultiLyra deactivated but writing settings to DB failed";
    else
        strRet = "MultiLyra deactivated";
    ui->message->setProperty("status", "");
    ui->message->style()->polish(ui->message);
    ui->message->setText(tr(strRet.c_str()));
    return;
}
