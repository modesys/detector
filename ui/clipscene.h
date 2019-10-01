#ifndef CLIPSCENE_H
#define CLIPSCENE_H
#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QSqlError>
#include <QRegularExpression>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "../src/leftcamparameters.h"
#include "../detector/ui/mainwindow.h"

namespace Ui {
class clipScene;
}

class clipScene : public QDialog
{
    Q_OBJECT
public:
    explicit clipScene(QWidget *parent = nullptr);
    ~clipScene();
    void setImage(QImage img);
    void setHeaderImage(QString path);
    void setBoundingBox(QRect rect);

    void load_classifiedImageFromDb(QImage img);
    SelectionData getData();
    void setData(SelectionData newdata);
    QString getError() const { return mError; }

    void setComboBoxes(const QString& length, const QString& primarySubstrate,
                       const QString& secondarySubstrate, const QDateTime &dateTime,
                       const QString& ptfirstX, const QString& ptfirstY,
                       const QString& ptSecondX, const QString& ptSecondY);
//    void clipScene::setDate();
//    void clipScene::setTime();

    MainWindow *main_window;

private slots:
    void on_closeBtn_clicked();
    void on_acceptBtn_clicked();
    void showEvent(QShowEvent *);
    //void on_saveCroppedCBox_clicked();
    void load_classifiedImage();
    void on_toolButtonPath_clicked();
    void on_loadBtnBBox_clicked();
    void on_noCBox_toggled(bool checked);

private:
    SelectionData returnData;
    Ui::clipScene *ui;
    QImage simg, simgR, simgG, simgB,clasImg;
    QRect srect;
    QGraphicsScene *scene;
    QGraphicsScene *workingImageScene;
    QList<QGraphicsPixmapItem*> leftClipPix;
    cv::Mat image; //current image
    QString clipDirLeft;
    QString clipDir;
    QString mError;    


};
#endif // CLIPSCENE_H
