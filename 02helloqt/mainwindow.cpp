#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createStatusBar();
    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
    ui->menuBar->addMenu(fileMenu);

}

MainWindow::~MainWindow()
{
    delete ui;
    qDebug()<< "## About to leave : inside destructor ~MainWindow()" << endl;
}

void MainWindow::on_cmdOK_clicked()
{
    ui->txtNameUpper->setText(ui->txtName->text().toUpper());
}

void MainWindow::on_cmdQuit_clicked()
{
    this->close();
}

void MainWindow::on_txtName_textChanged(const QString &arg1)
{

    ui->txtNameUpper->setText(arg1.toUpper());
    updateStatusBar();
}

void MainWindow::createStatusBar()
{
    infoLabel = new QLabel(" 0 chars   ");
    infoLabel->setAlignment(Qt::AlignHCenter);
    infoLabel->setMinimumSize(infoLabel->sizeHint());

    ui->statusBar->addWidget(infoLabel);
    updateStatusBar();
}

void MainWindow::updateStatusBar()
{
    QString strInfo = QString::fromLatin1("Your name is %1 chars long").arg(ui->txtName->text().length());
    infoLabel->setText(strInfo);
}
