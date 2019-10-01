#include "clipscene.h"
#include "../src/leftcamparameters.h"
#include "ui_clipscene.h"
#include "../src/imagedelegate.h"
#include "../ui/mainwindow.h"
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QBuffer>
#include <QImage>
#include <QVector>
#include <QRgb>
#include <QDataStream>
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <QSqlQuery>
#include <QDateTime>

using namespace cv;
using namespace std;


//inline QImage  cvMatToQImage( const cv::Mat &inMat )
//{
//    switch ( inMat.type() )
//    {
//    // 8-bit, 4 channel
//    case CV_8UC4:
//    {
//        QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB32);

//        return image;
//    }
//        // 8-bit, 3 channel
//    case CV_8UC3:
//    {
//        QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB888);
//        return image.rgbSwapped();
//        //return image;
//    }

//        // 8-bit, 1 channel
//    case CV_8UC1:
//    {
//        static QVector<QRgb>  sColorTable;
//        // only create our color table once
//        QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_Indexed8);
//        return image;
//    }
//    default:
//    {
//       break;
//    }
//    }
//    return QImage();
//}

//inline QPixmap cvMatToQPixmap( const cv::Mat &inMat )
//{
//    return QPixmap::fromImage( cvMatToQImage( inMat ) );
//}

inline QImage cvMat2QImage(cv::Mat image)
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


inline QPixmap cvMat2QPixmap(cv::Mat image)
{
  return QPixmap::fromImage(cvMat2QImage(image)).copy();
}



clipScene::clipScene(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::clipScene)
{
    ui->setupUi(this);

    scene = new QGraphicsScene();
    ui->graphicsViewClipped->setScene(scene);

    workingImageScene = new QGraphicsScene();
    ui->graphicsViewBBox->setScene(workingImageScene);

    ui->redImg->setText("<b>Calculate Graph</b>");
    ui->greenImg->setText("<b>Calculate Graph</b>");
    ui->blueImg->setText("<b>Calculate Graph</b>");
}

clipScene::~clipScene()
{
    delete ui;
    delete scene;
    delete workingImageScene;
}


