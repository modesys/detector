#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../src/leftcamparameters.h"
#include "../src/rightcamparameters.h"
#include "clipscene.h"
#include "../src/imagedelegate.h"
#include "editlefttabledialog.h"
#include "../src/dataleftcamera.h"
#include "../src/datarightcamera.h"
#include "../src/dataregion.h"
//#include "../ui/windowingdialog.h"
#include "../src/mgraphicsview.h"
#include "../src/graphicsviewbothdrag.h"
#include "../libCam/include/stereocal.h"
#include "../libCam/include/stereolist.h"
#include "connectionmenu.h"
//#include "windowingdialog.h"
#include "editlefttabledialog.h"
#include "colortabllecombobox.h"

#include <QString>
#include <QFileDialog>
#include <QByteArray>
#include <QBuffer>
#include <QMessageBox>
#include <QPrintDialog>
#include <QPrinter>
#include <QRubberBand>
#include <QAction>
#include <QTableView>
#include <QSqlTableModel>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsView>
#include <QFile>
#include <QDataStream>
#include <QMessageBox>
#include <QPixmap>
#include <QMatrix>
#include <QTransform>
#include <QPointF>
#include <QPainter>
#include <QCheckBox>

using namespace cv;
using namespace std;

QImage cvMat2QImage(cv::Mat image)
{
  // Convert the image to the RGB888 format
  cv::Mat image_display;
  switch (image.type())
  {
    case CV_8UC1:
      cvtColor(image, image_display, CV_GRAY2RGB);
      break;
    case CV_8UC3:
      cvtColor(image, image_display, CV_BGR2RGB);
      break;
  }
  // QImage needs the data to be stored continuously in memory
  assert(image_display.isContinuous());
  // Assign OpenCV's image buffer to the QImage. Note that the bytesPerLine parameter
  // (http://qt-project.org/doc/qt-4.8/qimage.html#QImage-6) is 3*width because each pixel
  // has three bytes.
  return QImage(image_display.data, image_display.cols, image_display.rows, image_display.cols * 3,
                QImage::Format_RGB888)
      .copy();
}


QPixmap cvMat2QPixmap(cv::Mat image)
{
  return QPixmap::fromImage(cvMat2QImage(image)).copy();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->labelCroppedA->setText("<b>No Image Set!</b>");
    ui->labelCroppedB->setText("<b>No Image Set!</b>");

    QPixmap pxR(15,15);//create pixmap
    pxR.fill(QColor(Qt::red));//all pixmap will be red
    QIcon iconR(pxR);
    ui->comboBoxColor->addItem(iconR,"Red");//set our icon

    QPixmap pxG(15,15);
    pxG.fill(QColor(Qt::green));
    QIcon iconG(pxG);
    ui->comboBoxColor->addItem(iconG,"Green");

    QPixmap pxBL(15,15);
    pxBL.fill(QColor(Qt::black));
    QIcon iconBL(pxBL);
    ui->comboBoxColor->addItem(iconBL,"Black");

    QPixmap pxY(15,15);
    pxY.fill(QColor(Qt::yellow));
    QIcon iconY(pxY);
    ui->comboBoxColor->addItem(iconY,"Yellow");

    QPixmap pxDY(15,15);
    pxDY.fill(QColor(Qt::darkYellow));
    QIcon iconDY(pxDY);
    ui->comboBoxColor->addItem(iconDY,"Orange");

    QPixmap pxBLUE(15,15);
    pxBLUE.fill(QColor(Qt::blue));
    QIcon iconBLUE(pxBLUE);
    ui->comboBoxColor->addItem(iconBLUE,"Blue");

    connect(ui->actionHelp, &QAction::triggered, this, &MainWindow::slotHelp);

//    connect(ui->leftView->verticalScrollBar(), SIGNAL(valueChanged(int)),
//            ui->rightView->verticalScrollBar(), SLOT(setValue(int)));

//    connect(ui->leftView->horizontalScrollBar(), SIGNAL(valueChanged(int)),
//            ui->rightView->horizontalScrollBar(), SLOT(setValue(int)));

    connect(ui->recordLeftTableView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(onTableClick(QModelIndex)));

    // Make left and right scenes
    leftScene = new MGraphicsScene();
    ui->leftView->setScene(leftScene);
    rightScene = new QGraphicsScene();
    ui->rightView->setScene(rightScene);

    listPtr.reset(new StereoList);
    graph = new graphDialog(this);
    lastMatchError = new std::vector<double>;
    ui->matchProgressBarLeft->setValue(0);
    ui->regionSizeSpinBox->setMaximum(999);
    ui->regionSizeSpinBox->setMinimum(9);

    // XXX Fix Me - Leave Point

    searchArea = new QGraphicsRectItem(0, 0, 0, 0);
//    searchArea->setPen(QPen(QBrush(Qt::red), 20));
    searchArea->setPen(QPen(Qt::NoPen));
    leftScene->addItem(searchArea);
    ui->leftView->show();

    searchingArea = new QGraphicsRectItem(0, 0, 0, 0);
    searchingArea->setPen(QPen(QBrush(Qt::red), 20));
    rightScene->addItem(searchingArea);
    ui->rightView->show();

    newBox = NULL;
    imageLoaded = false;

    points.clear();
    currentSelection = -1;
    currentLeftImagePath = "";

    ui->comboBoxColor->setCurrentIndex(0);
    connect(ui->comboBoxColor, SIGNAL(activated(const QString&)),
            this, SLOT(on_comboBoxColor_activated(const QString&)));

    // Setting zoom and moving capabilities for both scenes
    connect(ui->leftView, SIGNAL(rubberBandChanged(QRect,QPointF,QPointF)), this, SLOT(onRubberBandUpdate(QRect,QPointF,QPointF)));
    connect(ui->rightView, SIGNAL(rubberBandChanged(QRect,QPointF,QPointF)), this, SLOT(onRubberBandUpdate(QRect,QPointF,QPointF)));
    
    // correct but I need to create a QGraphicsScene where to inherit the information
    //connect(leftScene, SIGNAL(mouseReleased(QPointF)), this, SLOT(onSceneMouseRelease(QPointF)));
    connect(leftScene, SIGNAL(mouseDoubleClicked(QPointF)), this, SLOT(onSceneDoubleClick(QPointF)));
    connect(leftScene, SIGNAL(mouseReleased(QPointF)), this, SLOT(eventFilter(QPointF)));

    // Initialize and configure database for the left tableview
    mDatabaseLeftCamera = new dataLeftCamera(this);
    mModelLeftCamera = nullptr;
    mModelLeftCamera = new QSqlTableModel(this);
    ui->recordLeftTableView->setModel(mModelLeftCamera);
    ui->recordLeftTableView->setItemDelegate(new ImageDelegate(this));
    connections();
    //ui->SSDleftLineEdit->setText("---");
    ui->distAB->setText("---");

    croppedIndexA = 0;
    croppedIndexB = 0;

    createArrays();

    QDirModelObj = new QDirModel();


    ui->rightView->setModifiers(Qt::AltModifier);
    ui->leftView->setModifiers(Qt::ControlModifier);

    connect(ui->leftView, SIGNAL(scaleSignal(double, QPointF)), ui->rightView, SLOT(scaleSlot(double, QPointF)));
    connect(ui->rightView, SIGNAL(scaleSignal(double, QPointF)), ui->leftView, SLOT(scaleSlot(double, QPointF)));

    ui->saveCropsImgsBtn->setEnabled(true);
    ui->loadYMLFile->setEnabled(false);
    ui->loadYMLFile->setStyleSheet("QPushButton{ background-color: grey }");

    ui->refreshBtn->setEnabled(false);
    ui->refreshBtn->setStyleSheet("QPushButton{ background-color: grey }");

    ui->imageCroppedABtn->setEnabled(false);
    ui->imageCroppedABtn->setStyleSheet("QPushButton{ background-color: grey }");

    ui->imageCroppedBBtn->setEnabled(false);
    ui->imageCroppedBBtn->setStyleSheet("QPushButton{ background-color: grey }");

    ui->leftArrowCroppedA->setEnabled(false);
    ui->rightArrowCroppedA->setEnabled(false);
    ui->leftArrowCroppedA->setStyleSheet("QPushButton{ background-color: grey }");
    ui->rightArrowCroppedA->setStyleSheet("QPushButton{ background-color: grey }");

    ui->leftArrowCroppedB->setEnabled(false);
    ui->rightArrowCroppedB->setEnabled(false);
    ui->leftArrowCroppedB->setStyleSheet("QPushButton{ background-color: grey }");
    ui->rightArrowCroppedB->setStyleSheet("QPushButton{ background-color: grey }");

    ui->loadPairA->setEnabled(false);
    ui->loadPairA->setStyleSheet("QPushButton{ background-color: grey }");

    ui->loadPairB->setEnabled(false);
    ui->loadPairB->setStyleSheet("QPushButton{ background-color: grey }");

    lgrid = nullptr;
    rgrid = nullptr;

    // Default crops directory
    loadedDirectorySaved = "/home/emanuele/Desktop/croppedMatches";

    // temporary folder
    temporaryFolder = "/home/emanuele/Desktop/tempDBFolder/tmp.db";
    QFile dbRem(temporaryFolder);
    dbRem.remove();
    mDatabaseLeftCamera->inizializationDatabaseLeftCamera(temporaryFolder);
    mDatabaseLeftCamera->configurationDatabaseLeftCamera();
    mModelLeftCamera = new QSqlTableModel(this, mDatabaseLeftCamera->getDatabase());
    mModelLeftCamera->setTable("leftCamTable");
    mModelLeftCamera->select();
    ui->recordLeftTableView->setModel(mModelLeftCamera);
    ui->recordLeftTableView->setItemDelegate(new ImageDelegate(this));
    ui->recordLeftTableView->showColumn(true);
}


