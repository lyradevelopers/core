#ifndef MULTILYRADIALOG_H
#define MULTILYRADIALOG_H

#include <QDialog>

namespace Ui
{
class MultiLyraDialog;
}

class WalletModel;
class QLineEdit;
class MultiLyraDialog : public QDialog
{
    Q_OBJECT
    void updateCheckBoxes();

public:
    explicit MultiLyraDialog(QWidget* parent = 0);
    ~MultiLyraDialog();
    void setModel(WalletModel* model);
    void setAddress(const QString& address);
    void setAddress(const QString& address, QLineEdit* addrEdit);
private slots:
    void on_viewButton_clicked();
    void on_addButton_clicked();
    void on_deleteButton_clicked();
    void on_activateButton_clicked();
    void on_disableButton_clicked();
    void on_addressBookButton_clicked();

private:
    Ui::MultiLyraDialog* ui;
    WalletModel* model;
};

#endif // MULTILYRADIALOG_H
