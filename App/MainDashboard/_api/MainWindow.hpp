#pragma once

#include "WindowInterface.hpp"

namespace Ui {
class MainWindow;
}
class QLineEdit;
class QComboBox;

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

    QPalette originalPalette{};

    static void setSelectAlgoNames(QComboBox *comboBox, const UiData &uiData);
    static void setCrossoverAlgoNames(QComboBox *comboBox, const UiData &uiData);
    static void setMutationAlgoNames(QComboBox *comboBox, const UiData &uiData);

    static void setErrorLine(QLineEdit *lineEdit);
    static void resetErrorLine(QLineEdit *lineEdit);
    template<typename T>
    static void unlockLineEdit(QLineEdit *lineEdit, const QPalette &originalPalette, const T value);
    static void lockLineEdit(QLineEdit *lineEdit);

    void verifyRandomSeed(UiData &uiData);
    void verifySelectFunction(UiData &uiData);
    void verifyFunctionDimension(UiData &uiData);
    void verifySearchRange(UiData &uiData);
    void verifyPrecissionRange(UiData &uiData);
    void verifyPopulation(UiData &uiData);
    void verifyGenerations(UiData &uiData);
    void verifySelectAlgo(UiData &uiData);
    static bool verifySelectAlgoBox(QComboBox *comboBox, UiData &uiData);
    void verifyTournamentSize(UiData &uiData);
    void verifyCrossoverAlgo(UiData &uiData);
    static bool verifyCrossoverAlgoBox(QComboBox *comboBox, UiData &uiData);
    void verifyMutationAlgo(UiData &uiData);
    static bool verifyMutationAlgoBox(QComboBox *comboBox, UiData &uiData);
    void verifyEliteStrategy(UiData &uiData);

    static constexpr QLatin1StringView decimalExpression{"^[0-9+\\-]+$"};
    static constexpr QLatin1StringView floatExpression{"^[0-9+\\-.]+$"};

private slots:
    void onStartCalcButton();
    void selectStrategyChangeValue();
    void crossoverStrategyChangeValue();
    void eliteStrategyChangeValue();
};