void MainWindow::initPrimarySubCBox(const QString presentPrimaryBoxLeft) {
    if(presentPrimaryBoxLeft.isEmpty())
        ui->primarySubComboBoxLeft->isEnabled();
        QMessageBox::information(this, "Option not allowed yet", "Please upload images before using this selection");
    return;
}

void MainWindow::initSecondarySubCBox(const QString presentSecondaryBoxLeft) {
    if(presentSecondaryBoxLeft.isEmpty())
        ui->secondarySubComboBoxLeft->isEnabled();
        QMessageBox::information(this, "Option not allowed yet", "Please upload images before using this function");
    return;
}

void MainWindow::initDrawBoxesCBox(const QString presentDrawingBoxSelection) {
    // same as above
    Q_UNUSED(presentDrawingBoxSelection);
}

void MainWindow::connections() {
    // Removing, adding and editing records on the left table
    mEditLeftTable = new QAction(QIcon(":"), tr("Edit Left Table"), this);
    ui->recordLeftTableView->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->recordLeftTableView->addActions(QList<QAction*>() << mEditLeftTable);    
    connect(mEditLeftTable, &QAction::triggered, [&]() {
        editLeftTableDialog d("leftCamTable", this);
        if(d.exec() == QDialog::Rejected) {
            return;
        } else {
        }
        QString select = "SELECT * FROM leftCamTable";
        mModelLeftCamera->setTable("leftCamTable");
        mModelLeftCamera->select();
        ui->recordLeftTableView->setModel(mModelLeftCamera);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mItemLeftCamParam;
    delete mItemRightCamParam;
}

void MainWindow::drawLeftBox(QRectF area, QColor color)
{
    QGraphicsPolygonItem* newBox = leftScene->addPolygon(QPolygonF() << area.topLeft() << area.topRight() << area.bottomRight() << area.bottomLeft());
    boxes.append(newBox);
    (void) color;
}

void MainWindow::drawRightBox(QRect area, QColor color)
{
    QGraphicsPolygonItem* newBox = rightScene->addPolygon(QPolygonF() << area.topLeft() << area.topRight() << area.bottomRight() << area.bottomLeft());
    boxes.append(newBox);
    (void) color;
}

void MainWindow::drawLeftPoint(QPointF point, QColor col)
{
    (void) point;
    (void) col;
}

void MainWindow::drawRightPoint(QPointF point, QColor col)
{
    (void) point;
    (void) col;
}

void MainWindow::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
      {
        this->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
                this, SLOT(ShowContextMenu(const QPoint &)));
      }
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    if (selections[currentSelection]->index>-1){
        QModelIndex nIndex = mModelLeftCamera->index(currentSelection,1);
        onTableClick(nIndex);
    }
}

void MainWindow::onClippedImage(const QPixmap& pixmap)
{
    m_clippedLabel->setPixmap(pixmap);
}


void MainWindow::on_buttonBox_accepted()
{
}

void MainWindow::on_buttonBox_rejected()
{
    close();
}

void MainWindow::on_loadFromCustomPlotA_clicked()
{
    clearSceneLeft();
    QString dir = QFileDialog::getOpenFileName(this, tr("Open image directory"), "", tr("Images (*.jpg *.png *.jpeg *.bmp *.gif)"));
    QImage image;

    if(QString::compare(dir, QString()) != 0) {
        image = QImage(dir);
        currentLeftImagePath = dir;
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
        leftPix.append(item);
        leftScene->addItem(item);
        ui->leftView->show();
        ui->leftView->setSceneRect(QRectF(0, 0, image.width(), image.height()));
   }

}

void MainWindow::on_loadFromCustomPlotB_clicked()
{
    QString dir = QFileDialog::getOpenFileName(this, tr("Open image directory"), "", tr("Images (*.jpg *.png *.jpeg *.bmp *.gif)"));
    QImage image;
    if(QString::compare(dir, QString()) != 0) {
        image = QImage(dir);
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
        rightPix.append(item);
        rightScene->addItem(item);
   }
    ui->rightView->show();
    ui->rightView->setSceneRect(QRectF(0, 0, image.width(), image.height()));
}

void MainWindow::on_substrateIDGuideBtn_clicked()
{
    QMessageBox info;
    info.setWindowTitle("Substrate ID Guidelines");
    info.setStandardButtons(QMessageBox::Ok);
//    info.setIconPixmap(QPixmap(":/res/HabitatClassification.png"));
    info.setBaseSize(QSize(200, 200));
    info.setStyleSheet("QPushButton{ width:50px; font-size: 15px; }");
    info.show();
    info.exec();
}

