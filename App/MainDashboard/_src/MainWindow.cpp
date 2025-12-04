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
#include "MutationAlgoBase.hpp"
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

    ui->showChartsCheckBox->setChecked(uiData.showCharts);

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

    setSelectAlgorithmType(uiData);

    setSelectAlgoNames(ui->selectAlgoNameComboBox, uiData);

    const auto selectAlgoPopulationPercent = CommonFunc::percentOf(uiData.selectAlgoPopulationQuantity,
                                                                   uiData.populationQuantity);
    ui->selectAlgoPopulationPercentLine->setText(QString::number(selectAlgoPopulationPercent));
    ui->selectAlgoPopulationPercentLine->setValidator(
        new QRegularExpressionValidator(QRegularExpression{decimalExpression}, this));
    connect(ui->selectAlgoNameComboBox, &QComboBox::currentIndexChanged, this,
            &MainWindow::selectStrategyChangeValue);

    ui->tournamentSizeLine->setText(QString::number(uiData.tournamentPopulationQuantity));
    ui->tournamentSizeLine->setValidator(
        new QRegularExpressionValidator(QRegularExpression{decimalExpression}, this));
    selectStrategyChangeValue();

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

void MainWindow::setSelectAlgorithmType(const UiData &uiData)
{
    if(uiData.algorithmType == AlgorithmType::BINARY_ALGO_TYPE)
    {
        ui->binaryNumberRepresentationCheckBox->setCheckState(Qt::CheckState::Checked);
        onBinaryAlgoSelect();
    }
    else
    {
        ui->realNumberRepresentationCheckBox->setCheckState(Qt::CheckState::Checked);
        onRealAlgoSelect();
    }
    connect(ui->binaryNumberRepresentationCheckBox, &QCheckBox::checkStateChanged, this,
            &MainWindow::onBinaryAlgoSelect);
    connect(ui->realNumberRepresentationCheckBox, &QCheckBox::checkStateChanged, this,
            &MainWindow::onRealAlgoSelect);
}

void MainWindow::setSelectAlgoNames(QComboBox *comboBox, const UiData &uiData)
{
    comboBox->clear();
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
        case static_cast<decltype(selectAlgoId)>(SelectionAlgoId::TOURNAMENT_SELECTION):
            comboBox->addItem("Tournament selection");
            break;
        default:
            qDebug() << "Add select algorithm not found ID:" << selectAlgoId;
            break;
        }
    }
    comboBox->setCurrentIndex(uiData.selctAlgoIndex);
}

void MainWindow::setBinaryCrossoverAlgoNames(QComboBox *comboBox, const UiData &uiData)
{
    comboBox->clear();
    for (size_t idx = 0U; idx < uiData.crossoverBinaryAlgoQuantity; ++idx)
    {
        switch (uiData.crossoverAlgoNames[idx])
        {
        case static_cast<decltype(uiData.crossoverAlgoIndex)>(CrossoverAlgoId::SINGLE_POINT):
            comboBox->addItem("Signle point");
            break;
        case static_cast<decltype(uiData.crossoverAlgoIndex)>(CrossoverAlgoId::TWO_POINT):
            comboBox->addItem("Two point");
            break;
        case static_cast<decltype(uiData.crossoverAlgoIndex)>(CrossoverAlgoId::UNIFORM):
            comboBox->addItem("Uniform");
            break;
        case static_cast<decltype(uiData.crossoverAlgoIndex)>(CrossoverAlgoId::DISCRETE):
            comboBox->addItem("Discrete");
            break;
        default:
            qDebug() << "Add crossover algorithm not found ID:" << uiData.crossoverAlgoNames[idx];
            break;
        }
    }
    comboBox->setCurrentIndex(0U);
}

void MainWindow::setBinaryMutationAlgoNames(QComboBox *comboBox, const UiData &uiData)
{
    comboBox->clear();
    for (size_t idx = 0U; idx < uiData.mutationBinaryAlgoQuantity; ++idx)
    {
        switch (uiData.mutationAlgoNames[idx])
        {
        case static_cast<decltype(uiData.mutationAlgoIndex)>(MutationAlgoId::EDGE_MUTATION):
            comboBox->addItem("Edge mutation");
            break;
        case static_cast<decltype(uiData.mutationAlgoIndex)>(MutationAlgoId::ONE_POINT_MUTATION):
            comboBox->addItem("One point mutation");
            break;
        case static_cast<decltype(uiData.mutationAlgoIndex)>(MutationAlgoId::TWO_POINT_MUTATION):
            comboBox->addItem("Two points mutation");
            break;
        case static_cast<decltype(uiData.mutationAlgoIndex)>(MutationAlgoId::INWERSE_MUTATION):
            comboBox->addItem("Inverse mutation");
            break;
        default:
            qDebug() << "Add mutation algorithm not found ID:" << uiData.mutationAlgoNames[idx];
            break;
        }
    }
    comboBox->setCurrentIndex(0);
}