void clipScene::setImage(QImage img)
{
    simg = img;
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(simg));
    scene->addItem(item);
    ui->graphicsViewClipped->show();
    cv::Mat input = cv::Mat(simg.height(), simg.width(), CV_16UC3 /*QImage::Format_RGB888*/, simg.bits(), simg.bytesPerLine());

    int bins = 256;             // number of bins
    int nc = input.channels();    // number of channels
    vector<Mat> hist(nc);       // array for storing the histograms
    vector<Mat> canvas(nc);     // images for displaying the histogram
    int hmax[3] = {0,0,0};      // peak value for each histogram

    // The rest of the code will be placed here
    for (int i = 0; i < hist.size(); i++)
        hist[i] = Mat::zeros(1, bins, CV_32SC1);

    for (int i = 0; i < input.rows; i++)
    {
        for (int j = 0; j < input.cols; j++)
        {
            for (int k = 0; k < nc; k++)
            {
                uchar val = nc == 1 ? input.at<uchar>(i,j) : input.at<Vec3b>(i,j)[k];
                hist[k].at<int>(val) += 1;
            }
        }
    }
    for (int i = 0; i < nc; i++)
    {
        for (int j = 0; j < bins-1; j++)
            hmax[i] = hist[i].at<int>(j) > hmax[i] ? hist[i].at<int>(j) : hmax[i];
        if(hmax[i] == 0) hmax[i] = 1;
    }

    const char* wname[3] = { "blue", "green", "red" };
    Scalar colors[3] = { Scalar(255,0,0), Scalar(0,255,0), Scalar(0,0,255) };

    ui->redImg->hide();
    ui->greenImg->hide();
    ui->blueImg->hide();

    for (int i = 0; i < nc; i++)
    {
        canvas[i] = Mat::ones(125, bins, CV_8UC3);

        for (int j = 0, rows = canvas[i].rows; j < bins-1; j++)
        {
            qDebug() << hmax[i] <<endl;
            line(
                        canvas[i],
                        Point(j, rows),
                        Point(j, rows - (hist[i].at<int>(j) * rows/hmax[i])),
                        nc == 1 ? Scalar(200,200,200) : colors[i],
                        1, 8, 0
                        );
        }

       // QPixmap imgIn = cvMatToQPixmap(canvas[i]);
        QPixmap imgIn = cvMat2QPixmap(canvas[i]);

        if ( i == 0 )
        {
            ui->redImg->setPixmap(imgIn);
            ui->redImg->setScaledContents(true);
            double meanRed = cv::mean(input)[0];
//            if(meanRed == 0.0)
//                QMessageBox::information(this, "Not allowed", "This image has zero mean red "
//                                                 "Please capture a different image");
                //return;
            ui->redLineIntensityEdit->setText(QString("%1").arg(meanRed));
            ui->redImg->show();
        }

        if ( i == 1 )
        {
            ui->greenImg->setPixmap(imgIn);
            ui->greenImg->setScaledContents(true);
            double meanGreen = cv::mean(input)[1];
//            if(meanGreen == 0.0)
//                QMessageBox::information(this, "Not allowed", "This image has zero mean green "
//                                                 "Please capture a different image");
                //return;
            ui->greenLineIntensityEdit->setText(QString("%1").arg(meanGreen));
            ui->greenImg->show();
        }

        if ( i == 2 )
        {
            ui->blueImg->setPixmap(imgIn);
            ui->blueImg->setScaledContents(true);
            double meanBlue = cv::mean(input)[2];
//            if(meanBlue == 0.0)
//                QMessageBox::information(this, "Not allowed", "This image has zero mean blue "
//                                                 "Please capture a different image");
                //return;
            ui->blueLineIntensityEdit->setText(QString("%1").arg(meanBlue));
            ui->blueImg->show();
        }
        //imshow(nc == 1 ? "value" : wname[i], canvas[i]);
    }
}

void clipScene::setHeaderImage(QString path)
{
    if(path.length() > 0){
        try{
            QImage image(path);
            ui->pathImage->setScaledContents(true);
            ui->pathImage->setPixmap(QPixmap::fromImage(image.scaled(125,96,Qt::KeepAspectRatio,Qt::SmoothTransformation)));
            ui->pathLineEdit->setText(path);
            ui->pathImage->show();
        }catch(std::exception& e){
           ui->pathImage->setText("<b>Image not found</b>");
        }
    }else{
        ui->pathImage->setText("<b>Image not selected</b>");
    }
}


void clipScene::setBoundingBox(QRect rect)
{
    srect = rect;
    ui->ACoord->setText(QString("(%1, %2)").arg(srect.topLeft().x()).arg(srect.topLeft().y()));
    ui->BCoord->setText(QString("(%1, %2)").arg(srect.topRight().x()).arg(srect.topRight().y()));
    ui->CCoord->setText(QString("(%1, %2)").arg(srect.bottomLeft().x()).arg(srect.bottomLeft().y()));
    ui->DCoord->setText(QString("(%1, %2)").arg(srect.bottomRight().x()).arg(srect.bottomRight().y()));
}

SelectionData clipScene::getData()
{
    return returnData;
}

void clipScene::setComboBoxes(const QString& length, const QString& primarySubstrate,
                              const QString& secondarySubstrate, const QDateTime &dateTime,
                              const QString& ptfirstX, const QString& ptfirstY,
                              const QString& ptSecondX, const QString& ptSecondY)
{
    ui->lengthLineEdit->setText(length);
    ui->primarySubstrateComboBox->setCurrentIndex(ui->primarySubstrateComboBox->findText(primarySubstrate));
    ui->secondarySubstrateComboBox->setCurrentIndex(ui->secondarySubstrateComboBox->findText(secondarySubstrate));
    ui->dateTimeEdit->setDateTime(dateTime);
    ui->firstXCoord->setText(ptfirstX);
    ui->firstYCoord->setText(ptfirstY);
    ui->secondXCoord->setText(ptSecondX);
    ui->secondYCoord->setText(ptSecondY);
}

