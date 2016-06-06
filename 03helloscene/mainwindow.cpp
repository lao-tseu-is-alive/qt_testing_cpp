#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    theFilename = "";
    scene_width = 100;
    scene_heigth = 100;
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    view  = new QGraphicsView(scene,this);
    view->setCacheMode(QGraphicsView::CacheBackground);
    view->setRenderHints(QPainter::Antialiasing);
    view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    view->setDragMode(QGraphicsView::ScrollHandDrag);
    setCentralWidget(view);
  /*
    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(view);
    setCentralWidget(scrollArea);
*/
    createStatusBar();
    createActions();
    createMenus();
    setWindowTitle(tr("03 Scene Viewer"));
    resize(WIN_WIDTH, WIN_HEIGTH);
    QMatrix m;
    //m.translate( w_2, h_2 );
    //let's invert QT Y-axis coordinate system (so that y grows upwards)
    m.scale( 1, -1 );
    view->setMatrix(m);
    view->setSceneRect(-scene_width/2,-scene_heigth/2,scene_width,scene_heigth);
    //qDebug()<< "viewport :"<< view-> ->Viewport() << endl;
    drawScene();
    view->centerOn(QPointF(0,0));


}

MainWindow::~MainWindow()
{
    delete ui;
    qDebug()<< "## About to leave : inside destructor ~MainWindow()" << endl;
}

void MainWindow::createActions()
{
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcut(tr("Ctrl+O"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    zoomInAct = new QAction(tr("Zoom &In (25%)"), this);
    zoomInAct->setShortcut(tr("Ctrl++"));
    zoomInAct->setEnabled(false);
    connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

    zoomOutAct = new QAction(tr("Zoom &Out (25%)"), this);
    zoomOutAct->setShortcut(tr("Ctrl+-"));
    zoomOutAct->setEnabled(false);
    connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));

    normalSizeAct = new QAction(tr("&Normal Size"), this);
    normalSizeAct->setShortcut(tr("Ctrl+S"));
    normalSizeAct->setEnabled(false);
    connect(normalSizeAct, SIGNAL(triggered()), this, SLOT(normalSize()));

    fitToWindowAct = new QAction(tr("&Fit to Window"), this);
    fitToWindowAct->setEnabled(false);
    fitToWindowAct->setCheckable(true);
    fitToWindowAct->setShortcut(tr("Ctrl+F"));
    connect(fitToWindowAct, SIGNAL(triggered()), this, SLOT(fitToWindow()));

}

void MainWindow::createMenus()
{
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    viewMenu = new QMenu(tr("&View"), this);
    viewMenu->addAction(zoomInAct);
    viewMenu->addAction(zoomOutAct);
    viewMenu->addAction(normalSizeAct);
    viewMenu->addSeparator();
    viewMenu->addAction(fitToWindowAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(viewMenu);
}


void MainWindow::createStatusBar()
{
    infoLabel = new QLabel(theFilename);
    infoLabel->setAlignment(Qt::AlignHCenter);
    infoLabel->setMinimumSize(infoLabel->sizeHint());

    ui->statusBar->addWidget(infoLabel);
    updateStatusBar();
}

void MainWindow::updateStatusBar()
{
    QString strInfo = QString::fromLatin1("Current file: %1 ").arg(theFilename);
    infoLabel->setText(strInfo);
}

void MainWindow::open()
{
    theFilename = QFileDialog::getOpenFileName(this,
                                    tr("Open File"), QDir::currentPath());
    if (!theFilename.isEmpty()) {
        QImage image(theFilename);
        if (image.isNull()) {
            QMessageBox::information(this, tr("Image Viewer"),
                                     tr("Cannot load %1.").arg(theFilename));
            return;
        }
        view->setBackgroundBrush(QPixmap::fromImage(image));
        //view.setCacheMode(QGraphicsView::CacheBackground);
        //qDebug()<< "## Opening %1 of size [%2]".arg(theFilename,view->backgroundBrush().textureImage().size()) << endl;
        //imageLabel->setPixmap(QPixmap::fromImage(image));
        scaleFactor = 1.0;

        fitToWindowAct->setEnabled(true);
        updateActions();
        updateStatusBar();

        if (!fitToWindowAct->isChecked())
            view->adjustSize();
            //imageLabel->adjustSize();
    }
}

void MainWindow::zoomIn()
{
    scaleImage(1.25);
}

void MainWindow::zoomOut()
{
    scaleImage(0.8);
}

void MainWindow::normalSize()
{
    //imageLabel->adjustSize();
    view->adjustSize();
    scaleFactor = 1.0;
}

void MainWindow::fitToWindow()
{
    bool fitToWindow = fitToWindowAct->isChecked();
    //scrollArea->setWidgetResizable(fitToWindow);
    if (!fitToWindow) {
        normalSize();
    }
    updateActions();
}

void MainWindow::updateActions()
{
    zoomInAct->setEnabled(!fitToWindowAct->isChecked());
    zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
    normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
}

void MainWindow::scaleImage(double factor)
{
    //Q_ASSERT(view->backgroundBrush().textureImage());
    scaleFactor *= factor;
    view->resize(scaleFactor * view->backgroundBrush().textureImage().size());
    zoomInAct->setEnabled(scaleFactor < 3.0);
    zoomOutAct->setEnabled(scaleFactor > 0.333);
}

void MainWindow::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    //scrollBar->setValue(int(factor * scrollBar->value()+ ((factor - 1) * scrollBar->pageStep()/2)));
}

