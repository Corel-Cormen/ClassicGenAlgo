#include <limits>

#include <QCloseEvent>
#include <QIcon>
#include <QMessageBox>
#include <QRegularExpressionValidator>
#include <QStringListModel>

#include "CommonFunctions.hpp"
#include "FaultsManagerInterface.hpp"
#include "FunctionObserver.hpp"
#include "MainWindow.hpp"
#include "UiDataHolderInterface.hpp"

#include "./ui_mainwindow.h"

namespace {

template<typename T>
qint128 validateTextNumber(const QString& text, bool &parseResult)
{
    qint128 result = 0;

    if constexpr (std::is_same_v<T, quint8>)
    {
        result = static_cast<qint128>(text.toUInt(&parseResult));
        if (!parseResult || (result > static_cast<qint128>(std::numeric_limits<quint8>::max())))
        {
            parseResult = false;
        }
    }
    else if constexpr (std::is_same_v<T, quint16>)
    {
        result = static_cast<qint128>(text.toUShort(&parseResult));
    }
    else if constexpr (std::is_same_v<T, qint32>)
    {
        result = static_cast<qint128>(text.toInt(&parseResult));
    }
    else if constexpr (std::is_same_v<T, quint32>)
    {
        result = static_cast<qint128>(text.toUInt(&parseResult));
    }
    else if constexpr (std::is_same_v<T, qint64>)
    {
        result = static_cast<qint128>(text.toLongLong(&parseResult));
    }
    else if constexpr (std::is_same_v<T, quint64>)
    {
        result = static_cast<qint128>(text.toULongLong(&parseResult));
    }
    else
    {
        qDebug() << "Unsupported type for string to number";
    }

    return result;
}

} // end namespace