void MainWindow::onRubberBandUpdate(const QRect &viewportRect, const QPointF &fromScenePoint, const QPointF &toScenePoint)
{
    if(viewportRect.isNull() && fromScenePoint.isNull() && toScenePoint.isNull() && imageLoaded)
    {
#if 0
        if(ui->zoomSelect->isChecked())
        {
            ui->leftView->fitInView(QRectF(start, end), Qt::KeepAspectRatio);
            ui->rightView->fitInView(QRectF(start, end), Qt::KeepAspectRatio);
        }
#endif
        //else
       // {
            if(currentSelection >= 0)
                selections[currentSelection]->setActiveState(false);
            QRectF select;
            select.setCoords(start.x(), start.y(), end.x(), end.y());

            DataRegion *region = new DataRegion(select);
            region->index = currentSelection+1;

            // link the box to the table
            leftCamParameters *boxParam = new leftCamParameters();
            SelectionData tmpdat = boxParam->getData();

            // getting the A-B-C-D Coordinates
            tmpdat.mACoord.setX((int)select.topLeft().x());
            tmpdat.mACoord.setY((int)select.topLeft().y());
            tmpdat.mBCoord.setX((int)select.topRight().x());
            tmpdat.mBCoord.setY((int)select.topRight().y());
            tmpdat.mCCoord.setX((int)select.bottomLeft().x());
            tmpdat.mCCoord.setY((int)select.bottomLeft().y());
            tmpdat.mDCoord.setX((int)select.bottomRight().x());
            tmpdat.mDCoord.setY((int)select.bottomRight().y());

            tmpdat.mPath = currentLeftImagePath;

            boxParam->setData(tmpdat);

            mDatabaseLeftCamera->addItem(boxParam);
            mModelLeftCamera->select();
            ui->recordLeftTableView->show();

            region->setActiveState(true);
            region->updateLabelText();
            connect(this, SIGNAL(onHideAll(bool)), region, SLOT(setBoxHiddenOverride(bool)));
            connect(this, SIGNAL(onHideInactive(bool)), region, SLOT(setBoxHidden(bool)));
            connect(this, SIGNAL(onShowAll(bool)), region, SLOT(setBoxHiddenOverride(bool)));

            currentSelection = selections.size();
            selections.append(region);
            leftScene->addItem(region->getGraphics());
            ui->leftView->show();
       // }
    }
    else
    {
        start = fromScenePoint;
        end = toScenePoint;
    }
}


void MainWindow::on_leftSidePan_toggled(bool checked)
{
    if(ui->leftSidePan->isEnabled())
    {
        if(checked)
        {
            ui->leftView->setDragMode(QGraphicsView::RubberBandDrag);
            ui->leftSidePan->setText("Left: Select");
        }
        else
        {
            ui->leftView->setDragMode(QGraphicsView::ScrollHandDrag);
            ui->leftSidePan->setText("Left: Drag");
        }
    }
}

void MainWindow::on_rightSidePan_toggled(bool checked)
{
    if(ui->rightSidePan->isEnabled())
    {
        if(checked)
        {
            ui->rightView->setDragMode(QGraphicsView::RubberBandDrag);
            ui->rightSidePan->setText("Right: Select");
        }
        else
        {
            ui->rightView->setDragMode(QGraphicsView::ScrollHandDrag);
            ui->rightSidePan->setText("Right: Drag");
        }
    }
}

//void MainWindow::on_tiedPan_toggled(bool checked)
//{
//    if(checked)
//    {
//        on_leftSidePan_toggled(true);
//        on_rightSidePan_toggled(true);

//        ui->leftSidePan->setEnabled(false);
//        ui->rightSidePan->setEnabled(false);
//        ui->tiedPan->setText("Both Select");
//    }
//    else
//    {
//        ui->leftSidePan->setEnabled(true);
//        ui->rightSidePan->setEnabled(true);
//        ui->tiedPan->setText("Separate");

//        on_leftSidePan_toggled(ui->leftSidePan->isChecked());
//        on_rightSidePan_toggled(ui->rightSidePan->isChecked());
//    }
//}

void MainWindow::on_addNewRecordBtn_clicked()
{
    leftScene->clearSelection();     // Selections would also render to the file
    leftScene->setSceneRect(leftScene->itemsBoundingRect());    // Re-shrink the scene to it's bounding contents

    ui->leftView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->leftView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QImage image = ui->leftView->grab().toImage();
    image.save(QFileDialog::getSaveFileName(this, tr("New Image Name"), QDir::rootPath(),
                                            "Name (*.jpg *.jpeg *.png *.tiff *.tif)"));

    ui->leftView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->leftView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}

void MainWindow::on_printScreenRight_clicked()
{
    rightScene->clearSelection();     // Selections would also render to the file
    rightScene->setSceneRect(rightScene->itemsBoundingRect());    // Re-shrink the scene to it's bounding contents
    QImage image(rightScene->sceneRect().size().toSize(), QImage::Format_ARGB32);  // Create the image with the exact size of the shrunk scene
    image.fill(Qt::transparent);     // Start all pixels transparent
    QPainter painter(&image);
    rightScene->render(&painter);
    image.save(QFileDialog::getSaveFileName(this, tr("New Image Name"), QDir::rootPath(),
                                            "Name (*.jpg *.jpeg *.png *.tiff *.tif)"));
}


void MainWindow::setName(QString tableLeft)
{
    workingTableLeft = tableLeft;
    mModelLeftCamera = new QSqlTableModel(this);
    mModelLeftCamera->setTable(workingTableLeft);
    mModelLeftCamera->select();
    ui->recordLeftTableView->setModel(mModelLeftCamera);
    ui->recordLeftTableView->setItemDelegate(new ImageDelegate(this));
}

void MainWindow::on_actionNew_File_triggered()
{
    openDatabase(SAVING);
}

void MainWindow::on_actionSave_As_triggered()
{
    openDatabase(OPENING);
}

void MainWindow::openDatabase(MainWindow::Opening opening) {
    QString nameDatabase;
    if(opening == OPENING) {
        nameDatabase = QFileDialog::getOpenFileName(this,
                                                    "Open Database", QDir::rootPath(),
                                                    "Database (*.db);;Any type (*.*)");
    } else {
        nameDatabase = QFileDialog::getSaveFileName(this,
                                                    "New Database", QDir::rootPath(),
                                                    "Database (*.db);;Any type (*.*)");
    }
    if (nameDatabase.isEmpty()) {
        return;
    }
    if(!mDatabaseLeftCamera->inizializationDatabaseLeftCamera(nameDatabase)) {
         QMessageBox::critical(this, "Error", getErrorLeft());
         return;
    }
    if(!mDatabaseLeftCamera->configurationDatabaseLeftCamera()) {
        QMessageBox::critical(this, "Error", getErrorLeft());
        return;
    }
    delete mModelLeftCamera;

    // Working with the database file created
    mModelLeftCamera = new QSqlTableModel(this, mDatabaseLeftCamera->getDatabase());
    mModelLeftCamera->setTable("leftCamTable");
    mModelLeftCamera->select();
    ui->recordLeftTableView->setModel(mModelLeftCamera);
    ui->recordLeftTableView->setItemDelegate(new ImageDelegate(this));
    ui->recordLeftTableView->showColumn(true);
}