void MainWindow::drawScene()
{
    QBrush brushRed(Qt::red);
    QBrush brushGreen(Qt::green);
    QBrush brushBlue(Qt::blue);
    QPen   pen(Qt::black);

    //QFont font("Arial",12,5,true);

    double centerx = scene_width / 2;
    double centery = scene_heigth / 2;
    double tick_spacer = 10.0;
    qDebug()<< "scene_width :" << scene_width << endl;
    qDebug()<< "scene_heigth :" << scene_heigth << endl;
    qDebug()<< "tick_spacer :" << tick_spacer << endl;
    //scene->addText("Welcome to world !");
    pen.setWidth(2);
    //vertical axes
    line = scene->addLine(0,-tick_spacer,0,centery,pen);
    pen.setWidth(1);
    for (int i=0; i < 10;i++)
    {
        scene->addLine(-5,i*tick_spacer,5,i*tick_spacer,pen);
    }
    pen.setWidth(2);
    //horizontal axe centered
    line = scene->addLine(-tick_spacer,0,centerx,0,pen);
    pen.setWidth(1);
    for (int i=0; i < 10;i++)
    {
        scene->addLine(i*tick_spacer,-5,i*tick_spacer,5,pen);
    }

    pen.setWidth(4);
    rectangle = scene->addRect(10,10,60,50,pen,brushRed);
    //rectangle->shear(0.3,1);
    rectangle->setFlag(QGraphicsItem::ItemIsMovable);
    circle = scene->addEllipse(-25,-25,50,50,pen,brushBlue);

    int numPetals = 8;
    double angle_rotation = 360 / numPetals;
    for (int i=0;i<numPetals;i++)
    {
        petal = new QGraphicsEllipseItem(20,-15,50,30,circle);
        petal->setBrush(brushGreen);
        petal->setPen(pen);
        petal->setRotation(angle_rotation*i);
        scene->addItem(petal);
    }

    circle->setPos(QPointF(40,150));
    circle->setParentItem(rectangle);

    circle->setFlag(QGraphicsItem::ItemIsMovable);

    QPolygonF triangle;
    pen.setWidth(3);
    triangle << QPointF(0,0) << QPointF(-50,50) << QPointF(50,50);
    polygon = scene->addPolygon(triangle,pen);
    polygon->setPos(QPointF(100,-100));
    polygon->setRotation(45);
    polygon2 = scene->addPolygon(triangle,pen,brushBlue);
    polygon2->setPos(QPointF(-100,100));


}
