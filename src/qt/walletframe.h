// Copyright (c) 2011-2013 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_QT_WALLETFRAME_H
#define BITCOIN_QT_WALLETFRAME_H

#include <QFrame>
#include <QMap>

class BitcoinGUI;
class ClientModel;
class LyraCoinsRecipient;
class WalletModel;
class WalletView;
class TradingDialog;
class BlockExplorer;

QT_BEGIN_NAMESPACE
class QStackedWidget;
QT_END_NAMESPACE

class WalletFrame : public QFrame
{
    Q_OBJECT

public:
    explicit WalletFrame(BitcoinGUI* _gui = 0);
    ~WalletFrame();

    void setClientModel(ClientModel* clientModel);

    bool addWallet(const QString& name, WalletModel* walletModel);
    bool setCurrentWallet(const QString& name);
    bool removeWallet(const QString& name);
    void removeAllWallets();

    bool handlePaymentRequest(const LyraCoinsRecipient& recipient);

    void showOutOfSyncWarning(bool fShow);

private:
    QStackedWidget* walletStack;
    BitcoinGUI* gui;
    ClientModel* clientModel;
    QMap<QString, WalletView*> mapWalletViews;

    bool bOutOfSync;

    WalletView* currentWalletView();

public slots:
    /** Swtich to budget list page */
    void gotoBudgetView();
    /** Switch to announcement page */
    void gotoAnnView();
    /** Switch to overview (home) page */
    void gotoOverviewPage();
    /** Switch to history (transactions) page */
    void gotoHistoryPage();
    /** Switch to masternode page */
    void gotoMasternodePage();
    /** Switch to receive coins page */
    void gotoReceiveCoinsPage();
    /** Switch to lyra coins page */
    void gotoLyraCoinsPage(QString addr = "");
    /** Switch to explorer page */
    void gotoBlockExplorerPage();
    /** Show Sign/Verify Message dialog and switch to sign message tab */
    void gotoSignMessageTab(QString addr = "");
    /** Show Sign/Verify Message dialog and switch to verify message tab */
    void gotoVerifyMessageTab(QString addr = "");
    /** Show MultiLyra Dialog **/
    void gotoMultiLyraDialog();

    /** Show BIP 38 tool - default to Encryption tab */
    void gotoBip38Tool();

    /** Encrypt the wallet */
    void encryptWallet(bool status);
    /** Backup the wallet */
    void backupWallet();
    /** Change encrypted wallet passphrase */
    void changePassphrase();
    /** Ask for passphrase to unlock wallet temporarily */
    void unlockWallet();
    /** Lock wallet */
    void lockWallet();

    /** Show used lyraing addresses */
    void usedLyraingAddresses();
    /** Show used receiving addresses */
    void usedReceivingAddresses();
};

#endif // BITCOIN_QT_WALLETFRAME_H
