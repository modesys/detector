#include "connectionmenu.h"
#include "../detector/src/leftcamparameters.h"
#include "ui_connectionmenu.h"
#include <QMessageBox>
#include <QSqlDatabase>

connectionMenu::connectionMenu(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::connectionMenu)
{
    ui->setupUi(this);
    QStringList drivers = QSqlDatabase::drivers();
    // remove compat names
    drivers.removeAll("QMYSQL");
    drivers.removeAll("QMYSQL3");
    drivers.removeAll("QSQLITE");
    drivers.removeAll("QOCI8");
    drivers.removeAll("QODBC3");
    drivers.removeAll("QPSQL7");
    drivers.removeAll("QTDS7");
    // Selection of the coud system - If needed to add an additional cloud, add it here
    drivers.removeAll("PHPMyAdmin");
    drivers.removeAll("Cloud9");
    if (drivers.contains("QSQLITE")) {
        ui->checkBox->setChecked(true);
        //ui->comboBoxDriver->addItems(drivers);
    }
    if(ui->comboBoxDriver->currentText().isEmpty()) {
        QMessageBox::information(this, tr("No database driver selected"),
                                 tr("Please select a database driver"));
        ui->comboBoxDriver->setFocus();
    }
    ui->progressBar->setValue(0);

    tree = new QTreeWidget();
    tree->setContextMenuPolicy(Qt::CustomContextMenu);

//    QMenu *contextMenu = new QMenu(tree);
//    QAction *addElement = new QAction("Add Element",contextMenu);
    connect(ui->treeWidget,&QTreeWidget::customContextMenuRequested,this,&connectionMenu::prepareMenu);
//    tree->addAction(addElement);

//    connect(tree, &QTreeWidget::customContextMenuRequested, [&]() {
//        if(tree->type() == QTreeWidget::TABLE) {
//               return;
//        } else {
//            QMessageBox::warning(this, "Warning", "This function is allowed only when a table is selected");
//        }
//    });




    // Actions fpr the QTableView
    refreshAction = new QAction(QIcon(":"), tr("Refresh"), this);
    addRecord = new QAction(QIcon(":"), tr("Add Record"), this);
    removeRecord = new QAction(QIcon(":"), tr("Remove Record"), this);

    ui->tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->tableView->addActions({refreshAction});
    ui->tableView->addActions({addRecord});
    ui->tableView->addActions({removeRecord});

    mQueryModel = new QSqlQueryModel(this);
    ui->tableView->setModel(mQueryModel);
}

connectionMenu::~connectionMenu()
{
    mDatabase.close();
    delete ui;

}

void connectionMenu::setOutputImages(ImageList<Image>::Ptr outimg)
{
    imgList = outimg;


    for(int im=0;im<imgList->size();im++)
        imgList->getImage(im).getName();
}

QString connectionMenu::driverName() const
{
    return ui->comboBoxDriver->currentText();
}

QString connectionMenu::databaseName() const
{
    return ui->dbNameLineEdit->text();
}

QString connectionMenu::userName() const
{
    return ui->usernameLineEdit->text();
}

QString connectionMenu::password() const
{
    return ui->passwordLineEdit->text();
}

QString connectionMenu::hostName() const
{
    return ui->hostLineEdit->text();
}

int connectionMenu::port() const
{
    return ui->spinBoxPort->value();
}

bool connectionMenu::useInMemoryDatabase() const
{
    return ui->checkBox->isChecked();

}

/////

void connectionMenu::on_sendImagesToTable_clicked()
{
    if(mDatabase.isOpen()) {
        ui->progressBar->setMaximum(imgList->size());
        ui->progressBar->setValue(0);

     //Activate to write to database
        for(size_t i=0;i<imgList->size();i++){
            ui->progressBar->setValue(i);
            imgList->getImage(i).writeDB(activeDb.toStdString(), mDatabase/*, leftCam*/);
        }

        ui->progressBar->setValue(imgList->size());

    }
}

void connectionMenu::on_connectButton_clicked()
{
//    if(ui->comboBoxDriver->setCurrentText("Select Driver") == ) {
//        QMessageBox::information(this, tr("No databse driver selected"),
//                                 tr("Please select a databse driver"));
//        ui->comboBoxDriver->setFocus();
//    } else if(!ui->comboBoxDriver->currentText().isEmpty()) {
       //accept(); basically accept the driver
//    }
}

void connectionMenu::on_closeButton_clicked()
{
    //close();
}

void connectionMenu::on_checkBox_clicked(bool checked)
{
//    QStringList drivers = QSqlDatabase::drivers();
//    drivers.removeAll("QSQLITE");

//    if(checked) {
//        ui->comboBoxDriver->addItems("QSQLITE");
//    }
}

static QString qDBCaption(const QSqlDatabase &db)
{
//    QString nm = db.driverName();
//    nm.append(QLatin1Char(':'));
//    if (!db.userName().isEmpty())
//        nm.append(db.userName()).append(QLatin1Char('@'));
//    nm.append(db.databaseName());
//    return nm;
}

