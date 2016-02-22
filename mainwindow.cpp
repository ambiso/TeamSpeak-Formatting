#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QTextStream>
#include <QVBoxLayout>
#include <QFile>
#include <utility>
#include <QRegExp>

#include <iostream>
#include <memory>
#include <vector>
#include <set>
#include <stack>
#include <cassert>

#include <v8/v8.h>
#include <v8/libplatform/libplatform.h>

using namespace v8;

#include "format.h"
#include "color.h"
#include "formatting.h"
#include "fchar.h"

class ArrayBufferAllocator : public v8::ArrayBuffer::Allocator {
 public:
  virtual void* Allocate(size_t length) { return calloc(length, 1); }
  virtual void* AllocateUninitialized(size_t length) { return malloc(length); }
  virtual void Free(void* data, size_t) { free(data); }
};

MainWindow::MainWindow(QWidget *parent, char const* directory) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _clip = QApplication::clipboard();
    V8::InitializeICU();
    V8::InitializeExternalStartupData(directory);
    _platform = platform::CreateDefaultPlatform();
    V8::InitializePlatform(_platform);
    V8::Initialize();

    // Create a new Isolate and make it the current one.
    ArrayBufferAllocator allocator;
    Isolate::CreateParams create_params;
    create_params.array_buffer_allocator = &allocator;
    _isolate = Isolate::New(create_params); //Isolate::GetCurrent();
    _ui->setupUi(this);
    QObject::connect(_ui->compressionSlider, SIGNAL(valueChanged(int)), _ui->compressionSpin, SLOT(setValue(int)));
    QObject::connect(_ui->compressionSpin, SIGNAL(valueChanged(int)), _ui->compressionSlider, SLOT(setValue(int)));
}

MainWindow::~MainWindow()
{
    //isolate->Dispose();
    V8::Dispose();
    V8::ShutdownPlatform();
    delete _platform;
    delete _ui;
}

