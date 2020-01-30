// Copyright (c) 2011-2013 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_QT_LYRACOINSENTRY_H
#define BITCOIN_QT_LYRACOINSENTRY_H

#include "walletmodel.h"

#include <QStackedWidget>

class WalletModel;

namespace Ui
{
class LyraCoinsEntry;
}

/**
 * A single entry in the dialog for lyraing bitcoins.
 * Stacked widget, with different UIs for payment requests
 * with a strong payee identity.
 */
class LyraCoinsEntry : public QStackedWidget
{
    Q_OBJECT

public:
    explicit LyraCoinsEntry(QWidget* parent = 0);
    ~LyraCoinsEntry();

    void setModel(WalletModel* model);
    bool validate();
    LyraCoinsRecipient getValue();

    /** Return whether the entry is still empty and unedited */
    bool isClear();

    void setValue(const LyraCoinsRecipient& value);
    void setAddress(const QString& address);
    void setAmount(const CAmount nAmount);

    /** Set up the tab chain manually, as Qt messes up the tab chain by default in some cases
     *  (issue https://bugreports.qt-project.org/browse/QTBUG-10907).
     */
    QWidget* setupTabChain(QWidget* prev);

    void setFocus();

public slots:
    void clear();

signals:
    void removeEntry(LyraCoinsEntry* entry);
    void payAmountChanged();

private slots:
    void deleteClicked();
    void on_payTo_textChanged(const QString& address);
    void on_addressBookButton_clicked();
    void on_pasteButton_clicked();
    void updateDisplayUnit();

private:
    LyraCoinsRecipient recipient;
    Ui::LyraCoinsEntry* ui;
    WalletModel* model;

    bool updateLabel(const QString& address);
};

#endif // BITCOIN_QT_LYRACOINSENTRY_H