void clipScene::setData(SelectionData newdata)
{
    returnData = newdata;
    if(returnData.mPath.length() > 0){
        try{
            QImage image(returnData.mPath);
            ui->pathImage->setScaledContents(true);
            ui->pathImage->setPixmap(QPixmap::fromImage(image.scaled(125,96,Qt::KeepAspectRatio,Qt::SmoothTransformation)));
            ui->pathLineEdit->setText(returnData.mPath);
            ui->pathImage->show();
        }catch(std::exception& e){
           ui->pathImage->setText("<b>Image not found</b>");
        }
    }else{
        ui->pathImage->setText("<b>Image not selected</b>");
    }
    ui->imageNameLineEdit->setText(returnData.mName);
    ui->latitudeLineEdit->setText(QString::number(returnData.mLatitude));
    ui->longitudeLineEdit->setText(QString::number(returnData.mLongitude));
    ui->depthLineEdit->setText(QString::number(returnData.mDepth));
    ui->lengthLineEdit->setText(QString::number(returnData.mLength));
    ui->primarySubstrateComboBox->setCurrentIndex(ui->primarySubstrateComboBox->findText(returnData.mPrimarySub));
    ui->secondarySubstrateComboBox->setCurrentIndex(ui->secondarySubstrateComboBox->findText(returnData.mSecondarySub));
    ui->speciesComboBox->setCurrentIndex(ui->speciesComboBox->findText(returnData.mSpecies));
    ui->addParamComboBox->setCurrentIndex(ui->addParamComboBox->findText(returnData.mAddParam));
    ui->dateTimeEdit->setDateTime(returnData.mDateTime);
    ui->firstXCoord->setText(QString::number(returnData.firstXA));
    ui->firstYCoord->setText(QString::number(returnData.firstYA));
    ui->secondXCoord->setText(QString::number(returnData.secondXA));
    ui->secondYCoord->setText(QString::number(returnData.secondYA));

    int ax = returnData.mACoord.x();
    int ay = returnData.mACoord.y();
    int dx = returnData.mDCoord.x();
    int dy = returnData.mDCoord.y();

    QPoint topLeft = QPoint(ax,ay);
    QPoint bottomRight = QPoint(dx,dy);
    QRect rect = QRect(topLeft,bottomRight);
    setBoundingBox(rect);
    ui->blueLineIntensityEdit->setText(QString::number(returnData.mAvgBlue));
    ui->greenLineIntensityEdit->setText(QString::number(returnData.mAvgGreen));
    ui->redLineIntensityEdit->setText(QString::number(returnData.mAvgRed));

    ui->noCBox->setCheckState((returnData.save)?Qt::Checked:Qt::Unchecked);



}

void clipScene::on_closeBtn_clicked()
{
    close();
}

