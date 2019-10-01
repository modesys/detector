#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMetaEnum>
#include <QTimer>
#include <QDir>
#include <QAction>
#include <QtSql/QSqlTableModel>
#include <QPointF>
#include <QLabel>
#include <QtSql/QSqlQuery>
#include <QMouseEvent>
#include <QLineEdit>
#include <QStandardItemModel>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPixmapItem>
#include <cmath>
#include <QListWidgetItem>
#include <QScrollBar>
#include <QComboBox>
#include <QDirModel>
#include <QFile>

#include "../src/leftcamparameters.h"
#include "../src/rightcamparameters.h"
#include "../src/dataleftcamera.h"
#include "../src/datarightcamera.h"
#include "../src/dataregion.h"
#include "../src/mgraphicsview.h"
#include "../src/graphicsviewbothdrag.h"
#include "../libCam/include/stereocal.h"
#include "../libCam/include/stereolist.h"
#include "connectionmenu.h"
//#include "windowingdialog.h"
#include "graphdialog.h"
#include "editlefttabledialog.h"
#include "colortabllecombobox.h"
#include "cuttingtooldialog.h"

#include "opencv2/imgproc.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/highgui/highgui.hpp"

namespace Ui {
class MainWindow;
}
class ClipScene;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:

    // Possibility of opening or saving a new database file (.db)
    enum Opening {
        OPENING,
        SAVING,
    };

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setName(QString tableLeft);
    void drawLeftBox(QRectF area, QColor color);
    void drawRightBox(QRect area, QColor color);
    void drawLeftPoint(QPointF point, QColor col);
    void drawRightPoint(QPointF point, QColor col);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);

    QString getErrorLeft() const { return mErrorLeft; }
    QString getErrorRight() const { return mErrorRight; }
    QString getError() const { return mError; }

    void initPrimarySubCBox(const QString presentPrimaryBoxLeft);
    void initSecondarySubCBox(const QString presentSecondaryBoxLeft);
    void initDrawBoxesCBox(const QString presentDrawingBoxSelection);

    void yamlFileUpload(QString& filename);
    //void saveCrops(QString& dirName);

    void colorSelected(const QString& text);

    bool fileExists(QString path);
    void bothPrintScreen(const QString &pathImg, bool checkFolder);

    void retreiveImage(QString path);






private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

    void on_loadFromCustomPlotA_clicked();
    void on_loadFromCustomPlotB_clicked();
    void on_substrateIDGuideBtn_clicked();
    void onRubberBandUpdate(const QRect &viewportRect, const QPointF &fromScenePoint, const QPointF &toScenePoint);

    void on_leftSidePan_toggled(bool checked);
    void on_rightSidePan_toggled(bool checked);
//    void on_tiedPan_toggled(bool checked);
    void onClippedImage(const QPixmap& pixmap);
    void on_addNewRecordBtn_clicked();

    void on_actionNew_File_triggered();
    void on_actionSave_As_triggered();
//    void on_actionWindowing_triggered();
    void on_leftCamMenuComboBox_currentIndexChanged(int index);
    void clearSceneLeft();
    void clearSceneRight();

    void on_imageCroppedABtn_clicked();
    void on_rightArrowCroppedA_clicked();
    void on_leftArrowCroppedA_clicked();
    void on_imageCroppedBBtn_clicked();
    void on_rightArrowCroppedB_clicked();
    void on_leftArrowCroppedB_clicked();

    void slotHelp();

signals:
    void editTable();
    void onHideAll(bool x);
    void onHideInactive(bool x);
    void onShowAll(bool x);
    void mouseReleased(QPointF point);
    void rightClick(QPointF point);

public slots:
    void loadDirecory();

private slots:
    void onSceneDoubleClick(QPointF point);
    void on_pushButton_3_clicked();

    //void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    //void on_listWidget_2_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void on_connectCheckBox_toggled(bool checked);
    void onTableClick(const QModelIndex &index);

    //void on_cBoxMeasures_currentIndexChanged(const QString &arg1);
    void on_Extract_csv_file_triggered();

    //void on_cBoxMeasures_currentIndexChanged(int index);

    void on_primarySubComboBoxLeft_activated(const QString &arg1);
    void on_secondarySubComboBoxLeft_activated(const QString &arg1);

    void on_loadPairA_clicked();
    void on_loadPairB_clicked();

    void on_listWidget_currentRowChanged(int currentRow);
    void on_listWidget_2_currentRowChanged(int currentRow);

    bool matchImages(QPointF point);
    bool eventFilter(QPointF);
    void refreshDistance3D();
    void on_comboBoxImgA_currentIndexChanged(const QString &arg1);
    void on_toolButton_clicked();
    void clearData();
    void on_refreshBtn_clicked();
    void on_leftCamMenuComboBox_activated(const QString &arg1);
    void on_loadYMLFile_clicked();
    void on_saveCropsImgsBtn_clicked();
    //void on_comboBoxColor_currentIndexChanged(const QPen &arg1);
    //void on_comboBoxImgA_activated(const QString &arg1);
    void on_comboBoxColor_activated(const QString &arg1);

    void on_actionFit_triggered();

    void on_actionClose_triggered();

    void on_fitBtn_clicked();

    void on_checkBoxGridShow_toggled(bool checked);

    void on_checkBoxDetectClick_toggled(bool checked);

    void on_printScreenRight_clicked();

    void on_listWidget_currentTextChanged(const QString &currentText);

    void on_eraseLPointsBtn_clicked();

    void on_eraseLBoxesBtn_clicked();

    void on_cutToolBtn_clicked();

    void on_eraseRPointsBtn_clicked();

    void on_bothPrintScreen_clicked();

