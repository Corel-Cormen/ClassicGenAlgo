#pragma once

#include "WindowInterface.hpp"

namespace Ui {
class MainWindow;
}
class QLineEdit;

class FaultsManagerInterface;
class UiDataHolderInterface;
class FunctionObserver;
struct UiData;

class MainWindow : public WindowInterface
{
public:
    MainWindow(FaultsManagerInterface &faultsManager,
               UiDataHolderInterface &uiDataHolder,
               FunctionObserver &functionObserver,
               QWidget *parent = nullptr);

    ~MainWindow();

    void load() override;

    void closeEvent(QCloseEvent *event) override;

    void showCriticalMessage(const char* message) override;

    void showWarningMessage(const char* message) override;

private:
    FaultsManagerInterface &faultsManager;
    UiDataHolderInterface &uiDataHolder;
    FunctionObserver &functionObserver;
    Ui::MainWindow *ui;

    static void setErrorLine(QLineEdit *lineEdit);
    static void resetErrorLine(QLineEdit *lineEdit);

    void verifyRandomSeed(UiData &uiData);
    void verifySelectFunction(UiData &uiData);
    void verifyFunctionDimension(UiData &uiData);
    void verifySearchRange(UiData &uiData);
    void verifyPrecissionRange(UiData &uiData);
    void verifyPopulation(UiData &uiData);
    void verifyGenerations(UiData &uiData);
    void verifySelectAlgo(UiData &uiData);
    void verifySelectAlgoBestPercentPopulation(UiData &uiData);

    static constexpr QLatin1StringView decimalExpression{"^[0-9+\\-]+$"};
    static constexpr QLatin1StringView floatExpression{"^[0-9+\\-.]+$"};

private slots:
    void onStartCalcButton();
};