void MainWindow::setRealCrossoverAlgoNames(QComboBox *comboBox, const UiData &uiData)
{
    comboBox->clear();
    for (size_t idx = uiData.crossoverBinaryAlgoQuantity; idx < uiData.crossoverAlgoNames.size(); ++idx)
    {
        switch (uiData.crossoverAlgoNames[idx])
        {
        case static_cast<decltype(uiData.crossoverAlgoIndex)>(CrossoverAlgoId::ARITMETIC):
            comboBox->addItem("Arythmetic");
            break;
        case static_cast<decltype(uiData.crossoverAlgoIndex)>(CrossoverAlgoId::LINEAR):
            comboBox->addItem("Linear");
            break;
        case static_cast<decltype(uiData.crossoverAlgoIndex)>(CrossoverAlgoId::BLEND_ALPHA):
            comboBox->addItem("Blend Alpha");
            break;
        case static_cast<decltype(uiData.crossoverAlgoIndex)>(CrossoverAlgoId::BLEND_ALPHA_BETA):
            comboBox->addItem("Blend Alpha-Beta");
            break;
        case static_cast<decltype(uiData.crossoverAlgoIndex)>(CrossoverAlgoId::AVERAGING):
            comboBox->addItem("Averaging");
            break;
        default:
            qDebug() << "Add crossover algorithm not found ID:" << uiData.crossoverAlgoNames[idx];
            break;
        }
    }
    comboBox->setCurrentIndex(0);
}

void MainWindow::setRealMutationAlgoNames(QComboBox *comboBox, const UiData &uiData)
{
    comboBox->clear();
    for (size_t idx = uiData.mutationBinaryAlgoQuantity; idx < uiData.mutationAlgoNames.size(); ++idx)
    {
        switch (uiData.mutationAlgoNames[idx])
        {
        case static_cast<decltype(uiData.mutationAlgoIndex)>(MutationAlgoId::UNIFORM_MUTATION):
            comboBox->addItem("Uniform mutation");
            break;
        case static_cast<decltype(uiData.mutationAlgoIndex)>(MutationAlgoId::GAUSS_MUTATION):
            comboBox->addItem("Gauss mutation");
            break;
        default:
            qDebug() << "Add mutation algorithm not found ID:" << uiData.mutationAlgoNames[idx];
            break;
        }
    }
    comboBox->setCurrentIndex(0);
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
    verifyAlgorithmType(uiData);
    verifySelectAlgo(uiData);
    verifyTournamentSize(uiData);
    verifyCrossoverAlgo(uiData);
    verifyMutationAlgo(uiData);
    verifyEliteStrategy(uiData);
    verifyShowChars(uiData);

    emit triggerCalculate();
}

void MainWindow::onBinaryAlgoSelect()
{
    if(ui->binaryNumberRepresentationCheckBox->isChecked())
    {
        ui->realNumberRepresentationCheckBox->setCheckState(Qt::CheckState::Unchecked);

        auto& uiData = uiDataHolder.getRef();

        setBinaryCrossoverAlgoNames(ui->crossoverAlgoComboBox, uiData);
        connect(ui->crossoverAlgoComboBox, &QComboBox::currentIndexChanged, this,
                &MainWindow::crossoverStrategyChangeValue);

        ui->crossingPropabilityLine->setText(QString::number(uiData.crossingPropablity));
        ui->crossingPropabilityLine->setValidator(
            new QRegularExpressionValidator(QRegularExpression{floatExpression}, this));
        crossoverStrategyChangeValue();

        setBinaryMutationAlgoNames(ui->mutationAlgoComboBox, uiData);
        ui->mutationProbabilityLine->setText(QString::number(uiData.mutationPropablity));
        ui->mutationProbabilityLine->setValidator(
            new QRegularExpressionValidator(QRegularExpression{floatExpression}, this));
    }
    else
    {
        ui->realNumberRepresentationCheckBox->setCheckState(Qt::CheckState::Checked);
        disconnect(ui->crossoverAlgoComboBox, &QComboBox::currentIndexChanged, this,
                   &MainWindow::crossoverStrategyChangeValue);
    }
}

