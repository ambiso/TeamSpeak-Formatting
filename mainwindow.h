#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QClipboard>

#include <v8/v8.h>
#include <v8/libplatform/libplatform.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, char const* _directory = "", const char *_scriptdir = "scripts/");
    ~MainWindow();

    void setMax(int max) { _maxlen = max; }
    void updateScriptFiles();

private slots:
    void on_inputText_textChanged();

    void on_compressionSlider_valueChanged(int value);

    void on_scriptComboBox_currentIndexChanged(int index);

    void on_refreshButton_clicked();

private:
    Ui::MainWindow *_ui;
    QClipboard *_clip;
    v8::Platform *_platform;
    v8::Isolate *_isolate;
    void infoText(QString str);
    char const *_directory;
    QString _scriptdir;

    int _compression;
    int _maxlen = 1024;
    void update();
};

#endif // MAINWINDOW_H
