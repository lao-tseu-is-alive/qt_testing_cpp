#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>

#include <QMenu>
#include <QLabel>
#include <QScrollArea>
#include <QScrollBar>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QPointF>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void open();
    void zoomIn();
    void zoomOut();
    void normalSize();
    void fitToWindow();

private:
    const static int WIN_WIDTH =  750;
    const static int WIN_HEIGTH = 500;

    Ui::MainWindow *ui;

    QAction *openAct;
    QAction *exitAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *normalSizeAct;
    QAction *fitToWindowAct;
    QMenu *fileMenu;
    QMenu *viewMenu;

    QString theFilename;
    QLabel *imageLabel;
    QGraphicsScene *scene;
    QGraphicsView *view;
    QGraphicsLineItem *line;
    QGraphicsRectItem *rectangle;
    QGraphicsEllipseItem *circle;
    QGraphicsEllipseItem *petal;
    QGraphicsPixmapItem *image;
    QGraphicsPolygonItem *polygon;
    QGraphicsPolygonItem *polygon2;

    //QScrollArea *scrollArea;
    double scene_width;
    double scene_heigth;
    double scaleFactor;
    QLabel *infoLabel;

    void createActions();
    void createMenus();
    void updateActions();
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);
    void createStatusBar();
    void updateStatusBar();
    void drawScene();
};

#endif // MAINWINDOW_H