void connectionMenu::addDatabase()
{

}

void connectionMenu::removeDatabase()
{

}

void connectionMenu::prepareMenu(const QPoint &pos)
{
    QTreeWidgetItem *nd = ui->treeWidget->itemAt(pos);

    qDebug()<<pos<<nd->text(0);

    QAction *newAct = new QAction(QIcon(":"), tr("&New"), this);
    newAct->setStatusTip(tr("new sth"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newDev()));

    QMenu menu(this);
    menu.addAction(newAct);
    QPoint pt(pos);
    menu.exec( ui->treeWidget->mapToGlobal(pos) );
}



void connectionMenu::refresh()
{
//    tree->clear();
//        QStringList connectionNames = QSqlDatabase::connectionNames();

//        bool gotActiveDb = false;
//        for (int i = 0; i < connectionNames.count(); ++i) {
//            QTreeWidgetItem *root = new QTreeWidgetItem(tree);
//            QSqlDatabase db = QSqlDatabase::database(connectionNames.at(i), false);
//            root->setText(0, qDBCaption(db));
//            if (connectionNames.at(i) == activeDb) {
//                gotActiveDb = true;
//                setActive(root, 0);
//            }
//            if (db.isOpen()) {
//                QStringList tables = db.tables();
//                for (int t = 0; t < tables.count(); ++t) {
//                    QTreeWidgetItem *table = new QTreeWidgetItem(root);
//                    table->setText(0, tables.at(t));
//                }
//            }
//        }
//        if (!gotActiveDb) {
//            activeDb = connectionNames.value(0);
//            setActive(tree->topLevelItem(0), 0);
//        }

//        tree->doItemsLayout(); // HACK

}


QSqlDatabase connectionMenu::currentDatabase() const
{
    return QSqlDatabase::database(activeDb);
}


static void qSetBold(QTreeWidgetItem *item, bool bold)
{
    QFont font = item->font(0);
    font.setBold(bold);
    item->setFont(0, font);
}

void connectionMenu::setActive(QTreeWidgetItem *item, int column)
{
    for (int i = 0; i < tree->topLevelItemCount(); ++i) {
        if (tree->topLevelItem(i)->font(0).bold())
            qSetBold(tree->topLevelItem(i), false);
    }
    if (!item)
        return;

    // We will show the content of the ckicked table here
    qSetBold(item, true);
    activeDb = item->text(column);
    mQueryModel->setQuery("SELECT * FROM " + activeDb, mDatabase);
    ui->tableView->show();
//    if (mQueryModel->lastError().isValid())
//        qDebug() << mQueryModel->lastError();
    //    qDebug() << activeDb << " " << endl;
}


void connectionMenu::on_treeWidget_itemActivated(QTreeWidgetItem *item, int column)
{
//    setActive(ui->treeWidget->currentItem(), 0);
//    emit tableActivated(ui->treeWidget->currentItem()->text(0));
}

void connectionMenu::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    //metaDataAction->setEnabled(current && current->parent());
}


void connectionMenu::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    setActive(ui->treeWidget->currentItem(), 0);
    emit tableActivated(ui->treeWidget->currentItem()->text(0));
}

void connectionMenu::on_pushButton_pressed()
{
    if(!mDatabase.isOpen())
    {
        Image testImg;
        mDatabase = QSqlDatabase::addDatabase("QSQLITE");
        // Giving the path where to read and write the .db file
        QString filepath = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     "/home",
                                                     tr("Database (*.db)"));
        // Setting the database name
        mDatabase.setDatabaseName(filepath);
        mDatabase.open();

        Image i;
        QStringList tablesFound = i.readDBTables(mDatabase);
        auto baseItem = new QTreeWidgetItem;
        baseItem->setIcon(0, QIcon(":database"));
        baseItem->setText(0, "List of Tables");

        for(int tb=0;tb<tablesFound.size();tb++)
        {
            auto tablesItem = new QTreeWidgetItem;
            tablesItem->setIcon(0, QIcon(":"));
            tablesItem->setText(0, tablesFound[tb]);
            baseItem->addChild(tablesItem);
        }
        ui->treeWidget->addTopLevelItem(baseItem);
        ui->label_8->setText("Connection Status: connected to file");
    }
}

void connectionMenu::on_checkBox_toggled(bool checked)
{
//    if(checked) {
//        switch(ui->comboBoxDriver->value());
//        {

//            case(One of them):
//            {
//            ui->nameLineEdit->setValue();
//            ui->comboBoxDriver->setValue();
//            ui->usernameLineEdit->setValue();
//            ui->dbNameLineEdit->setValue();
//            ui->passwordLineEdit->setValue();
//            ui->hostLineEdit->setValue();
//            ui->spinBoxPort->setValue();
//            }
//            break;
//        }
//    } else {

//    }
}

connectionMenu::Type connectionMenu::type(ERTreeItem *item) const
{
    if (item == nullptr) {
        return NULLPTR;
    }
    auto padre = item->parent();
    if (padre == nullptr) {
        return BASE;
    }
    return TABLE;
}