MainWindow::MainWindow(FaultsManagerInterface &faultsManager,
                       UiDataHolderInterface &uiDataHolder,
                       FunctionObserver &functionObserver,
                       QWidget *parent) :
    WindowInterface(parent),
    faultsManager{faultsManager},
    uiDataHolder{uiDataHolder},
    functionObserver{functionObserver},
    ui(new Ui::MainWindow)
{}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::load()
{
    Q_INIT_RESOURCE(AssetsWindowDashboard);

    ui->setupUi(this);
    setWindowTitle("ClassicGenAlgo");
    setWindowIcon(QIcon(":/icons/ClassicGenAlgoLogo.ico"));

    const auto& uiData = uiDataHolder.getRef();

    QStringList items;
    for (const auto& funcName : functionObserver.getNames())
    {
        items << funcName;
    }

    QStringListModel *model = new QStringListModel(this);
    model->setStringList(items);
    ui->functionsListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->functionsListView->setModel(model);
    ui->functionsListView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->functionsListView->setCurrentIndex(model->index(uiData.selectFunctionId, 0));

    ui->randomSeedLine->setText(QString::number(uiData.randomSeed));
    ui->randomSeedLine->setValidator(
        new QRegularExpressionValidator(QRegularExpression{decimalExpression}, this));

    ui->functionDimensionLine->setText(QString::number(uiData.functionDimension));
    ui->functionDimensionLine->setValidator(
        new QRegularExpressionValidator(QRegularExpression{decimalExpression}, this));

    ui->minSearchScopeLine->setText(QString::number(uiData.minSearchRange));
    ui->minSearchScopeLine->setValidator(
        new QRegularExpressionValidator(QRegularExpression{floatExpression}, this));

    ui->maxSearchScopeLine->setText(QString::number(uiData.maxSearchRange));
    ui->maxSearchScopeLine->setValidator(
        new QRegularExpressionValidator(QRegularExpression{floatExpression}, this));

    ui->precissionLine->setText(QString::number(uiData.precission));
    ui->precissionLine->setValidator(
        new QRegularExpressionValidator(QRegularExpression{decimalExpression}, this));

    ui->populationLine->setText(QString::number(uiData.populationQuantity));
    ui->populationLine->setValidator(
        new QRegularExpressionValidator(QRegularExpression{decimalExpression}, this));

    ui->generationsLine->setText(QString::number(uiData.generations));
    ui->generationsLine->setValidator(
        new QRegularExpressionValidator(QRegularExpression{decimalExpression}, this));

    for(const auto& selectAlgoName : uiData.selectAlgoNames)
    {
        ui->selectAlgoNameComboBox->addItem(selectAlgoName.toString());
    }
    ui->selectAlgoNameComboBox->setCurrentIndex(uiData.selctAlgoIndex);

    ui->selectAlgoPopulationPercentLine->setText(QString::number(uiData.selectAlgoPopulationPercent));
    ui->selectAlgoPopulationPercentLine->setValidator(
        new QRegularExpressionValidator(QRegularExpression{decimalExpression}, this));

    ui->eliteStrategyCheckBox->setChecked(uiData.eliteStrategyEnable);
    originalPalette = ui->elitePopulationLine->palette();
    ui->elitePopulationLine->setValidator(
        new QRegularExpressionValidator(QRegularExpression{decimalExpression}, this));
    eliteStrategyChangeValue();

    connect(ui->eliteStrategyCheckBox, &QCheckBox::checkStateChanged, this,
            &MainWindow::eliteStrategyChangeValue);

    connect(ui->startCalcButton, &QPushButton::clicked, this,
            &MainWindow::onStartCalcButton);

    show();
    if (!isVisible())
    {
        qDebug() << "Load window error";
        faultsManager.updateFault(Faults::LOAD_WINDOW_ERROR, true);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (faultsManager.isNonRecoverable())
    {
        qDebug() << "Critical fault detected - close application";
        emit windowClosed();
    }
    else
    {
        auto reply = QMessageBox::question(
            this,
            tr("Confirm"),
            tr("Are you sure you want to close the application?"),
            QMessageBox::Yes | QMessageBox::No
        );

        if (reply == QMessageBox::Yes)
        {
            qDebug() << "Window will be closed";
            emit windowClosed();
            event->accept();
            QMainWindow::closeEvent(event);
        }
        else
        {
            qDebug() << "Close window canceled";
            event->ignore();
        }
    }
}

void MainWindow::showCriticalMessage(const char* message)
{
    QString text = (message != nullptr) ?
        QString::fromUtf8(message) : "Unknow error";

    (void) QMessageBox::critical(
        this,
        tr("Critical Error"),
        text
    );
}

void MainWindow::showWarningMessage(const char* message)
{
    QString text = (message != nullptr) ?
        QString::fromUtf8(message) : "Unknow warning";

    (void) QMessageBox::warning(
        this,
        tr("Warning"),
        text
    );
}

void MainWindow::setErrorLine(QLineEdit *lineEdit)
{
    lineEdit->setStyleSheet("QLineEdit { border: 2px solid red; }");
}

void MainWindow::resetErrorLine(QLineEdit *lineEdit)
{
    lineEdit->setStyleSheet("");
}

void MainWindow::onStartCalcButton()
{
    auto& uiData = uiDataHolder.getRef();

    verifyRandomSeed(uiData);
    verifySelectFunction(uiData);
    verifyFunctionDimension(uiData);
    verifySearchRange(uiData);
    verifyPrecissionRange(uiData);
    verifyPopulation(uiData);
    verifyGenerations(uiData);
    verifySelectAlgo(uiData);
    verifyEliteStrategy(uiData);

    emit triggerCalculate();
}

void MainWindow::verifyRandomSeed(UiData &uiData)
{
    bool parseStatus = false;

    const QString seedStr = ui->randomSeedLine->text();
    qint128 parseNumber = validateTextNumber<decltype(uiData.randomSeed)>(seedStr, parseStatus);
    if (parseStatus)
    {
        qDebug() << "Input random seed:" << parseNumber;
        uiData.randomSeed = static_cast<decltype(uiData.randomSeed)>(parseNumber);
        resetErrorLine(ui->randomSeedLine);
    }
    else
    {
        qDebug() << "Maximum number reached for random seed line:" << seedStr;
        setErrorLine(ui->randomSeedLine);
        faultsManager.updateFault(Faults::INPUT_RANDOM_SEED_ERROR, true);
    }
}

void MainWindow::verifySelectFunction(UiData &uiData)
{
    QModelIndex index = ui->functionsListView->currentIndex();
    uiData.selectFunctionId = index.row();
    qDebug() << "Select function:" << ui->functionsListView->currentIndex().data(Qt::DisplayRole).toString();
}

void MainWindow::verifyFunctionDimension(UiData &uiData)
{
    bool parseStatus = false;

    const QString funDimStr = ui->functionDimensionLine->text();
    qint128 parseNumber = validateTextNumber<decltype(uiData.functionDimension)>(funDimStr, parseStatus);
    if (parseStatus && (parseNumber > 1U))
    {
        qDebug() << "Input function dimension:" << parseNumber;
        uiData.functionDimension = static_cast<decltype(uiData.functionDimension)>(parseNumber);
        resetErrorLine(ui->functionDimensionLine);
    }
    else
    {
        qDebug() << "Maximum number reached for function dimension line:" << funDimStr;
        setErrorLine(ui->functionDimensionLine);
        faultsManager.updateFault(Faults::INPUT_FUNCTION_PARAMETERS_ERROR, true);
    }
}

void MainWindow::verifySearchRange(UiData &uiData)
{
    bool parseMinStatus = false;
    bool parseMaxStatus = false;

    const QString minStr = ui->minSearchScopeLine->text();
    const QString maxStr = ui->maxSearchScopeLine->text();
    qreal min = minStr.toDouble(&parseMinStatus);
    qreal max = maxStr.toDouble(&parseMaxStatus);

    if(parseMinStatus && parseMaxStatus &&
        CommonFunc::lessThan(min, max) && !CommonFunc::equal(min, max))
    {
        qDebug() << "Search range: [" << min << "," << max << "]";
        uiData.minSearchRange = min;
        uiData.maxSearchRange = max;
        resetErrorLine(ui->minSearchScopeLine);
        resetErrorLine(ui->maxSearchScopeLine);
    }
    else
    {
        qDebug() << "Parse or verify search scope error [" << minStr << "," << maxStr << "]";
        setErrorLine(ui->minSearchScopeLine);
        setErrorLine(ui->maxSearchScopeLine);
        faultsManager.updateFault(Faults::INPUT_FUNCTION_PARAMETERS_ERROR, true);
    }
}

void MainWindow::verifyPrecissionRange(UiData &uiData)
{
    bool parseStatus = false;

    const QString precissionStr = ui->precissionLine->text();
    qint128 parseNumber = validateTextNumber<decltype(uiData.precission)>(precissionStr, parseStatus);
    if (parseStatus)
    {
        qDebug() << "Precission:" << parseNumber;
        uiData.precission = static_cast<decltype(uiData.precission)>(parseNumber);
        resetErrorLine(ui->precissionLine);
    }
    else
    {
        qDebug() << "Maximum number reached for precission line:" << precissionStr;
        setErrorLine(ui->precissionLine);
        faultsManager.updateFault(Faults::INPUT_FUNCTION_PARAMETERS_ERROR, true);
    }
}

void MainWindow::verifyPopulation(UiData &uiData)
{
    bool parseStatus = false;

    const QString populationStr = ui->populationLine->text();
    qint128 parseNumber = validateTextNumber<decltype(uiData.populationQuantity)>(populationStr, parseStatus);
    if (parseStatus)
    {
        qDebug() << "Population:" << parseNumber;
        uiData.populationQuantity = static_cast<decltype(uiData.populationQuantity)>(parseNumber);
        resetErrorLine(ui->populationLine);
    }
    else
    {
        qDebug() << "Maximum number reached for population line:" << populationStr;
        setErrorLine(ui->populationLine);
        faultsManager.updateFault(Faults::INPUT_ALGORITHM_PARAMETRS_ERROR, true);
    }
}

void MainWindow::verifyGenerations(UiData &uiData)
{
    bool parseStatus = false;

    const QString generationsStr = ui->generationsLine->text();
    qint128 parseNumber = validateTextNumber<decltype(uiData.generations)>(generationsStr, parseStatus);
    if (parseStatus)
    {
        qDebug() << "Generations:" << parseNumber;
        uiData.generations = static_cast<decltype(uiData.generations)>(parseNumber);
        resetErrorLine(ui->populationLine);
    }
    else
    {
        qDebug() << "Maximum number reached for generations line:" << generationsStr;
        setErrorLine(ui->generationsLine);
        faultsManager.updateFault(Faults::INPUT_ALGORITHM_PARAMETRS_ERROR, true);
    }
}

void MainWindow::verifySelectAlgo(UiData &uiData)
{
    uiData.selctAlgoIndex = ui->selectAlgoNameComboBox->currentIndex();
    qDebug() << "Choose selection algorithm" << ui->selectAlgoNameComboBox->currentText();

    bool parseStatus = false;

    const QString percentStr = ui->selectAlgoPopulationPercentLine->text();
    qint128 percentNumber =
        validateTextNumber<decltype(uiData.selectAlgoPopulationPercent)>(percentStr, parseStatus);
    if (parseStatus && (percentNumber > 0U) && (percentNumber <= 50U))
    {
        qDebug() << "Select percent of population for select algorithm:" << percentNumber;
        uiData.selectAlgoPopulationPercent =
            static_cast<decltype(uiData.selectAlgoPopulationPercent)>(percentNumber);
        resetErrorLine(ui->selectAlgoPopulationPercentLine);
    }
    else
    {
        qDebug() << "Parse error for input percent of population of select algorithm:" << percentStr;
        setErrorLine(ui->selectAlgoPopulationPercentLine);
        faultsManager.updateFault(Faults::INPUT_ALGORITHM_PARAMETRS_ERROR, true);
    }
}

void MainWindow::verifyEliteStrategy(UiData &uiData)
{
    if (ui->eliteStrategyCheckBox->isChecked())
    {
        bool parseStatus = false;

        const QString percentStr = ui->elitePopulationLine->text();
        qint128 percentNumber =
            validateTextNumber<decltype(uiData.eliteStrategyPercent)>(percentStr, parseStatus);
        if (parseStatus && (percentNumber > 0U) &&
            (percentNumber < uiData.selectAlgoPopulationPercent))
        {
            qDebug() << "Hold % of best population:" << percentNumber;
            uiData.eliteStrategyPercent = static_cast<decltype(uiData.eliteStrategyPercent)>(percentNumber);
            resetErrorLine(ui->elitePopulationLine);
        }
        else
        {
            qDebug() << "Parse error for input percent of elite population strategy:" << percentStr;
            setErrorLine(ui->elitePopulationLine);
            faultsManager.updateFault(Faults::INPUT_ALGORITHM_PARAMETRS_ERROR, true);
        }
    }
    else
    {
        qDebug("Ignore hold elite strategy");
    }
}

void MainWindow::eliteStrategyChangeValue()
{
    const UiData& uiData = uiDataHolder.getRef();
    if(ui->eliteStrategyCheckBox->isChecked())
    {
        unlockLineEdit(ui->elitePopulationLine, originalPalette, uiData.eliteStrategyPercent);
    }
    else
    {
        lockLineEdit(ui->elitePopulationLine);
    }
}

template<typename T>
void MainWindow::unlockLineEdit(QLineEdit *lineEdit, const QPalette &originalPalette, const T value)
{
    QPalette p = lineEdit->palette();
    p.setColor(QPalette::Base, QColor(originalPalette.color(QPalette::Base)));
    p.setColor(QPalette::Text, QColor(originalPalette.color(QPalette::Text)));
    lineEdit->setPalette(p);
    lineEdit->setText(QString::number(value));
    lineEdit->setReadOnly(false);
}

void MainWindow::lockLineEdit(QLineEdit *lineEdit)
{
    QPalette p = lineEdit->palette();
    p.setColor(QPalette::Base, QColor(0, 0, 0));
    p.setColor(QPalette::Text, QColor(255, 255, 255));
    lineEdit->setPalette(p);
    lineEdit->setText("inactive");
    lineEdit->setReadOnly(true);
}
