#include <limits>

#include <QCloseEvent>
#include <QIcon>
#include <QMessageBox>
#include <QRegularExpressionValidator>
#include <QStringListModel>

#include "CommonFunctions.hpp"
#include "CrossoverAlgoBase.hpp"
#include "FaultsManagerInterface.hpp"
#include "FunctionObserver.hpp"
#include "MainWindow.hpp"
#include "SelectionAlgoBase.hpp"
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
    ui->label_14->setVisible(false);

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

    setSelectAlgoNames(ui->selectAlgoNameComboBox, uiData);

    const auto selectAlgoPopulationPercent = CommonFunc::percentOf(uiData.selectAlgoPopulationQuantity,
                                                                   uiData.populationQuantity);
    ui->selectAlgoPopulationPercentLine->setText(QString::number(selectAlgoPopulationPercent));
    ui->selectAlgoPopulationPercentLine->setValidator(
        new QRegularExpressionValidator(QRegularExpression{decimalExpression}, this));

    setCrossoverAlgoNames(ui->crossoverAlgoComboBox, uiData);
    connect(ui->crossoverAlgoComboBox, &QComboBox::currentIndexChanged, this,
            &MainWindow::crossoverStrategyChangeValue);

    ui->crossingPropabilityLine->setText(QString::number(uiData.crossingPropablity));
    ui->crossingPropabilityLine->setValidator(
        new QRegularExpressionValidator(QRegularExpression{floatExpression}, this));
    crossoverStrategyChangeValue();

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

void MainWindow::setSelectAlgoNames(QComboBox *comboBox, const UiData &uiData)
{
    for(const auto selectAlgoId : uiData.selectAlgoNames)
    {
        switch (selectAlgoId)
        {
        case static_cast<decltype(selectAlgoId)>(SelectionAlgoId::BEST_SELECTION):
            comboBox->addItem("Best selection");
            break;
        case static_cast<decltype(selectAlgoId)>(SelectionAlgoId::ROULETTE_SELECTION):
            comboBox->addItem("Roulette selection");
            break;
        default:
            qDebug() << "Add select algorithm not found ID:" << selectAlgoId;
            break;
        }
    }
    comboBox->setCurrentIndex(uiData.selctAlgoIndex);
}

void MainWindow::setCrossoverAlgoNames(QComboBox *comboBox, const UiData &uiData)
{
    for(const auto crossoverAlgoId : uiData.crossoverAlgoNames)
    {
        switch (crossoverAlgoId)
        {
        case static_cast<decltype(crossoverAlgoId)>(CrossoverAlgoId::SINGLE_POINT):
            comboBox->addItem("Signle point");
            break;
        case static_cast<decltype(crossoverAlgoId)>(CrossoverAlgoId::TWO_POINT):
            comboBox->addItem("Two point");
            break;
        case static_cast<decltype(crossoverAlgoId)>(CrossoverAlgoId::UNIFORM):
            comboBox->addItem("Uniform");
            break;
        case static_cast<decltype(crossoverAlgoId)>(CrossoverAlgoId::DISCRETE):
            comboBox->addItem("Discrete");
            break;
        default:
            qDebug() << "Add crossover algorithm not found ID:" << crossoverAlgoId;
            break;
        }
    }
    comboBox->setCurrentIndex(uiData.crossoverAlgoIndex);
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
    verifyCrossoverAlgo(uiData);
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
        uiData.functionDimension = static_cast<decltype(uiData.functionDimension)>(parseNumber-1);
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
    if (parseStatus && (parseNumber > 1U))
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
        resetErrorLine(ui->generationsLine);
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
    if(!verifySelectAlgoBox(ui->selectAlgoNameComboBox, uiData))
    {
        qDebug() << "Chose not supported select algorithm";
        faultsManager.updateFault(Faults::INPUT_ALGORITHM_PARAMETRS_ERROR, true);
    }

    bool parseStatus = false;

    const QString percentStr = ui->selectAlgoPopulationPercentLine->text();
    qint128 percentNumber = validateTextNumber<quint8>(percentStr, parseStatus);
    if (parseStatus && (percentNumber > 0U) && (percentNumber <= 50U))
    {
        const auto selectPopulationQantity = CommonFunc::numberOfPercent<
            decltype(uiData.selectAlgoPopulationQuantity)>(uiData.populationQuantity,
                                                           percentNumber);
        qDebug() << "Calculate quantity of population for select algorithm:" << selectPopulationQantity;
        uiData.selectAlgoPopulationQuantity = selectPopulationQantity;
        resetErrorLine(ui->selectAlgoPopulationPercentLine);
    }
    else
    {
        qDebug() << "Parse error for input percent of population of select algorithm:" << percentStr;
        setErrorLine(ui->selectAlgoPopulationPercentLine);
        faultsManager.updateFault(Faults::INPUT_ALGORITHM_PARAMETRS_ERROR, true);
    }
}

bool MainWindow::verifySelectAlgoBox(QComboBox *comboBox, UiData &uiData)
{
    bool status = true;
    qDebug() << "Choose selection algorithm" << comboBox->currentText();
    switch (comboBox->currentIndex())
    {
    case static_cast<qint32>(SelectionAlgoId::BEST_SELECTION):
        uiData.selctAlgoIndex =
            static_cast<decltype(uiData.selctAlgoIndex)>(SelectionAlgoId::BEST_SELECTION);
        break;
    case static_cast<qint32>(SelectionAlgoId::ROULETTE_SELECTION):
        uiData.selctAlgoIndex =
            static_cast<decltype(uiData.selctAlgoIndex)>(SelectionAlgoId::ROULETTE_SELECTION);
        break;
    default:
        status = false;
        break;
    }
    return status;
}

