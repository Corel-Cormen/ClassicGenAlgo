#pragma once

#include <QMainWindow>

class WindowInterface : public QMainWindow
{
    Q_OBJECT

public:
    WindowInterface(QWidget *parent = nullptr)
        : QMainWindow{parent}
    {}

    virtual ~WindowInterface() = default;

    virtual void load() = 0;

    virtual void showCriticalMessage(const char* message) = 0;

    virtual void showWarningMessage(const char* message) = 0;

signals:
    void windowClosed();
    void triggerCalculate();
};