//void MainWindow::on_actionWindowing_triggered()
//{
//    mWindowingDialog = new windowingDialog(this);
//    mWindowingDialog->show();
//}

void MainWindow::on_pushButton_3_clicked()
{
    mConnectionMenu = new connectionMenu(this);
    mConnectionMenu->show();
}

void MainWindow::on_leftCamMenuComboBox_currentIndexChanged(int index)
{
    switch (index)
    {
    case(0):
        onShowAll(true);
        break;
    case(1):
        onHideInactive(false);
        selections[currentSelection]->setVisible(true);
        break;
    case(2):
        onHideAll(false);
        break;
    default:
        break;
    }
    ui->leftView->show();
}

void MainWindow::clearSceneLeft()
{
    if (selections.size() > 0) {
        qDeleteAll(selections);
        selections.clear();
        currentSelection = -1;
    }
    for(int p=0;p<leftMatchPoints.size();p++)
    {
        leftScene->removeItem(leftMatchPoints[p]);
        delete leftMatchPoints[p];
    }
    leftMatchPoints.clear();
}

void MainWindow::clearSceneRight()
{
    if (selections.size() > 0) {
        qDeleteAll(selections);
        selections.clear();
        currentSelection = -1;
    }

    for(int p=0;p<rightMatchPoints.size();p++)
    {
        rightScene->removeItem(rightMatchPoints[p]);
        delete rightMatchPoints[p];
    }

    rightMatchPoints.clear();
}

void MainWindow::on_connectCheckBox_toggled(bool checked)
{
    if(checked)
    {
        ui->connectCheckBox->setText("Connected");
        connect(ui->leftView->verticalScrollBar(), SIGNAL(valueChanged(int)),
                ui->rightView->verticalScrollBar(), SLOT(setValue(int)));
        connect(ui->leftView->horizontalScrollBar(), SIGNAL(valueChanged(int)),
                ui->rightView->horizontalScrollBar(), SLOT(setValue(int)));
    }
    else
    {
        ui->connectCheckBox->setText("Disconnected");
        disconnect(ui->leftView->verticalScrollBar(), SIGNAL(valueChanged(int)),
                ui->rightView->verticalScrollBar(), SLOT(setValue(int)));
        disconnect(ui->leftView->horizontalScrollBar(), SIGNAL(valueChanged(int)),
                ui->rightView->horizontalScrollBar(), SLOT(setValue(int)));
    }
}

QPoint MainWindow::parseCoordString(QString input)
{
    QPoint output;

    if(input.contains('('))
    {
        output.setX(input.remove('(').remove(')').remove(',').split(" ")[0].toInt());
        output.setY(input.remove('(').remove(')').remove(',').split(" ")[1].toInt());
    }
    else
    {
        output.setX(input.split(":")[0].toInt());
        output.setY(input.split(":")[1].toInt());
    }

    return output;
}

void MainWindow::onTableClick(const QModelIndex &index)
{
    int row = index.row();
    SelectionData currentData;

    //if(row == -1) return;

    int ItemId = index.sibling(row, 0).data().toInt();
    currentData.mName = index.sibling(row, 1).data().toString();
    currentData.mLatitude = index.sibling(row, 2).data().toDouble();
    currentData.mLongitude = index.sibling(row, 3).data().toDouble();
    currentData.mDepth = index.sibling(row, 4).data().toDouble();
    currentData.mLength = index.sibling(row, 5).data().toDouble();

    currentData.mACoord.setX(parseCoordString(index.sibling(row,10).data().toString()).x());
    currentData.mACoord.setY(parseCoordString(index.sibling(row,10).data().toString()).y());
    currentData.mBCoord.setX(parseCoordString(index.sibling(row,11).data().toString()).x());
    currentData.mBCoord.setY(parseCoordString(index.sibling(row,11).data().toString()).y());
    currentData.mCCoord.setX(parseCoordString(index.sibling(row,12).data().toString()).x());
    currentData.mCCoord.setY(parseCoordString(index.sibling(row,12).data().toString()).y());
    currentData.mDCoord.setX(parseCoordString(index.sibling(row,13).data().toString()).x());
    currentData.mDCoord.setY(parseCoordString(index.sibling(row,13).data().toString()).y());

    currentData.mPrimarySub = index.sibling(row, 6).data().toString();
    currentData.mSecondarySub = index.sibling(row, 7).data().toString();
    currentData.mSpecies = index.sibling(row, 8).data().toString();
    currentData.mAddParam = index.sibling(row, 9).data().toString();
    currentData.mAvgRed = index.sibling(row,15).data().toDouble();
    currentData.mAvgGreen = index.sibling(row,16).data().toDouble();
    currentData.mAvgBlue = index.sibling(row,17).data().toDouble();
    currentData.mSave = index.sibling(row, 18).data().toByteArray();
    currentData.mPath = index.sibling(row, 19).data().toString();
    currentData.mDateTime = index.sibling(row, 20).data().toDateTime();
    currentData.firstXA = index.sibling(row, 21).data().toDouble();
    currentData.firstYA = index.sibling(row, 22).data().toDouble();
    currentData.secondXA = index.sibling(row, 23).data().toDouble();
    currentData.secondYA = index.sibling(row, 24).data().toDouble();

    QPixmap iconPix;
    if(!iconPix.loadFromData(index.sibling(row, 18).data().toByteArray())) {
    }
    QPixmap iconPix2;
    if(!iconPix2.loadFromData(index.sibling(row, 14).data().toByteArray())) {
    }

    start.setX(currentData.mACoord.x());
    start.setY(currentData.mACoord.y());
    end.setX(currentData.mDCoord.x());
    end.setY(currentData.mDCoord.y());

    if(currentSelection >= 0)
        selections[currentSelection]->setActiveState(false);

        // Used to get the correct box activation
    if (currentLeftImagePath != currentData.mPath) {
        return;
    }

    currentSelection = ItemId-1;
    selections[currentSelection]->setActiveState(true);

    QImage image(leftScene->sceneRect().size().toSize(), QImage::Format_ARGB32);  // Create the image with the exact size of the shrunk scene
    image.fill(Qt::transparent);     // Start all pixels transparent

    QPainter painter(&image);
    leftScene->render(&painter);
    QImage subimage = image.copy(QRect(start.toPoint(), end.toPoint()));

    clipScene d(this);
    d.setImage(subimage);
    d.setHeaderImage(currentLeftImagePath);
    //d.setBoundingBox(QRect(start.toPoint(), end.toPoint()));

    d.setData(currentData);
    d.setImage(iconPix.toImage());
    d.load_classifiedImageFromDb(iconPix2.toImage());

    d.setComboBoxes(ui->distAB->text(),
                    ui->primarySubComboBoxLeft->currentText(),
                    ui->secondarySubComboBoxLeft->currentText(),
                    ui->dateTimeEdit->dateTime(),
                    ui->leftCoordX->text(),
                    ui->leftCoordY->text(),
                    ui->pp_ax->text(),
                    ui->pp_ay->text());

//    d.setData(currentData);
//    d.setImage(iconPix.toImage());
//    d.load_classifiedImageFromDb(iconPix2.toImage());

    if(d.exec() == QDialog::Rejected) {
        return;
    } else {
        //
    }
    SelectionData result = d.getData();
    leftCamParameters* param = new leftCamParameters(result);
    param->setData(result);
    mDatabaseLeftCamera->updateItem(ItemId,param);
    mModelLeftCamera->select();
    ui->recordLeftTableView->show();

}