private:

    bool totalCount;
    bool owrite = true;
    int counterA=0;
    int counterB=0;


    StereoCal match;
    ColorTablleComboBox *colorComboBox;
    QGraphicsRectItem* colorBox;

    QMap<QString, QColor> colors;

    void connections();
    //void load_imageLeft();
    //void load_imageRight();
    void loadImagesGraphics(int row);


    QString main_directoryFirst;
    QString main_directorySecond;

    QString filesListLeft;
    QString fileListRight;

    Ui::MainWindow *ui;
    MGraphicsScene* leftScene;

    QString main_graphFirst;
    QString main_graphSecond;
    QString fileGraphLeft;
    QString fileGraphRight;
    std::shared_ptr<StereoList> listPtr;
    QString loadedDirectory;
    QString loadedDirectorySaved;

    QString temporaryFolder;

    QDir dir;
    QString imgadir;

    Qt::KeyboardModifiers _modifiers;
    double _zoom_factor_base;
    QPointF target_scene_pos, target_viewport_pos;

    Qt::KeyboardModifiers _modifiersRight;
    double _zoom_factor_base_right;
    QPointF target_scene_pos_right, target_viewport_pos_right;

public:
    QList<DataRegion*> selections;
    int currentSelection;

private:
    leftCamParameters *mItemLeftCamParam;
    rightCamParameters *mItemRightCamParam;
    QString mErrorLeft;
    QString mErrorRight;
    QString mError;
    QSqlTableModel* mModel;
    dataRightCamera *mDatabaseRightCamera;
    dataLeftCamera *mDatabaseLeftCamera;

    QDirModel *QDirModelObj;


public:
    QSqlTableModel *mModelLeftCamera;
    void makeGrid(QGraphicsItemGroup *qItemGrp, QRect rect, int gridSize = 20, const QColor &gridColor = 0x0a8affu);
private:
    QSqlTableModel *mModelRightCamera;
    void openDatabase(Opening opening);
    QString workingTableLeft;
    QString workingTableRight;
    // Removing and adding records on the left table
    // Possibility to edit the left table
    QAction* mEditLeftTable;
    // Removing and adding records on the right table
    // Possibility to edit the right table
    QAction* mEditRightTable;
    // Graphic Scenes management
    QGraphicsScene *rightScene;

    QList<QGraphicsPixmapItem*> leftPix, rightPix;
    QList<QGraphicsPolygonItem*> boxes;
    QPointF start, end;
    QPointF point, col;
    bool imageLoaded;
    QGraphicsItemGroup *lgrid, *rgrid;

    QList<QGraphicsEllipseItem*> points;
    QGraphicsLineItem* pointline;

    QLabel* m_clippedLabel;  // A label in which the cropped image will be placed
    ClipScene* m_clipScene;  // The graphical scene in which the image trimming functionality is implemented

    cv::Mat original_image_left; //loaded image left
    cv::Mat image_left; //current image left
    cv::Mat original_image_right; // loaded image right
    cv::Mat image_right; // current image right

    connectionMenu *mConnectionMenu;
    cuttingToolDialog *mCutToolMenu;

//    windowingDialog *mWindowingDialog;
    QString currentLeftImagePath;

    QGraphicsRectItem* searchArea;
    QGraphicsRectItem* searchingArea;

    QList<QGraphicsRectItem*> rightMatchPoints;
    QList<QGraphicsRectItem*> leftMatchPoints;

    std::vector<double> *lastMatchError;
    graphDialog *graph;

    QGraphicsPolygonItem* newBox;

    QString cdir;
    QString ddir;
    QString fileCamA;
    QString fileCamB;
    QDir croppedA, croppedB;
    QStringList croppedFilenamesA, croppedFilenamesB;
    int croppedIndexA, croppedIndexB;

    // In order to refresh the data we will create a series of
    // arrays of comboboxes, labels and buttons
    QLineEdit *lineEditArray[7];
    QComboBox *comboBoxArray[1];
    QComboBox *comboPrimary[1];
    QComboBox *comboSecondary[1];
    QComboBox *comboColor[1];
    void createLineEditArray();
    void createComboBoxArray();
    void createComboPrimary();
    void createComboSecondary();
    void createComboColor();
    void createArrays();

    //bool ctrl_down = false; // to check if CTRL is pressed
    bool alt_down = false; // to check if ALT is pressed

//protected:
//    void wheelEvent(QWheelEvent *event);
//private:
//    void applyZoom(double factor, const QPoint &fixedViewPos);
//    Qt::KeyboardModifiers m_modifiers;
//    const double base = 1.0015;
//public:
//    void setModifiers(const Qt::KeyboardModifiers &modifiers);

    QPoint parseCoordString(QString input);
protected:
   //void drawBackground(QPainter * painter, const QRectF & rect );




};

#endif // MAINWINDOW_H
