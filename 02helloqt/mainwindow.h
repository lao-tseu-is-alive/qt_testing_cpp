#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
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
    void on_cmdOK_clicked();

    void on_cmdQuit_clicked();

    void on_txtName_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    QAction *exitAct;
    QMenu *fileMenu;
    QLabel *infoLabel;

    void createStatusBar();
    void updateStatusBar();
};

#endif // MAINWINDOW_H