void MainWindow::onSceneDoubleClick(QPointF point)
{
    qDebug() << "Click!";
        QList<QGraphicsItem*> foundItems = leftScene->items(point);
        if(foundItems.size() > 0 && foundItems[0]->group() != nullptr)
        {
            qDebug() << "Found";
            int i = 0;
            for(i=0;i<selections.size();i++)
            {
                qDebug() << "Iterate";
                if(selections[i]->getGraphics() == foundItems[0]->group())
                {
                    qDebug() << "Target";
                    break;
                }
            }
            if(currentSelection >= 0)
                selections[currentSelection]->setActiveState(false);

            currentSelection = i;
            selections[currentSelection]->setActiveState(true);
        }
}

void MainWindow::on_primarySubComboBoxLeft_activated(const QString &arg1)
{
    if(currentSelection < 0) {
        QMessageBox::information(this, "Option not allowed yet", "Please upload images before using this selection, and/or "
                                                                 "capture region of interests before using this option");
        ui->primarySubComboBoxLeft->setCurrentText("Primary Substrate");
        return;
    } else {
       selections[currentSelection]->setPrimarySubText(arg1);
       selections[currentSelection]->updateLabelText();
    }
}

void MainWindow::on_secondarySubComboBoxLeft_activated(const QString &arg1)
{
    if (currentSelection < 0) {
        QMessageBox::information(this, "Option not allowed yet", "Please upload images before using this selection, and/or "
                                                                 "capture region of interests before using this option");
        ui->secondarySubComboBoxLeft->setCurrentText("Secondary Substrate");
        return;
    } else {
        selections[currentSelection]->setSecondarySubText(arg1);
        selections[currentSelection]->updateLabelText();
    }
}

//void MainWindow::on_comboBoxImgA_activated(const QString &arg1)
//{
//    if(currentSelection < 0) {
//        QMessageBox::information(this, "Option not allowed yet", "Please upload images before using this option");

//        ui->comboBoxImgA->setCurrentText("Select Operation");
//        return;
//    }
//}

void MainWindow::on_Extract_csv_file_triggered()
{
    auto filename = QFileDialog::getSaveFileName(this, "Save",
                                                 QDir::rootPath(), "TXT File(*.txt)");
    if(filename.isEmpty()) {
        return;
    }
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }
    QTextStream xout(&file);
    QStringList strList;
    const int rowCount = mModelLeftCamera->rowCount();
    const int colCount = mModelLeftCamera->columnCount();
    for(int ix = 0; ix < rowCount; ix++) {
        strList.clear();
        for(int jx = 1; jx < colCount; jx++) {
            QVariant data = ui->recordLeftTableView->model()->index(ix, jx).data();
            strList << data.toString();
        }
        xout << "\n";
        xout << strList.join(";")+"\n";
    }
    file.flush();
    file.close();
}

void MainWindow::on_loadPairA_clicked()
{
    main_graphFirst = QFileDialog::getExistingDirectory(this, tr("Choose an image directoy to load"),
            fileGraphLeft, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    imgadir = main_graphFirst;
    QDir drA(main_graphFirst);
    QStringList lA = drA.entryList(QDir::Files);
    ui->listWidget->addItems(lA);
    ui->listWidget->setCurrentRow(0);
    loadImagesGraphics(0);
}

void MainWindow::on_loadYMLFile_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Camera Cal File"),
                                       loadedDirectory, tr("yml (*.yml)"));
    yamlFileUpload(filename);
    match.clearCount();
//    searchingArea->hide();

}

void MainWindow::yamlFileUpload(QString& filename)
{
    if(filename != QString(""))
    {
        loadedDirectory = filename;
        match.readFile(filename.toStdString());
        ui->labelYmlCamA->setText("Yml file loaded");

        ui->loadPairA->setEnabled(true);
        ui->loadPairA->setStyleSheet("QPushButton{ background-color: green }");

        ui->loadPairB->setEnabled(true);
        ui->loadPairB->setStyleSheet("QPushButton{ background-color: green }");
    }
    else if(filename == QString(""))
    {
        QMessageBox::StandardButton q = QMessageBox::warning(this,"Error","Please upload .yml calibration file");
        return;
        Q_UNUSED(q);
    }
}

void MainWindow::loadImagesGraphics(int row)
{
    if(main_graphFirst.toStdString().size()>0&&main_graphSecond.toStdString().size()>0){
        if (main_graphFirst.toStdString().size() != main_graphSecond.toStdString().size())
        {
            QMessageBox::StandardButton q = QMessageBox::warning(this,"Error","Both list have different size");
            return;
            Q_UNUSED(q);
        }
        listPtr->setListsLoad(boost::filesystem::path(main_graphFirst.toStdString()),
                              boost::filesystem::path(main_graphSecond.toStdString()),BGR);
        //StereoCal rectified;
        //rectified.readFile("/home/emanuele/Desktop/cam_calib/abStereo.yml");

        yamlFileUpload(loadedDirectory);

        StereoImage im = listPtr->getImage(row);
        {
            int width = im.getB().getBitmap().size().width;

            ui->xSearchStart->setMinimum(1);
            ui->xSearchStart->setMaximum(width);
            ui->xSearchEnd  ->setMinimum(1);
            ui->xSearchEnd  ->setMaximum(width);

            ui->xSearchStart->setValue  (1);
            ui->xSearchEnd  ->setValue  (width);
        }

        im.getA().toGrey();
        im.getB().toGrey();
        //rectified.rectifyImages(im);

        match.rectifyImages(im);
        StereoImage imageRectified;

        imageRectified = match.getRectImages();
        //imageRectified = rectified.getRectImages();

        clearSceneLeft();
        QImage image = cvMat2QImage(imageRectified.getA().get8Bitmap());
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
        item->setZValue(0);
        leftScene->addItem(item);

        if(lgrid != nullptr)
        {
            leftScene->removeItem(lgrid);
            delete lgrid;
        }
        lgrid = new QGraphicsItemGroup();
        makeGrid(lgrid, image.rect());
        lgrid->setZValue(5);
        leftScene->addItem(lgrid);
        lgrid->setVisible(ui->checkBoxGridShow->isChecked());

        clearSceneRight();
        QImage imageB = cvMat2QImage(imageRectified.getB().get8Bitmap());
        QGraphicsPixmapItem* itemB = new QGraphicsPixmapItem(QPixmap::fromImage(imageB));
        itemB->setZValue(0);
        rightScene->addItem(itemB);

        if(rgrid != nullptr)
        {
            rightScene->removeItem(rgrid);
            delete rgrid;
        }
        rgrid = new QGraphicsItemGroup();
        makeGrid(rgrid, imageB.rect());
        rgrid->setZValue(5);
        rightScene->addItem(rgrid);
        rgrid->setVisible(ui->checkBoxGridShow->isChecked());

        ui->leftView->fitInView(leftScene->sceneRect(), Qt::KeepAspectRatio);
        ui->rightView->fitInView(rightScene->sceneRect(), Qt::KeepAspectRatio);

        ui->leftView->show();
        ui->rightView->show();

        imageLoaded = true;
    }
    return;    
}