void MainWindow::update() {
    if(_ui->scriptComboBox->count() == 0) {
        infoText("Please add a script to format");
        return;
    }
    QFile srcf(_ui->scriptComboBox->currentText());
    srcf.open(QIODevice::ReadOnly);
    if(!srcf.isOpen()) {
        infoText("Could not load script");
        return;
    }
    QTextStream stream(&srcf);
    QString src = stream.readAll();
    srcf.close();

    Isolate::Scope isolate_scope(_isolate);
    HandleScope handle_scope(_isolate);
    Local<Context> context = Context::New(_isolate);
    Context::Scope context_scope(context);

    Local<Value> italicString = String::NewFromUtf8(_isolate, "italic", NewStringType::kNormal).ToLocalChecked();
    Local<Value> boldString = String::NewFromUtf8(_isolate, "bold", NewStringType::kNormal).ToLocalChecked();
    Local<Value> underlineString = String::NewFromUtf8(_isolate, "underline", NewStringType::kNormal).ToLocalChecked();
    Local<Value> strikeString = String::NewFromUtf8(_isolate, "strike", NewStringType::kNormal).ToLocalChecked();
    Local<Value> rString = String::NewFromUtf8(_isolate, "r", NewStringType::kNormal).ToLocalChecked();
    Local<Value> gString = String::NewFromUtf8(_isolate, "g", NewStringType::kNormal).ToLocalChecked();
    Local<Value> bString = String::NewFromUtf8(_isolate, "b", NewStringType::kNormal).ToLocalChecked();

    MaybeLocal<String> source =
            String::NewFromUtf8(_isolate, src.toLatin1().data(),
                                NewStringType::kNormal);
    if(source.IsEmpty()) {
        infoText("Could not store script");
        return;
    }
    MaybeLocal<Script> script = Script::Compile(context, source.ToLocalChecked());
    if(script.IsEmpty()) {
        infoText("Could not compile script");
        return;
    }
    QString input = _ui->inputText->text();
    script.ToLocalChecked()->Run(context).ToLocalChecked();
    MaybeLocal<String> argm = String::NewFromUtf8(_isolate, input.toUtf8().data(), NewStringType::kNormal);
    if(argm.IsEmpty()) {
        infoText("Failed to prepare string for init()");
        return;
    }
    Local<Value> arg = argm.ToLocalChecked();
    Local<Function> init = Local<Function>::Cast(
                context->Global()->Get( //Get a function
                    context, String::NewFromUtf8(
                        _isolate, "init", NewStringType::kNormal).ToLocalChecked()) //Called init
                .ToLocalChecked());
    init->Call(context, context->Global(), 1, &arg);

    Local<Function> format = Local<Function>::Cast(
                context->Global()->Get( //Get a function
                    context, String::NewFromUtf8(
                        _isolate, "format", NewStringType::kNormal).ToLocalChecked()) //Called format
                .ToLocalChecked());

    int pos = 0, msglen = input.size();
    FChar formatted[msglen];
    foreach(QChar c, input) {
        argm = String::NewFromUtf8(_isolate, QString(c).toUtf8().data(), NewStringType::kNormal);
        if(argm.IsEmpty()) {
            infoText("Failed to prepare char argument for format()");
            return;
        }
        arg = argm.ToLocalChecked();
        MaybeLocal<Value> formatmlv = format->Call(context, context->Global(), 1, &arg);
        if(formatmlv.IsEmpty()) {
            infoText("Could not call format()");
            return;
        }
        Local<Value> formatlv = formatmlv.ToLocalChecked();
        if(!formatlv->IsObject()) {
            infoText("Invalid type - expected Object");
            return;
        }
        MaybeLocal<Object> formatmlo = formatlv->ToObject(context);
        if(formatmlo.IsEmpty()) {
            infoText("Failed to retrieve object from format()");
            return;
        }
        Local<Object> formatlo = formatmlo.ToLocalChecked();
        bool italic = formatlo->Get(context, italicString).ToLocalChecked()->BooleanValue(context).FromJust();
        bool bold = formatlo->Get(context, boldString).ToLocalChecked()->BooleanValue(context).FromJust();
        bool underline = formatlo->Get(context, underlineString).ToLocalChecked()->BooleanValue(context).FromJust();
        bool strike = formatlo->Get(context, strikeString).ToLocalChecked()->BooleanValue(context).FromJust();
        unsigned char red = formatlo->Get(context, rString).ToLocalChecked()->Int32Value(context).FromJust();
        unsigned char green = formatlo->Get(context, gString).ToLocalChecked()->Int32Value(context).FromJust();
        unsigned char blue = formatlo->Get(context, bString).ToLocalChecked()->Int32Value(context).FromJust();

        formatted[pos++] = FChar(c, Format(Color(red, green, blue), bold, italic, underline, strike));
    }

    FChar prev;
    for(int i = 0; i < msglen; ++i) {
        FChar &cur = formatted[i];
        //apply color compression
        if(!cur.format().getColor().diff(prev.format().getColor(), _compression)) {
            cur.format().setColoren(false);
            cur.format().setColor(prev.format().getColor());
        }
        for(int j = 0; j < Format::numProperties(); ++j) {
            if(prev.getFormat().getAttrib(j) != cur.getFormat().getAttrib(j)) {
                cur.format().setAttribEn(j, true); //enable attrib if it differs from previous character's
            }
        }
        prev = cur;
    }
    QString converted = formatToString(formatted, msglen, 20, _compression);
    _clip->setText(converted);
    int length = converted.toUtf8().size() + converted.count(QRegExp("[\\/ ]"));
    infoText("Length: " + QString::number(length));
    if(length > _maxlen) {
        _ui->infoLabel->setStyleSheet("QLabel{color:red}");
    }
}

void MainWindow::on_inputText_textChanged()
{
    update();
}

void MainWindow::on_compressionSlider_valueChanged(int value)
{
    int tmp = _compression;
    _compression = value;
    if(tmp != _compression) {
        update();
    }
}

void MainWindow::on_addButton_clicked()
{
    QStringList scripts = QFileDialog::getOpenFileNames(this, tr("Open Script"), "", tr("JS Files (*.js)"));
    _ui->scriptComboBox->addItems(scripts);
    update();
}

void MainWindow::on_removeButton_clicked()
{
    _ui->scriptComboBox->removeItem(_ui->scriptComboBox->currentIndex());
    update();
}

void MainWindow::infoText(QString str)
{
    _ui->infoLabel->setText(str);
    _ui->infoLabel->setStyleSheet("QLabel{color:black;}");
}

void MainWindow::on_scriptComboBox_currentIndexChanged(int)
{
    update();
}