void MainWindow::verifyCrossoverAlgo(UiData &uiData)
{
    if(!verifyCrossoverAlgoBox(ui->crossoverAlgoComboBox, uiData))
    {
        qDebug() << "Chose not supported crossover algorithm";
        faultsManager.updateFault(Faults::INPUT_ALGORITHM_PARAMETRS_ERROR, true);
    }

    if (uiData.crossoverAlgoIndex ==
        static_cast<decltype(uiData.crossoverAlgoIndex)>(CrossoverAlgoId::UNIFORM))
    {
        bool parseStatus = false;
        const QString probabilityStr = ui->crossingPropabilityLine->text();
        qreal probability = probabilityStr.toDouble(&parseStatus);

        if(parseStatus && CommonFunc::moreEqThan(probability, 0.0) &&
            CommonFunc::lessEqThan(probability, 1.0))
        {
            qDebug() << "Crossing probability:" << probability;
            uiData.crossingPropablity = probability;
            resetErrorLine(ui->crossingPropabilityLine);
        }
        else
        {
            qDebug() << "Parse error for input crossing probability:" << probabilityStr;
            setErrorLine(ui->crossingPropabilityLine);
            faultsManager.updateFault(Faults::INPUT_ALGORITHM_PARAMETRS_ERROR, true);
        }
    }
    else
    {
        qDebug() << "Not use probability field for crossing algorithm";
    }
}

bool MainWindow::verifyCrossoverAlgoBox(QComboBox *comboBox, UiData &uiData)
{
    bool result = true;
    qDebug() << "Choose crossover algorithm" << comboBox->currentText();
    switch (comboBox->currentIndex())
    {
    case static_cast<qint32>(CrossoverAlgoId::SINGLE_POINT):
        uiData.crossoverAlgoIndex =
            static_cast<decltype(uiData.crossoverAlgoIndex)>(CrossoverAlgoId::SINGLE_POINT);
        break;
    case static_cast<qint32>(CrossoverAlgoId::TWO_POINT):
        uiData.crossoverAlgoIndex =
            static_cast<decltype(uiData.crossoverAlgoIndex)>(CrossoverAlgoId::TWO_POINT);
        break;
    case static_cast<qint32>(CrossoverAlgoId::UNIFORM):
        uiData.crossoverAlgoIndex =
            static_cast<decltype(uiData.crossoverAlgoIndex)>(CrossoverAlgoId::UNIFORM);
        break;
    case static_cast<qint32>(CrossoverAlgoId::DISCRETE):
        uiData.crossoverAlgoIndex =
            static_cast<decltype(uiData.crossoverAlgoIndex)>(CrossoverAlgoId::DISCRETE);
        break;
    default:
        result = false;
        break;
    }
    return result;
}

void MainWindow::verifyEliteStrategy(UiData &uiData)
{
    if (ui->eliteStrategyCheckBox->isChecked())
    {
        uiData.eliteStrategyEnable = true;
        bool parseStatus = false;

        const QString percentStr = ui->elitePopulationLine->text();
        qint128 percentNumber =
            validateTextNumber<decltype(uiData.eliteStrategyQuantity)>(percentStr, parseStatus);
        if (parseStatus && (percentNumber > 0U))
        {
            const auto elitePopulationQantity = CommonFunc::numberOfPercent<
                decltype(uiData.eliteStrategyQuantity)>(uiData.populationQuantity,
                                                        percentNumber);
            if ((elitePopulationQantity <= uiData.selectAlgoPopulationQuantity) &&
                (elitePopulationQantity != 0U))
            {
                qDebug() << "Hold quantity of best population:" << elitePopulationQantity;
                uiData.eliteStrategyQuantity = elitePopulationQantity;
                resetErrorLine(ui->elitePopulationLine);
            }
            else
            {
                qDebug() << "Elite population quantity value error";
                setErrorLine(ui->elitePopulationLine);
                faultsManager.updateFault(Faults::INPUT_ALGORITHM_PARAMETRS_ERROR, true);
            }
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
        uiData.eliteStrategyEnable = false;
        qDebug("Ignore hold elite strategy");
    }
}

void MainWindow::eliteStrategyChangeValue()
{
    if (ui->eliteStrategyCheckBox->isChecked())
    {
        const UiData& uiData = uiDataHolder.getRef();
        const auto eliteStrategyPercent = CommonFunc::percentOf(uiData.eliteStrategyQuantity,
                                                                uiData.populationQuantity);
        unlockLineEdit(ui->elitePopulationLine, originalPalette, eliteStrategyPercent);
    }
    else
    {
        lockLineEdit(ui->elitePopulationLine);
    }
}

void MainWindow::crossoverStrategyChangeValue()
{
    if (ui->crossoverAlgoComboBox->currentIndex() ==
        static_cast<qint32>(CrossoverAlgoId::UNIFORM))
    {
        const UiData& uiData = uiDataHolder.getRef();
        unlockLineEdit(ui->crossingPropabilityLine,
                       originalPalette,
                       uiData.crossingPropablity);
    }
    else
    {
        lockLineEdit(ui->crossingPropabilityLine);
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