void MainWindow::on_loadPairB_clicked()
{
    main_graphSecond = QFileDialog::getExistingDirectory(this, tr("Choose an image directoy to load"),
            fileGraphRight, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    QDir drB(main_graphSecond);
    QStringList lB = drB.entryList(QDir::Files);
    ui->listWidget_2->addItems(lB);
    ui->listWidget_2->setCurrentRow(0);
    loadImagesGraphics(0);

    ui->refreshBtn->setEnabled(true);
    ui->refreshBtn->setStyleSheet("QPushButton{ background-color: green }");

    ui->imageCroppedABtn->setEnabled(true);
    ui->leftArrowCroppedA->setEnabled(true);
    ui->rightArrowCroppedA->setEnabled(true);

    ui->imageCroppedBBtn->setEnabled(true);
    ui->leftArrowCroppedB->setEnabled(true);
    ui->rightArrowCroppedB->setEnabled(true);
}

void MainWindow::on_listWidget_currentRowChanged(int currentRow)
{
    if(ui->listWidget_2->count() > 0)
        ui->listWidget_2->setCurrentRow(currentRow);
}
void MainWindow::on_listWidget_2_currentRowChanged(int currentRow)
{
    if(ui->listWidget->count() > 0)
        ui->listWidget->setCurrentRow(currentRow);
    loadImagesGraphics(currentRow);
    searchingArea->hide();
}


bool MainWindow::matchImages(QPointF point) {
    //match.readFile("/home/emanuele/Desktop/cam_calib/abStereo.yml");
    //yamlFileUpload();
    boost::filesystem::path pa("/home/emanuele/Desktop/totalImages");

    StereoImage imagePair = listPtr->getImage(ui->listWidget->currentRow());
    imagePair.getA().toGrey();
    imagePair.getA().to8Bit();
    imagePair.getB().toGrey();
    imagePair.getB().to8Bit();
    match.rectifyImages(imagePair);
    match.setRegionSize(ui->regionSizeSpinBox->value());
    // Clear the previous match's data
//    ui->bestMatchPositionLineEdit->setText("--");
//    ui->bestMatchValueLineEdit->setText("--");
//    ui->disparityLineEdit->setText("--");
    //ui->SSDleftLineEdit->setText("--");

    match.controlXY(ui->xSearchStart->value(), ui->xSearchEnd->value()+1);

    match.clickPoint.x = (int)point.x();
    match.clickPoint.y = (int)point.y();

    QGraphicsRectItem* pntA = new QGraphicsRectItem(QRectF(match.clickPoint.x, match.clickPoint.y, 2, 2));
    pntA->setPen(QPen(Qt::green, 10));
    pntA->setZValue(6);
    leftMatchPoints.append(pntA);
    leftScene->addItem(pntA);

    cv::Point updatedPoint=match.checkPoint(match.clickPoint.x, match.clickPoint.y);
    ui->threshSpinBoxLeft->setValue(updatedPoint.x);

    // Area to search for on the second image
    searchArea->setRect(updatedPoint.x-ui->regionSizeSpinBox->value()/2, updatedPoint.y-ui->regionSizeSpinBox->value()/2, ui->regionSizeSpinBox->value(), ui->regionSizeSpinBox->value());
    searchArea->setZValue(1);
    ui->leftView->show();
    // Area we're searching on the second image
    searchingArea->setRect(0, updatedPoint.y-ui->regionSizeSpinBox->value()/2, ui->regionSizeSpinBox->value(), ui->regionSizeSpinBox->value());
    searchingArea->setZValue(1);
    searchingArea->show();
    ui->rightView->show();

    QApplication::processEvents();

    BestMatch best = match.findMatchedRegion(ui->matchProgressBarLeft, searchingArea,
                                             point.x(),point.y(),
                                             loadedDirectorySaved.toStdString(),
                                             (ui->checkBoxDetectClick->checkState()==Qt::Checked)?false:true,
                                             ui->threshSpinBoxRight->value());
//    ui->bestMatchPositionLineEdit->setText(QString::number(best.best));
//    ui->bestMatchValueLineEdit->setText(QString::number(best.errvec));
//    ui->disparityLineEdit->setText(QString::number(best.disparity));

    //ui->SSDleftLineEdit->setText(QString::number(best.errvec));
    ui->leftCoordX->setText(QString::number(best.calculatedPoint[0]));
    ui->leftCoordY->setText(QString::number(best.calculatedPoint[1]));
    ui->leftCoordDepth->setText(QString::number(best.calculatedPoint[2]));
    *lastMatchError = best.fullError;

    searchingArea->setPos(QPointF(best.best-(ui->threshSpinBoxRight->value()/2), searchingArea->pos().y()));
    QGraphicsRectItem* pnt = new QGraphicsRectItem(QRectF(best.best, updatedPoint.y, 2, 2));
    pnt->setPen(QPen(Qt::green, 10));
    pnt->setZValue(3);
    rightMatchPoints.append(pnt);
    rightScene->addItem(pnt);
    ui->rightView->show();

    StereoImage imageRectified;
    imageRectified = match.getRectImages();
    imageRectified.getA().setSavePath(pa);
    imageRectified.getB().setSavePath(pa);
    imageRectified.getA().save();
    imageRectified.getB().save();
}

void MainWindow::on_saveCropsImgsBtn_clicked()
{
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Save cropped images"),
                                       loadedDirectorySaved, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(!dirName.isEmpty())
    {
         loadedDirectorySaved = dirName;
         dirName.toStdString();
         ui->savedCropsLabelA->setText("Cropped images are currently saved in" + dirName);
         ui->loadYMLFile->setEnabled(true);
         ui->loadYMLFile->setStyleSheet("QPushButton{ background-color: green }");
    }
    else if(dirName == QString(""))
    {
        QMessageBox::StandardButton q = QMessageBox::warning(this,"Error","Please define images destination folder");
        return;
        Q_UNUSED(q);
    }
}

void MainWindow::on_toolButton_clicked()
{
    graph->setData(lastMatchError);
    if(graph->exec() == QDialog::Rejected) {
        return;
    } else {
        //
    }
}

bool MainWindow::eventFilter(QPointF point)
{
    cout << point.x() << "," << point.y()<< endl;
    switch(ui->comboBoxImgA->currentIndex())
    {
        case(0): /*matchImages(point); break;*/ // Nothing happens this is the "select one option"
            break;
        case(1):
        {
            // Move previously found point to storage
            ui->pp_ax->setText(ui->leftCoordX->text());
            ui->pp_ay->setText(ui->leftCoordY->text());
            ui->pp_az->setText(ui->leftCoordDepth->text());
            matchImages(point); // We have that, ckick and get the best match
            refreshDistance3D();
        }
        break;
    }
}

void MainWindow::refreshDistance3D()
{
    double ax = ui->leftCoordX->text().toDouble();
    double ay = ui->leftCoordY->text().toDouble();
    double az = ui->leftCoordDepth->text().toDouble();
    double bx = ui->pp_ax->text().toDouble();
    double by = ui->pp_ay->text().toDouble();
    double bz = ui->pp_az->text().toDouble();
    double distance = (sqrt(pow(bx-ax, 2) + pow(by-ay, 2) + pow(bz-az, 2)))/10;
    ui->distAB->setText(QString("%1").arg(distance));
}


void MainWindow::on_comboBoxImgA_currentIndexChanged(const QString &arg1)
{
//    if (arg1 == "Point")
//    {
//        leftScene->removeItem(newBox);
//        delete newBox;
//    }
}

void MainWindow::loadDirecory()
{
    QDirModelObj->setReadOnly(false);
    QDirModelObj->setSorting(QDir::DirsFirst | QDir::IgnoreCase | QDir::Name);
    //ui->lineEditfolderA->setModel(QDirModelObj);
    // https://stackoverflow.com/questions/45729339/printing-filenames-in-a-specific-folder-using-any-qt-widget
}


void MainWindow::on_imageCroppedABtn_clicked()
{
     cdir = QFileDialog::getExistingDirectory(this, tr("Choose an image directory to load"),
                              fileCamA, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
     if((cdir.isEmpty()) || (cdir.isNull()))
         return;
     croppedA.setPath(cdir);
     croppedFilenamesA = croppedA.entryList(QStringList() << "*.tiff" << "*.TIFF" << "*.tif" << "*.TIF" << "*.jpg" << "*.png", QDir::Files);
     croppedIndexA = 0;
     ui->labelCroppedA->setPixmap(QPixmap::fromImage(QImage(cdir + "/" + croppedFilenamesA[croppedIndexA]).scaled(125,125, Qt::KeepAspectRatio,Qt::SmoothTransformation)));
     ui->lineEditfolderA->setText(cdir + "/" + croppedFilenamesA[croppedIndexA]);
}

void MainWindow::on_rightArrowCroppedA_clicked()
{
    croppedIndexA++;
    if(croppedIndexA >= croppedFilenamesA.size())
        croppedIndexA = 0;

    if(croppedFilenamesA.size() > 0)
    {
        ui->lineEditfolderA->setText(cdir + "/" + croppedFilenamesA[croppedIndexA]);

        ui->labelCroppedA->setScaledContents(true);
        ui->labelCroppedA->setPixmap(QPixmap::fromImage(QImage(cdir + "/" + croppedFilenamesA[croppedIndexA]).scaled(125,125, Qt::KeepAspectRatio,Qt::SmoothTransformation)));
        ui->labelCroppedA->show();
    }
}

void MainWindow::on_leftArrowCroppedA_clicked()
{
    croppedIndexA--;
    if(croppedIndexA < 0)
        croppedIndexA = croppedFilenamesA.size()-1;

    if(croppedFilenamesA.size() > 0)
    {
        ui->lineEditfolderA->setText(cdir + "/" + croppedFilenamesA[croppedIndexA]);

        ui->labelCroppedA->setScaledContents(true);
        ui->labelCroppedA->setPixmap(QPixmap::fromImage(QImage(cdir + "/" + croppedFilenamesA[croppedIndexA]).scaled(125,125, Qt::KeepAspectRatio,Qt::SmoothTransformation)));
        ui->labelCroppedA->show();
    }
}

void MainWindow::on_imageCroppedBBtn_clicked()
{
     ddir = QFileDialog::getExistingDirectory(this, tr("Choose an image directory to load"),
                                                         fileCamB, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        if((ddir.isEmpty()) || (ddir.isNull()))
            return;

        croppedB.setPath(ddir);
        croppedFilenamesB = croppedB.entryList(QStringList() << "*.tiff" << "*.TIFF" << "*.tif" << "*.TIF" << "*.jpg" << "*.png", QDir::Files);
        croppedIndexB = 0;
        ui->labelCroppedB->setPixmap(QPixmap::fromImage(QImage(ddir + "/" + croppedFilenamesB[croppedIndexB]).scaled(125,125, Qt::KeepAspectRatio,Qt::SmoothTransformation)));
        //ui->lineEditfolderB->setText(croppedB.path());
        ui->lineEditfolderB->setText(ddir + "/" + croppedFilenamesB[croppedIndexB]);

}

void MainWindow::on_rightArrowCroppedB_clicked()
{
    croppedIndexB++;
    if(croppedIndexB >= croppedFilenamesB.size())
        croppedIndexB = 0;

    if(croppedFilenamesB.size() > 0)
    {
        ui->lineEditfolderB->setText(ddir + "/" + croppedFilenamesB[croppedIndexB]);

        ui->labelCroppedB->setScaledContents(true);
        ui->labelCroppedB->setPixmap(QPixmap::fromImage(QImage(ddir + "/" + croppedFilenamesB[croppedIndexB]).scaled(125,125, Qt::KeepAspectRatio,Qt::SmoothTransformation)));
        ui->labelCroppedB->show();
    }
}

void MainWindow::on_leftArrowCroppedB_clicked()
{
    croppedIndexB--;
    if(croppedIndexB < 0)
        croppedIndexB = croppedFilenamesB.size()-1;

    if(croppedFilenamesB.size() > 0)
    {
        ui->lineEditfolderB->setText(ddir + "/" + croppedFilenamesB[croppedIndexB]);

        ui->labelCroppedB->setScaledContents(true);
        ui->labelCroppedB->setPixmap(QPixmap::fromImage(QImage(ddir + "/" + croppedFilenamesB[croppedIndexB]).scaled(125,125, Qt::KeepAspectRatio,Qt::SmoothTransformation)));
        ui->labelCroppedB->show();
    }
}

void MainWindow::slotHelp()
{
    QMessageBox::information(this, trUtf8("Command Help"),
                             trUtf8("Detector Systsem\n"
                                    "The present user interface is used to find matches between images.\n"));
}

void MainWindow::clearData()
{
    for(int j = 0; j < 7; j++) {
        lineEditArray[j]->setText("--");
        for(int k = 0; k < 1; k++) {
            comboBoxArray[k]->setCurrentText("Select Operation");
            for(int i = 0; i < 1; i++) {
                comboPrimary[i]->setCurrentText("Primary Substrate");
                for(int c = 0; c < 1; c++) {
                    comboSecondary[c]->setCurrentText("Secondary Substrate");
                    for(int a = 0; a < 1; a++) {
                        comboColor[a]->setCurrentText("Red");
                    }
                }
            }
        }
    }
}

void MainWindow::createLineEditArray()
{
    lineEditArray[0] = ui->leftCoordX;
    lineEditArray[1] = ui->leftCoordY;
    lineEditArray[2] = ui->leftCoordDepth;
    lineEditArray[3] = ui->pp_ax;
    lineEditArray[4] = ui->pp_ay;
    lineEditArray[5] = ui->pp_az;
    lineEditArray[6] = ui->distAB;
}

void MainWindow::createComboBoxArray()
{
    comboBoxArray[0] = ui->comboBoxImgA;
}

void MainWindow::createComboPrimary()
{
    comboPrimary[0] = ui->primarySubComboBoxLeft;
}

void MainWindow::createComboSecondary()
{
    comboSecondary[0] = ui->secondarySubComboBoxLeft;
}

void MainWindow::createComboColor()
{
    comboColor[0] = ui->comboBoxColor;
}

void MainWindow::createArrays()
{
    createLineEditArray();
    createComboBoxArray();
    createComboPrimary();
    createComboSecondary();
    createComboColor();
}


void MainWindow::on_refreshBtn_clicked()
{
    clearData();
}

void MainWindow::on_leftCamMenuComboBox_activated(const QString &arg1)
{
    if(currentSelection < 0) {
        QMessageBox::information(this, "Option not allowed yet", "Please upload images before using this selection, and/or "
                                                                 "capture region of interests before using this option");
        ui->leftCamMenuComboBox->setCurrentText("All Objects");
        return;
    } else {
       //selections[currentSelection]->setPrimarySubText(arg1);
       selections[currentSelection]->updateLabelText();
    }
}

void MainWindow::on_comboBoxColor_activated(const QString &arg1)
{
    if(currentSelection < 0) {
        QMessageBox::information(this, "Option not allowed yet", "Please upload images before using this option");

        ui->comboBoxColor->setCurrentText("Choose Color(s)");
        return;
    } else {
        if(arg1 == "Red")
            selections[currentSelection]->updateColor(Qt::red, Qt::darkRed);
        if(arg1 == "Green")
            selections[currentSelection]->updateColor(Qt::green, Qt::darkGreen);
        if(arg1 == "Black")
            selections[currentSelection]->updateColor(Qt::darkGray, Qt::black);
        if(arg1 == "Yellow")
            selections[currentSelection]->updateColor(Qt::yellow, Qt::darkYellow);
        if(arg1 == "Orange")
            selections[currentSelection]->updateColor(QColor(247, 135, 0), QColor(117, 61, 0));
        if(arg1 == "Blue")
            selections[currentSelection]->updateColor(Qt::blue, Qt::darkBlue);

    }
}

void MainWindow::on_actionFit_triggered()
{
    ui->leftView->fitInView(leftScene->sceneRect(), Qt::KeepAspectRatio);
    ui->rightView->fitInView(leftScene->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::on_actionClose_triggered()
{
    close();
}

void MainWindow::on_fitBtn_clicked()
{
    ui->leftView->fitInView(leftScene->sceneRect(), Qt::KeepAspectRatio);
    ui->rightView->fitInView(leftScene->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::on_checkBoxGridShow_toggled(bool checked)
{
    if(lgrid != nullptr && rgrid != nullptr)
    {
        lgrid->setVisible(checked);
        rgrid->setVisible(checked);
    }
}

void MainWindow::on_checkBoxDetectClick_toggled(bool checked)
{
    if(checked)
    {
        ui->checkBoxDetectClick->setText("Interval Search");
        ui->boxDistCalc->setEnabled(true);
    }
    else
    {
        ui->checkBoxDetectClick->setText("Manual Search");
        ui->boxDistCalc->setEnabled(false);

    }
}


void MainWindow::on_listWidget_currentTextChanged(const QString &currentText)
{
    currentLeftImagePath = imgadir + "/" + currentText;
}

void MainWindow::makeGrid(
  QGraphicsItemGroup *qItemGrp, QRect rect,
  int gridSize, const QColor &gridColor)
{
  for (int x = rect.x(), xE = x + rect.width(); x < xE; x += gridSize) {
    QGraphicsLineItem *pQItem
      = new QGraphicsLineItem(x, rect.y(), x, rect.height() - 1);
    pQItem->setPen(gridColor);
    qItemGrp->addToGroup(pQItem);
  }
  for (int y = rect.y(), yE = y + rect.height(); y < yE; y += gridSize) {
    QGraphicsLineItem *pQItem
      = new QGraphicsLineItem(rect.x(), y, rect.width() - 1, y);
    pQItem->setPen(gridColor);
    qItemGrp->addToGroup(pQItem);
  }
}

void MainWindow::on_eraseLPointsBtn_clicked()
{
//    clearSceneLeft();
}

void MainWindow::on_eraseLBoxesBtn_clicked()
{
    // Erase from the db (ok works)
    int itemId = currentSelection;
    mDatabaseLeftCamera->removeItem(itemId);
    mModelLeftCamera->select();

    // Erase from QGraphicsView
    SelectionData *currentData;
    currentSelection = selections.size();
    leftScene->removeItem(itemId));
    ui->leftView->show();

//    DataRegion *indexRegion = getActiveRegion();
//    DataRegion *indexRegion = selections->getGraphics();

//    if(currentSelection >= 0)
//        selections[currentSelection]->setActiveState(true);

//    indexRegion->itemId();

//    leftScene->removeItem(region->getGraphics());
//    ui->leftView->show();

    // =========================================

    // Update the view related to the box region
    // grab the selected bos and erase it
//    QRectF select;
//    select.setCoords(start.x(), start.y(), end.x(), end.y());
//    DataRegion *region = new DataRegion(select);
//    region->index = currentSelection+1;
//    currentSelection = selections.size();
//    leftScene->removeItem(region->getGraphics());
//    ui->leftView->show();
}

void MainWindow::on_cutToolBtn_clicked()
{
    mCutToolMenu = new cuttingToolDialog(this);
    mCutToolMenu->show();
}

void MainWindow::on_eraseRPointsBtn_clicked()
{
    //clearSceneRight();
}


bool MainWindow::fileExists(QString path) {
    QFileInfo check_file(path);
    // check if file exists and if yes: Is it really a file and no directory?
    if (check_file.exists() && check_file.isFile()) {
        return true;
    } else {
        return false;
    }
}

void MainWindow::bothPrintScreen(const QString& pathImg, bool checkFolder){
        QString outA;
        do{
            outA = pathImg+"/printScreenA/"+ QString::number(counterA)+".png";
            counterA++;
        }
        while((checkFolder && fileExists(outA)));
        QImage imageA = ui->leftView->grab().toImage();
        imageA.save(outA);

        QString outB;
        do{
            outB = pathImg+"/printScreenB/"+ QString::number(counterB)+".png";
            counterB++;
        }
        while((checkFolder && fileExists(outB)));
        QImage imageB = ui->rightView->grab().toImage();
        imageB.save(outB);
}

void MainWindow::retreiveImage(QString path)
{
    if(path.length() > 0){
        try{
            QImage image(path);
        }catch(std::exception& e){
            // Image not found
        }
    }else{
            // Image not selected
    }
}

void MainWindow::on_bothPrintScreen_clicked()
{
    bothPrintScreen("/home/emanuele/Desktop/both", !owrite);
}


