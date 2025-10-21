#pragma once

#include "WindowInterface.hpp"

namespace Ui {
class MainWindow;
}
class QLineEdit;

class FaultsManagerInterface;
class UiDataHolderInterface;

class MainWindow : public WindowInterface
{
public:
    MainWindow(FaultsManagerInterface &faultsManager,
               UiDataHolderInterface &uiDataHolder,
               QWidget *parent = nullptr);

    ~MainWindow();

    void load() override;

    void closeEvent(QCloseEvent *event) override;

    void showCriticalMessage(const char* message) override;

    void showWarningMessage(const char* message) override;

private:
    FaultsManagerInterface &faultsManager;
    UiDataHolderInterface &uiDataHolder;
    Ui::MainWindow *ui;

    static void setErrorLine(QLineEdit *lineEdit);
    static void resetErrorLine(QLineEdit *lineEdit);

    void verifyRandomSeed();
    void verifySearchRange();
    void verifyPrecissionRange();
    void verifyPopulation();
    void verifyGenerations();

    static constexpr QLatin1StringView decimalExpression{"^[0-9+\\-]+$"};
    static constexpr QLatin1StringView floatExpression{"^[0-9+\\-.]+$"};

private slots:
    void onStartCalcButton();
};
