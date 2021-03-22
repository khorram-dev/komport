#include <klocalizedstring.h>

/********************************************************************************
** Form generated from reading UI file 'newdevice.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWDEVICE_H
#define UI_NEWDEVICE_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_newDevice
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QLineEdit *leProfileName;
    QTabWidget *tabWidget;
    QWidget *tabSerialport;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *serialBox;
    QGridLayout *gridLayout;
    QLabel *laDataBits;
    QLabel *laStopBits;
    QLabel *laBaudRate;
    QComboBox *cbBaudRate;
    QLabel *laParity;
    QComboBox *cbParity;
    QLabel *laDevicesPort;
    QComboBox *cbStopBits;
    QComboBox *cbFlowControl;
    QLabel *laFlowControl;
    QComboBox *cbDataBits;
    QComboBox *cbDevicesPort;
    QFrame *line_2;
    QFrame *line_3;
    QWidget *tabSocket;
    QVBoxLayout *verticalLayout_4;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_2;
    QFrame *line;
    QHBoxLayout *horizontalLayout_2;
    QComboBox *comboBox_Interface;
    QLabel *label_2;
    QLineEdit *lineEdit_LocalIP;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *button_Refresh;
    QHBoxLayout *horizontalLayout_4;
    QComboBox *comboBox_Interface_2;
    QLabel *label_3;
    QLineEdit *lineEdit_UdpListenPort;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pbOK;
    QPushButton *pbCancel;

    void setupUi(QDialog *newDevice)
    {
        if (newDevice->objectName().isEmpty())
            newDevice->setObjectName(QString::fromUtf8("newDevice"));
        newDevice->resize(405, 378);
        verticalLayout = new QVBoxLayout(newDevice);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label = new QLabel(newDevice);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_3->addWidget(label);

        leProfileName = new QLineEdit(newDevice);
        leProfileName->setObjectName(QString::fromUtf8("leProfileName"));

        horizontalLayout_3->addWidget(leProfileName);


        verticalLayout->addLayout(horizontalLayout_3);

        tabWidget = new QTabWidget(newDevice);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabSerialport = new QWidget();
        tabSerialport->setObjectName(QString::fromUtf8("tabSerialport"));
        verticalLayout_3 = new QVBoxLayout(tabSerialport);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(9, 9, 9, 9);
        serialBox = new QGroupBox(tabSerialport);
        serialBox->setObjectName(QString::fromUtf8("serialBox"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(serialBox->sizePolicy().hasHeightForWidth());
        serialBox->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(serialBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(9, 9, 9, 9);
        laDataBits = new QLabel(serialBox);
        laDataBits->setObjectName(QString::fromUtf8("laDataBits"));

        gridLayout->addWidget(laDataBits, 4, 0, 1, 1);

        laStopBits = new QLabel(serialBox);
        laStopBits->setObjectName(QString::fromUtf8("laStopBits"));

        gridLayout->addWidget(laStopBits, 5, 0, 1, 1);

        laBaudRate = new QLabel(serialBox);
        laBaudRate->setObjectName(QString::fromUtf8("laBaudRate"));

        gridLayout->addWidget(laBaudRate, 2, 0, 1, 1);

        cbBaudRate = new QComboBox(serialBox);
        cbBaudRate->setObjectName(QString::fromUtf8("cbBaudRate"));

        gridLayout->addWidget(cbBaudRate, 2, 1, 1, 1);

        laParity = new QLabel(serialBox);
        laParity->setObjectName(QString::fromUtf8("laParity"));

        gridLayout->addWidget(laParity, 3, 0, 1, 1);

        cbParity = new QComboBox(serialBox);
        cbParity->setObjectName(QString::fromUtf8("cbParity"));

        gridLayout->addWidget(cbParity, 3, 1, 1, 1);

        laDevicesPort = new QLabel(serialBox);
        laDevicesPort->setObjectName(QString::fromUtf8("laDevicesPort"));

        gridLayout->addWidget(laDevicesPort, 1, 0, 1, 1);

        cbStopBits = new QComboBox(serialBox);
        cbStopBits->setObjectName(QString::fromUtf8("cbStopBits"));

        gridLayout->addWidget(cbStopBits, 5, 1, 1, 1);

        cbFlowControl = new QComboBox(serialBox);
        cbFlowControl->setObjectName(QString::fromUtf8("cbFlowControl"));

        gridLayout->addWidget(cbFlowControl, 6, 1, 1, 1);

        laFlowControl = new QLabel(serialBox);
        laFlowControl->setObjectName(QString::fromUtf8("laFlowControl"));

        gridLayout->addWidget(laFlowControl, 6, 0, 1, 1);

        cbDataBits = new QComboBox(serialBox);
        cbDataBits->setObjectName(QString::fromUtf8("cbDataBits"));

        gridLayout->addWidget(cbDataBits, 4, 1, 1, 1);

        cbDevicesPort = new QComboBox(serialBox);
        cbDevicesPort->setObjectName(QString::fromUtf8("cbDevicesPort"));

        gridLayout->addWidget(cbDevicesPort, 1, 1, 1, 1);

        line_2 = new QFrame(serialBox);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_2, 0, 1, 1, 1);

        line_3 = new QFrame(serialBox);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_3, 0, 0, 1, 1);


        verticalLayout_3->addWidget(serialBox);

        tabWidget->addTab(tabSerialport, QString());
        tabSocket = new QWidget();
        tabSocket->setObjectName(QString::fromUtf8("tabSocket"));
        verticalLayout_4 = new QVBoxLayout(tabSocket);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        groupBox_3 = new QGroupBox(tabSocket);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        verticalLayout_2 = new QVBoxLayout(groupBox_3);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        line = new QFrame(groupBox_3);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout_2->addWidget(line);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        comboBox_Interface = new QComboBox(groupBox_3);
        comboBox_Interface->setObjectName(QString::fromUtf8("comboBox_Interface"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(comboBox_Interface->sizePolicy().hasHeightForWidth());
        comboBox_Interface->setSizePolicy(sizePolicy1);
        comboBox_Interface->setMinimumSize(QSize(100, 0));
        comboBox_Interface->setMaximumSize(QSize(200, 16777215));
        comboBox_Interface->setEditable(false);

        horizontalLayout_2->addWidget(comboBox_Interface);

        label_2 = new QLabel(groupBox_3);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy2);

        horizontalLayout_2->addWidget(label_2);

        lineEdit_LocalIP = new QLineEdit(groupBox_3);
        lineEdit_LocalIP->setObjectName(QString::fromUtf8("lineEdit_LocalIP"));

        horizontalLayout_2->addWidget(lineEdit_LocalIP);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_5);

        button_Refresh = new QPushButton(groupBox_3);
        button_Refresh->setObjectName(QString::fromUtf8("button_Refresh"));

        horizontalLayout_2->addWidget(button_Refresh);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        comboBox_Interface_2 = new QComboBox(groupBox_3);
        comboBox_Interface_2->addItem(QString());
        comboBox_Interface_2->addItem(QString());
        comboBox_Interface_2->addItem(QString());
        comboBox_Interface_2->setObjectName(QString::fromUtf8("comboBox_Interface_2"));
        sizePolicy1.setHeightForWidth(comboBox_Interface_2->sizePolicy().hasHeightForWidth());
        comboBox_Interface_2->setSizePolicy(sizePolicy1);
        comboBox_Interface_2->setMinimumSize(QSize(100, 0));
        comboBox_Interface_2->setMaximumSize(QSize(200, 16777215));

        horizontalLayout_4->addWidget(comboBox_Interface_2);

        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        sizePolicy2.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy2);

        horizontalLayout_4->addWidget(label_3);

        lineEdit_UdpListenPort = new QLineEdit(groupBox_3);
        lineEdit_UdpListenPort->setObjectName(QString::fromUtf8("lineEdit_UdpListenPort"));
        sizePolicy2.setHeightForWidth(lineEdit_UdpListenPort->sizePolicy().hasHeightForWidth());
        lineEdit_UdpListenPort->setSizePolicy(sizePolicy2);
        lineEdit_UdpListenPort->setMinimumSize(QSize(80, 0));
        lineEdit_UdpListenPort->setMaximumSize(QSize(80, 16777215));

        horizontalLayout_4->addWidget(lineEdit_UdpListenPort);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);


        verticalLayout_2->addLayout(horizontalLayout_4);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        verticalLayout_4->addWidget(groupBox_3);

        tabWidget->addTab(tabSocket, QString());

        verticalLayout->addWidget(tabWidget);

        widget = new QWidget(newDevice);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(1, 1, 1, 1);
        horizontalSpacer = new QSpacerItem(250, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pbOK = new QPushButton(widget);
        pbOK->setObjectName(QString::fromUtf8("pbOK"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Icons/Icons/ok.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbOK->setIcon(icon);

        horizontalLayout->addWidget(pbOK);

        pbCancel = new QPushButton(widget);
        pbCancel->setObjectName(QString::fromUtf8("pbCancel"));

        horizontalLayout->addWidget(pbCancel);


        verticalLayout->addWidget(widget);

        QWidget::setTabOrder(cbDevicesPort, cbBaudRate);
        QWidget::setTabOrder(cbBaudRate, cbParity);
        QWidget::setTabOrder(cbParity, cbDataBits);
        QWidget::setTabOrder(cbDataBits, cbStopBits);
        QWidget::setTabOrder(cbStopBits, cbFlowControl);
        QWidget::setTabOrder(cbFlowControl, pbOK);

        retranslateUi(newDevice);

        tabWidget->setCurrentIndex(1);
        pbOK->setDefault(true);


        QMetaObject::connectSlotsByName(newDevice);
    } // setupUi

    void retranslateUi(QDialog *newDevice)
    {
        newDevice->setWindowTitle(tr2i18n("Open Com Port", nullptr));
        label->setText(tr2i18n("Profile name:", nullptr));
        serialBox->setTitle(tr2i18n("RS232", nullptr));
        laDataBits->setText(tr2i18n("Data bits", nullptr));
        laStopBits->setText(tr2i18n("Stop bits", nullptr));
        laBaudRate->setText(tr2i18n("Baud rate", nullptr));
        laParity->setText(tr2i18n("Parity", nullptr));
        laDevicesPort->setText(tr2i18n("Com Port", nullptr));
        laFlowControl->setText(tr2i18n("Flow control", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabSerialport), tr2i18n("Serialport", nullptr));
        groupBox_3->setTitle(tr2i18n("Network Interface:", nullptr));
        label_2->setText(tr2i18n("IP: ", nullptr));
        button_Refresh->setText(tr2i18n("Refresh", nullptr));
        comboBox_Interface_2->setItemText(0, tr2i18n("TCP Client", nullptr));
        comboBox_Interface_2->setItemText(1, tr2i18n("TCP Server", nullptr));
        comboBox_Interface_2->setItemText(2, tr2i18n("UDP", nullptr));

        label_3->setText(tr2i18n("Port:", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabSocket), tr2i18n("Socket", nullptr));
        pbOK->setText(tr2i18n("Ok", nullptr));
        pbCancel->setText(tr2i18n("Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class newDevice: public Ui_newDevice {};
} // namespace Ui

QT_END_NAMESPACE

#endif // NEWDEVICE_H