void clipScene::on_acceptBtn_clicked()
{
    // Main data of the cells
    returnData.mPath = ui->pathLineEdit->text();
    returnData.mName = ui->imageNameLineEdit->text();
    returnData.mLatitude = ui->latitudeLineEdit->text().toDouble();
    returnData.mLongitude = ui->longitudeLineEdit->text().toDouble();
    returnData.mDepth = ui->depthLineEdit->text().toDouble();
    returnData.mLength = ui->lengthLineEdit->text().toDouble();
    returnData.mPrimarySub = ui->primarySubstrateComboBox->currentText();
    returnData.mSecondarySub = ui->secondarySubstrateComboBox->currentText();
    returnData.mSpecies = ui->speciesComboBox->currentText();
    returnData.mAddParam = ui->addParamComboBox->currentText();
    returnData.mDateTime = ui->dateTimeEdit->dateTime();
    returnData.firstXA = ui->firstXCoord->text().toDouble();
    returnData.firstYA = ui->firstYCoord->text().toDouble();
    returnData.secondXA = ui->secondXCoord->text().toDouble();
    returnData.secondYA = ui->secondYCoord->text().toDouble();

    // it should save the cropped image
    if(ui->noCBox->isChecked())
    {
        QPixmap img1;
        img1.convertFromImage(simg);
        QByteArray img1Array;
        QBuffer buffer1(&img1Array);
        buffer1.open(QIODevice::WriteOnly);
        img1.save(&buffer1, "TIFF");

        if(img1Array.isEmpty())
        {
            img1Array = QByteArray(5, '1');
        }

        returnData.mSave = img1Array;
    }
    // it should save the classified image
    QPixmap img2;
    img2.convertFromImage(clasImg);
    QByteArray img2Array;
    QBuffer buffer2(&img2Array);
    buffer2.open(QIODevice::WriteOnly);
    img2.save(&buffer2, "TIFF");
    returnData.mClassImg = img2Array;

    // Coordinates of the bunding box
    returnData.mACoord = srect.topLeft();
    returnData.mBCoord = srect.topRight();
    returnData.mCCoord = srect.bottomLeft();
    returnData.mDCoord = srect.bottomRight();

    // RGB intensities
    returnData.mAvgRed = ui->redLineIntensityEdit->text().toDouble();
    returnData.mAvgGreen = ui->greenLineIntensityEdit->text().toDouble();
    returnData.mAvgBlue = ui->blueLineIntensityEdit->text().toDouble();
    returnData.save = ui->noCBox->isChecked();
    accept();

}

void clipScene::showEvent(QShowEvent *) {
    if(ui->tabWidget->currentIndex() == 0) {
        ui->graphicsViewClipped->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    } else if(ui->tabWidget->currentIndex() == 1) {
        ui->graphicsViewBBox->fitInView(workingImageScene->sceneRect(), Qt::KeepAspectRatio);
    }
}

void clipScene::on_toolButtonPath_clicked()
{
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("Images (*.tif *.png *.jpg *.jpeg *.tiff *.tif)"));
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Images (*.tif *.png *.jpg *.jpeg *.tiff)"));
    if(!fileName.isEmpty()) {
        QImage image(fileName);
        ui->pathImage->setScaledContents(true);
        ui->pathImage->setPixmap(QPixmap::fromImage(image.scaled(125,96,Qt::KeepAspectRatio,Qt::SmoothTransformation)));
        ui->pathLineEdit->setText(fileName);
        ui->pathImage->show();
    }
}

void clipScene::on_loadBtnBBox_clicked()
{
    load_classifiedImage();
}

void clipScene::load_classifiedImageFromDb(QImage image)
{
    clasImg = image;
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    leftClipPix.append(item);
    workingImageScene->addItem(item);
    ui->graphicsViewBBox->show();
    ui->graphicsViewBBox->setSceneRect(QRectF(0, 0, image.width(), image.height()));
}

void clipScene::load_classifiedImage()
{
    QString dir = QFileDialog::getOpenFileName(this, tr("Open image directory"), "", tr("Images (*.tif *.jpg *.png *.jpeg *.bmp *.gif *.tiff)"));
    QImage image;
    if(QString::compare(dir, QString()) != 0) {
        image = QImage(dir);
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
        leftClipPix.append(item);
        workingImageScene->addItem(item);
    }
    ui->graphicsViewBBox->show();
    ui->graphicsViewBBox->setSceneRect(QRectF(0, 0, image.width(), image.height()));
    clasImg = image;
}

void clipScene::on_noCBox_toggled(bool checked)
{
    if(checked)
    {
        // Do not save cropped image and show "No image saved"
    }
}