void MainWindow::onRealAlgoSelect()
{
    if(ui->realNumberRepresentationCheckBox->isChecked())
    {
        ui->binaryNumberRepresentationCheckBox->setCheckState(Qt::CheckState::Unchecked);

        auto& uiData = uiDataHolder.getRef();

        setRealCrossoverAlgoNames(ui->crossoverAlgoComboBox, uiData);
        setRealMutationAlgoNames(ui->mutationAlgoComboBox, uiData);
    }
    else
    {
        ui->binaryNumberRepresentationCheckBox->setCheckState(Qt::CheckState::Checked);
    }
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
    functionObserver.choseFunctionId(uiData.selectFunctionId);
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
        const auto functionType = functionObserver.getSelectType();
        if (functionType.has_value() && (functionType.value() == FunctionType::BENCHMARK_FUNC))
        {
            parseNumber -= 1;
        }
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

void MainWindow::verifyAlgorithmType(UiData &uiData)
{
    if (ui->binaryNumberRepresentationCheckBox->isChecked())
    {
        uiData.algorithmType = AlgorithmType::BINARY_ALGO_TYPE;
    }
    else if (ui->realNumberRepresentationCheckBox->isChecked())
    {
        uiData.algorithmType = AlgorithmType::REAL_ALGO_TYPE;
    }
    else
    {
        qDebug() << "No choose algorithm type";
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
    case static_cast<qint32>(SelectionAlgoId::TOURNAMENT_SELECTION):
        uiData.selctAlgoIndex =
            static_cast<decltype(uiData.selctAlgoIndex)>(SelectionAlgoId::TOURNAMENT_SELECTION);
        break;
    default:
        status = false;
        break;
    }
    return status;
}

void MainWindow::verifyTournamentSize(UiData &uiData)
{
    if (uiData.selctAlgoIndex ==
        static_cast<decltype(uiData.selctAlgoIndex)>(SelectionAlgoId::TOURNAMENT_SELECTION))
    {
        bool parseStatus = false;
        const QString sizeStr = ui->tournamentSizeLine->text();
        qint128 sizeNumber = validateTextNumber<quint8>(sizeStr, parseStatus);
        if (parseStatus && (sizeNumber > 1U) && (sizeNumber <= uiData.populationQuantity))
        {
            qDebug() << "Select tournament population size:" << sizeNumber;
            uiData.tournamentPopulationQuantity = sizeNumber;
            resetErrorLine(ui->tournamentSizeLine);
        }
        else
        {
            qDebug() << "Parse error for input tournament population size:" << sizeStr;
            setErrorLine(ui->tournamentSizeLine);
            faultsManager.updateFault(Faults::INPUT_ALGORITHM_PARAMETRS_ERROR, true);
        }
    }
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
    if (uiData.algorithmType == AlgorithmType::BINARY_ALGO_TYPE)
    {
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
    }
    else
    {
        if (comboBox->currentIndex() == (static_cast<qint32>(CrossoverAlgoId::ARITMETIC) -
                                         static_cast<qint32>(uiData.crossoverBinaryAlgoQuantity)))
        {
            uiData.crossoverAlgoIndex =
                static_cast<decltype(uiData.crossoverAlgoIndex)>(CrossoverAlgoId::ARITMETIC);
        }
        else if (comboBox->currentIndex() == (static_cast<qint32>(CrossoverAlgoId::LINEAR) -
                                              static_cast<qint32>(uiData.crossoverBinaryAlgoQuantity)))
        {
            uiData.crossoverAlgoIndex =
                static_cast<decltype(uiData.crossoverAlgoIndex)>(CrossoverAlgoId::LINEAR);
        }
        else if (comboBox->currentIndex() == (static_cast<qint32>(CrossoverAlgoId::BLEND_ALPHA) -
                                              static_cast<qint32>(uiData.crossoverBinaryAlgoQuantity)))
        {
            uiData.crossoverAlgoIndex =
                static_cast<decltype(uiData.crossoverAlgoIndex)>(CrossoverAlgoId::BLEND_ALPHA);
        }
        else if (comboBox->currentIndex() == (static_cast<qint32>(CrossoverAlgoId::BLEND_ALPHA_BETA) -
                                              static_cast<qint32>(uiData.crossoverBinaryAlgoQuantity)))
        {
            uiData.crossoverAlgoIndex =
                static_cast<decltype(uiData.crossoverAlgoIndex)>(CrossoverAlgoId::BLEND_ALPHA_BETA);
        }
        else if (comboBox->currentIndex() == (static_cast<qint32>(CrossoverAlgoId::AVERAGING) -
                                              static_cast<qint32>(uiData.crossoverBinaryAlgoQuantity)))
        {
            uiData.crossoverAlgoIndex =
                static_cast<decltype(uiData.crossoverAlgoIndex)>(CrossoverAlgoId::AVERAGING);
        }
        else
        {
            result = false;
        }
    }
    return result;
}

void MainWindow::verifyMutationAlgo(UiData &uiData)
{
    if(!verifyMutationAlgoBox(ui->mutationAlgoComboBox, uiData))
    {
        qDebug() << "Chose not supported mutation algorithm";
        faultsManager.updateFault(Faults::INPUT_ALGORITHM_PARAMETRS_ERROR, true);
    }

    bool parseStatus = false;
    const QString probabilityStr = ui->mutationProbabilityLine->text();
    qreal probability = probabilityStr.toDouble(&parseStatus);

    if(parseStatus && CommonFunc::moreEqThan(probability, 0.0) &&
        CommonFunc::lessEqThan(probability, 1.0))
    {
        qDebug() << "Mutation probability:" << probability;
        uiData.mutationPropablity = probability;
        resetErrorLine(ui->mutationProbabilityLine);
    }
    else
    {
        qDebug() << "Parse error for input mutation probability:" << probabilityStr;
        setErrorLine(ui->mutationProbabilityLine);
        faultsManager.updateFault(Faults::INPUT_ALGORITHM_PARAMETRS_ERROR, true);
    }
}

bool MainWindow::verifyMutationAlgoBox(QComboBox *comboBox, UiData &uiData)
{
    bool result = true;
    qDebug() << "Choose mutation algorithm" << comboBox->currentText();
    if (uiData.algorithmType == AlgorithmType::BINARY_ALGO_TYPE)
    {
        switch (comboBox->currentIndex())
        {
        case static_cast<qint32>(MutationAlgoId::EDGE_MUTATION):
            uiData.mutationAlgoIndex =
                static_cast<decltype(uiData.mutationAlgoIndex)>(MutationAlgoId::EDGE_MUTATION);
            break;
        case static_cast<qint32>(MutationAlgoId::ONE_POINT_MUTATION):
            uiData.mutationAlgoIndex =
                static_cast<decltype(uiData.mutationAlgoIndex)>(MutationAlgoId::ONE_POINT_MUTATION);
            break;
        case static_cast<qint32>(MutationAlgoId::TWO_POINT_MUTATION):
            uiData.mutationAlgoIndex =
                static_cast<decltype(uiData.mutationAlgoIndex)>(MutationAlgoId::TWO_POINT_MUTATION);
            break;
        case static_cast<qint32>(MutationAlgoId::INWERSE_MUTATION):
            uiData.mutationAlgoIndex =
                static_cast<decltype(uiData.mutationAlgoIndex)>(MutationAlgoId::INWERSE_MUTATION);
            break;
        default:
            result = false;
            break;
        }
    }
    else
    {
        if (comboBox->currentIndex() == (static_cast<qint32>(MutationAlgoId::UNIFORM_MUTATION) -
                                         static_cast<qint32>(uiData.mutationBinaryAlgoQuantity)))
        {
            uiData.mutationAlgoIndex =
                static_cast<decltype(uiData.mutationAlgoIndex)>(MutationAlgoId::UNIFORM_MUTATION);
        }
        else if (comboBox->currentIndex() == (static_cast<qint32>(MutationAlgoId::GAUSS_MUTATION) -
                                                static_cast<qint32>(uiData.mutationBinaryAlgoQuantity)))
        {
            uiData.mutationAlgoIndex =
                static_cast<decltype(uiData.mutationAlgoIndex)>(MutationAlgoId::GAUSS_MUTATION);
        }
        else
        {
            result = false;
        }
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

void MainWindow::verifyShowChars(UiData &uiData)
{
    if (ui->showChartsCheckBox->isChecked())
    {
        qDebug() << "Show charts enable";
        const auto functionType = functionObserver.getSelectType();
        const quint8 dim = (functionType.has_value() &&
                            (functionType.value() == FunctionType::BENCHMARK_FUNC)) ? 2U : 3U;
        if((uiData.functionDimension == dim) ||
            (uiData.functionDimension == (dim - 1U)))
        {
            uiData.showCharts = true;
        }
        else
        {
            qDebug() << "Ignore show charts too big dimension";
            uiData.showCharts = false;
        }
    }
    else
    {
        qDebug() << "Show charts disable";
        uiData.showCharts = false;
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

void MainWindow::selectStrategyChangeValue()
{
    if (ui->selectAlgoNameComboBox->currentIndex() ==
        static_cast<qint32>(SelectionAlgoId::TOURNAMENT_SELECTION))
    {
        const UiData& uiData = uiDataHolder.getRef();
        unlockLineEdit(ui->tournamentSizeLine,
                       originalPalette,
                       uiData.tournamentPopulationQuantity);
    }
    else
    {
        lockLineEdit(ui->tournamentSizeLine);
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
