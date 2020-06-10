#include "qenergyloggerdecoder.h"
#include "elfileparser.h"
#include <QFile>
#include <QDebug>
#include <QDataStream>
#include <QByteArray>
#include <QHeaderView>
#include <QThread>
#include <QSettings>
#include <QLabel>
#include <QComboBox>

const int QEnergyLoggerDecoder::K_TABLE_WIDGET_DATE = 0;
const int QEnergyLoggerDecoder::K_TABLE_WIDGET_TENSION = 1;
const int QEnergyLoggerDecoder::K_TABLE_WIDGET_CURRENT = 2;
const int QEnergyLoggerDecoder::K_TABLE_WIDGET_COSPHI = 3;
const int QEnergyLoggerDecoder::K_TABLE_WIDGET_WATT = 4;
const int QEnergyLoggerDecoder::K_TABLE_WIDGET_VA = 5;
const int QEnergyLoggerDecoder::K_TABLE_WIDGET_CUMULATED_CONSUMPTION = 6;
const int QEnergyLoggerDecoder::K_TABLE_WIDGET_ID = 7;

QEnergyLoggerDecoder::QEnergyLoggerDecoder(QWidget *parent)
     : QMainWindow(parent)
{

    QCoreApplication::setApplicationName("EnergyDataDecoder");
    mainLayout = new QHBoxLayout(this);
    QWidget *window = new QWidget(this);
    //mainLayout->addLayout(hLayout);
    window->setLayout(mainLayout);
    setCentralWidget(window);

    QVBoxLayout *controlLayout =  new QVBoxLayout(this);
    mainLayout->addLayout(controlLayout);

    fileMenu = menuBar()->addMenu(tr("&File"));
    helpMenu = menuBar()->addMenu(tr("&Help"));
    aboutQtAction = new QAction(tr("&About Qt"), this);
    aboutQtAction->setStatusTip(tr("Show the Qt library's About box"));
    helpMenu->addAction(aboutQtAction);
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    aboutAction = new QAction(tr("About &QEnergyLoggerDecoder"), this);
    aboutAction->setStatusTip(tr("Show this application's About box"));
    helpMenu->addAction(aboutAction);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    QFormLayout *filesFormLayout = new QFormLayout(this);
    controlLayout->addLayout(filesFormLayout);


    QHBoxLayout *inputFileLayout = new QHBoxLayout(this);
    inputFileLineEdit = new QLineEdit(this);
    QPushButton *inputFilePushButton = new QPushButton(tr("Select"), this);
    inputFileLayout->addWidget(inputFileLineEdit);
    inputFileLayout->addWidget(inputFilePushButton);
    connect(inputFilePushButton, SIGNAL(clicked(bool)), this, SLOT(selectFiles()));
    filesFormLayout->addRow(tr("SD Card directory"), inputFileLayout);


    customPlot = new QCustomPlot(window);
    customPlot->setObjectName(QString::fromUtf8("Graph"));
    controlLayout->addWidget(customPlot,7);
    customPlot->setGeometry(QRect(100, 125, 700, 650));


    QHBoxLayout *configFile = new QHBoxLayout(this);
    configFileLineEdit = new QLineEdit(this);
    QPushButton *configFilePushButton = new QPushButton(tr("Select"), this);
    configFile->addWidget(configFileLineEdit);
    configFile->addWidget(configFilePushButton);
    connect(configFilePushButton, SIGNAL(clicked(bool)), this, SLOT(selectConfigFile()));
    filesFormLayout->addRow(tr("Username file"), configFile);

    // combo box
        combo = new QComboBox(this);
        combo->addItem("Pick User:");
        configFile->addWidget(combo);
        combo->setStyleSheet("QComboBox QAbstractItemView {min-width: 150px;}");


    QHBoxLayout *idFile = new QHBoxLayout(this);
    idFileLineEdit = new QLineEdit(this);
    QPushButton *idFilePushButton = new QPushButton(tr("Select"), this);
    idFile->addWidget(idFileLineEdit);
    idFile->addWidget(idFilePushButton);
    connect(idFilePushButton, SIGNAL(clicked(bool)), this, SLOT(selectidFile()));
    filesFormLayout->addRow(tr("Plug ID file"), idFile);

    // combo box2
        combo2 = new QComboBox(this);
        combo2->addItem("Pick plug ID:");
        idFile->addWidget(combo2);
        combo2->setStyleSheet("QComboBox QAbstractItemView {min-width: 150px;}");


    QHBoxLayout *outputFileLayout = new QHBoxLayout(this);
    outputFileLineEdit = new QLineEdit(this);
    QPushButton *outputFilePushButton = new QPushButton(tr("Select"), this);
    outputFileLayout->addWidget(outputFileLineEdit);
    outputFileLayout->addWidget(outputFilePushButton);
    connect(outputFilePushButton, SIGNAL(clicked(bool)), this, SLOT(selectOutputFile()));
    filesFormLayout->addRow(tr("Output folder"), outputFileLayout);



// commentaire et checkbox

    checkbox = new QCheckBox("Acquisition valide?", this);
    checkbox->setCheckState(Qt::Checked);

    texte = new QLabel(tr("Commentaire :"));
    textEdit = new QTextEdit;
    textEdit->setMaximumHeight(40);
    controlLayout->addWidget(texte, 1 , Qt::AlignBottom);
    controlLayout->addWidget(textEdit);

    controlLayout->addWidget(checkbox, 0, Qt::AlignBottom);

    // Calculate button //
 /*   calculatePushButton = new QPushButton(tr("Calculate"), this);
    connect(calculatePushButton, SIGNAL(clicked(bool)), this, SLOT(makePlot()));
    controlLayout->addWidget(calculatePushButton);
    calculatePushButton->setEnabled(true);
    calculatePushButton->setToolTip("Run once the decoding is done !");
*/


    //---------------------Run button---------------------//
    runPushButton = new QPushButton(tr("Decoder"), this);
    connect(runPushButton, SIGNAL(clicked(bool)), this, SLOT(runFilesDecoding()));
    controlLayout->addWidget(runPushButton);
    runPushButton->setEnabled(false);
    runPushButton->setToolTip("You need to select an input dir in order to decode files !");

  /*  //---------------------Export button---------------------//
    exportPushButton = new QPushButton(tr("Export to CSV"), this);
    connect(exportPushButton, SIGNAL(clicked(bool)), this, SLOT(runExportToCsv()));
    controlLayout->addWidget(exportPushButton);
    exportPushButton->setEnabled(false);
    exportPushButton->setToolTip("You need to select an output file to generate CSV file");
*/
    // rapport button
 /*   QFormLayout *filesFormLayout2 = new QFormLayout(this);
    controlLayout->addLayout(filesFormLayout2);

    QHBoxLayout *outputRapportLayout = new QHBoxLayout(this);
    outputRapportLineEdit = new QLineEdit(this);
    QPushButton *outputRapportPushButton = new QPushButton(tr("Select"), this);
    outputRapportLayout->addWidget(outputRapportLineEdit);
    outputRapportLayout->addWidget(outputRapportPushButton);
    connect(outputRapportPushButton, SIGNAL(clicked(bool)), this, SLOT(selectRapportFile()));
    filesFormLayout2->addRow(tr("report output"), outputRapportLayout); */
     // generate report
    generatePushButton = new QPushButton(tr("Generate report"), this);
    connect(generatePushButton, SIGNAL(clicked(bool)), this, SLOT(generateReport()));
    controlLayout->addWidget(generatePushButton);
    generatePushButton->setEnabled(true);
    generatePushButton->setToolTip("You need to select an output file to generate a report file");


    //---------------------Status bar-----------------------//
    selectedCumulatedConsumption = new QLabel();
    statusBar()->addPermanentWidget(selectedCumulatedConsumption);
    selectedCumulatedConsumption->setText("");
         //---------------------Progress bar---------------------//
    progressBar = new QProgressBar();
    statusBar()->addPermanentWidget(progressBar);

    statusBar()->showMessage("Ready");

    //---------------------table widget---------------------//
    tableWidget = new QTableWidget(1, 8, this);
    tableWidget->setHorizontalHeaderItem(K_TABLE_WIDGET_DATE, new QTableWidgetItem("Date"));
    tableWidget->setHorizontalHeaderItem(K_TABLE_WIDGET_TENSION, new QTableWidgetItem("Tension"));
    tableWidget->setHorizontalHeaderItem(K_TABLE_WIDGET_CURRENT, new QTableWidgetItem("Current"));
    tableWidget->setHorizontalHeaderItem(K_TABLE_WIDGET_COSPHI, new QTableWidgetItem("CosPhi"));
    tableWidget->setHorizontalHeaderItem(K_TABLE_WIDGET_WATT, new QTableWidgetItem("Watt"));
    tableWidget->setHorizontalHeaderItem(K_TABLE_WIDGET_VA, new QTableWidgetItem("VA"));
    tableWidget->setHorizontalHeaderItem(K_TABLE_WIDGET_CUMULATED_CONSUMPTION, new QTableWidgetItem("Cumulated consumption"));
    tableWidget->setHorizontalHeaderItem(K_TABLE_WIDGET_ID, new QTableWidgetItem("ID"));
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    mainLayout->addWidget(tableWidget);

    //tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //qDebug() << tableWidget->horizontalHeader()->length() << tableWidget->verticalHeader()->width();

    tableWidget->setItem(0, K_TABLE_WIDGET_DATE, new QTableWidgetItem("2017-12-15T12:12:12"));
    tableWidget->setItem(0, K_TABLE_WIDGET_TENSION, new QTableWidgetItem("235.6"));
    tableWidget->setItem(0, K_TABLE_WIDGET_CURRENT, new QTableWidgetItem("35.165"));
    tableWidget->setItem(0, K_TABLE_WIDGET_COSPHI, new QTableWidgetItem("0.99"));
    tableWidget->setItem(0, K_TABLE_WIDGET_WATT, new QTableWidgetItem("15000.00"));
    tableWidget->setItem(0, K_TABLE_WIDGET_VA, new QTableWidgetItem("15000.00"));
    tableWidget->setItem(0, K_TABLE_WIDGET_CUMULATED_CONSUMPTION, new QTableWidgetItem("9999.99"));
    tableWidget->setItem(0, K_TABLE_WIDGET_ID, new QTableWidgetItem("0"));

    tableWidget->resizeColumnsToContents();
    tableWidget->setMinimumWidth(tableWidget->horizontalHeader()->length() + tableWidget->verticalHeader()->width()+15);
    tableWidget->clearContents();
    tableWidget->setRowCount(0);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(tableWidget, SIGNAL(itemSelectionChanged()), this, SLOT(sumupConsumption()));



    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "QEnergyLoggerDecoder");
//    QStringList itemsInComboBox;
    settings.beginGroup("files");
    inputFileLineEdit->setText(settings.value("input").toString());
    outputFileLineEdit->setText(settings.value("output").toString());
    configFileLineEdit->setText(settings.value("config").toString());
    idFileLineEdit->setText(settings.value("id").toString());

 //   outputRapportLineEdit->setText(settings.value("rapport").toString());
 /*   for (int index = 0; index < combo->count(); index++)
        itemsInComboBox << combo->itemText(index);
*/
    settings.endGroup();

    //connect slots AFTER settings reading in order to avoid concurrent modifying of text when restoring settings.
    connect(outputFileLineEdit, SIGNAL(textChanged(QString)), this, SLOT(inputOutputConfigFileLineEditChange()));
    connect(inputFileLineEdit, SIGNAL(textChanged(QString)), this, SLOT(inputOutputConfigFileLineEditChange()));
    connect(configFileLineEdit, SIGNAL(textChanged(QString)), this, SLOT(inputOutputConfigFileLineEditChange()));
    connect(idFileLineEdit, SIGNAL(textChanged(QString)), this, SLOT(inputOutputConfigFileLineEditChange()));


    //Enable buttons if files are restored from QSettings
    inputOutputConfigFileLineEditChange();
    QString contenu = configFileLineEdit->text();
    qDebug() << contenu;
    combo->clear();
    combo->addItem("Pick user:");
    QFile inputFile(contenu);
    inputFile.open(QIODevice::ReadOnly);

      QTextStream in(&inputFile);
      while (!in.atEnd())
      {
         QString line = in.readLine();
         combo->addItem(line);
      }
      inputFile.close();

      statusBar()->showMessage("Users successfully imported.");
      QString contenu2 = idFileLineEdit->text();

      qDebug() << contenu2;
      combo2->clear();
      combo2->addItem("Pick outlet ID:");

      QFile inputFile2(contenu2);
      inputFile2.open(QIODevice::ReadOnly);

        QTextStream in2(&inputFile2);
        while (!in2.atEnd())
        {
           QString line2 = in2.readLine();
           combo2->addItem(line2);
        }
        inputFile2.close();
        statusBar()->showMessage("Plug IDs successfully imported.");



    // import users button //
/*    importerPushButton = new QPushButton(tr("Import users"), this);
    connect(importerPushButton, SIGNAL(clicked(bool)), this, SLOT(importUsers()));
    configFile->addWidget(importerPushButton);
    importerPushButton->setEnabled(true);
    importerPushButton->setToolTip("Run to import users once you chose user file !"); */

    qDebug() << outputFileLineEdit->text()+"/bin" ;
}


void QEnergyLoggerDecoder::inputOutputConfigFileLineEditChange()
{

    if(outputFileLineEdit->text().trimmed().isEmpty())
    {
        runPushButton->setEnabled(false);
        runPushButton->setToolTip("You need to select an output file to report txt file");
    }
    else
    {
        runPushButton->setEnabled(true);
        runPushButton->setToolTip("");
}
            if(outputFileLineEdit->text().trimmed().isEmpty())
            {
                generatePushButton->setEnabled(false);
                generatePushButton->setToolTip("You need to select an output file to report txt file");
            }
            else
            {
                generatePushButton->setEnabled(true);
                generatePushButton->setToolTip("");
}

    if(inputFileLineEdit->text().trimmed().isEmpty())
    {
        runPushButton->setEnabled(false);
        runPushButton->setToolTip("You need to select an input dir in order to decode files !");
    }
    else
    {
        runPushButton->setEnabled(true);
        runPushButton->setToolTip("");
    }

    //reset runPushButton text (in case of previous failure)
    runPushButton->setText("Run");

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "QEnergyLoggerDecoder");
    settings.beginGroup("files");
    settings.setValue("input", inputFileLineEdit->text());
    settings.setValue("output", outputFileLineEdit->text());
    settings.setValue("config", configFileLineEdit->text());
   // settings.setValue("rapport", outputRapportLineEdit->text());
    settings.setValue("id", idFileLineEdit->text());
    settings.endGroup();
}

void QEnergyLoggerDecoder::noFileToDecode()
{
    statusBar()->showMessage("Failure : no file to decode in this directory");
    runPushButton->setText("Failure. Select another directory !");
}

void QEnergyLoggerDecoder::runFilesDecoding()
{
    eldh.elData->clear();
    eldh.dirPath = inputFileLineEdit->text();

    ELFileParser *elfp = new ELFileParser(&eldh);

    QThread *thread = new QThread(this);
    elfp->moveToThread(thread);

    statusBar()->showMessage("Decoding files... Please wait !");

    runPushButton->setText(tr("Decoding files... Please wait !"));
    runPushButton->setEnabled(false);

    selectedCumulatedConsumption->setText("");

    thread->start();

    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(elfp, SIGNAL(finished()), elfp, SLOT(deleteLater()));

    connect(thread, SIGNAL(started()), elfp, SLOT(runFilesDecoding()));
    connect(elfp, SIGNAL(finished()), this, SLOT(populateTable()));
    connect(elfp, SIGNAL(finished()), this, SLOT(makePlot()));
//    connect(elfp, SIGNAL(finished()), this, SLOT(generateReport()));
    connect(elfp, SIGNAL(finished()), this, SLOT(runExportToCsv()));


    connect(elfp, SIGNAL(noFile()), this, SLOT(noFileToDecode()));
    connect(elfp, SIGNAL(bytesToBeProcessed(int)), this, SLOT(setProgressBarMaximum(int)));
    connect(elfp, SIGNAL(bytesProcessed(int)), this, SLOT(updateProgressBar(int)));


    thread->connect(elfp,
                    SIGNAL(finished()),
                    SLOT(quit()));

    QString sDate = QDateTime::currentDateTime().toString("yyyy_MM_dd");
    QString path =outputFileLineEdit->text()+"/"+combo2->currentText()+"_"+sDate;


    QDir dir(path+"/bin");

    if (!dir.exists())
        dir.mkpath(".");

    copyPath(inputFileLineEdit->text(),path+"/bin");

    QDesktopServices::openUrl(QUrl::fromLocalFile(outputFileLineEdit->text()));
}

void QEnergyLoggerDecoder::setProgressBarMaximum(int byteToProceed)
{
    progressBar->setMaximum(byteToProceed);
    progressBar->reset();
}
void QEnergyLoggerDecoder::makePlot(){

    // generate some data:
    QVector<double> dates(eldh.elData->size());
    QVector<double> voltages(eldh.elData->size());
    QVector<double> va(eldh.elData->size());
    QVector<double> power(eldh.elData->size());
    QVector<double> consumed(eldh.elData->size());
    QVector<double> current(eldh.elData->size());


   // QVector<double> x(101), y(101); // initialize with entries 0..100
    for (int i=0; i<eldh.elData->size(); ++i)
    {
      dates[i]= eldh.elData->at(i).date.toSecsSinceEpoch();
      voltages[i]= eldh.elData->at(i).voltage;
      current[i]= eldh.elData->at(i).current;
      power[i]= eldh.elData->at(i).voltage*eldh.elData->at(i).current;
      va[i]=eldh.elData->at(i).voltage*eldh.elData->at(i).current*((eldh.elData->at(i).cosPhi+1)/100);
      consumed[i]=eldh.elData->at(i).consumedPower;

    }
    // create graph and assign data to it:    
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                    QCP::iSelectLegend | QCP::iSelectPlottables);
    customPlot->addGraph();
    customPlot->graph(0)->setData(dates, voltages);
    customPlot->graph(0)->setName(QString("tension").arg(customPlot->graphCount()-1));
    customPlot->axisRect()->setRangeDrag(Qt::Horizontal);   // Enable only drag along the horizontal axis
    customPlot->axisRect()->setRangeZoom(Qt::Horizontal);   // Enable zoom only on the horizontal axis
    customPlot->axisRect()->setupFullAxesBox();
    customPlot->legend->setVisible(true);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    customPlot->legend->setFont(legendFont);
    customPlot->legend->setSelectedFont(legendFont);
    customPlot->legend->setSelectableParts(QCPLegend::spItems);
    customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);   // Label coordinates along the X axis as the Date and Time
    customPlot->xAxis->setDateTimeFormat("yyyy-MM-dd\nhh:mm:ss");  // Set the date and time format
    customPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    customPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
    customPlot->xAxis->setAutoTickStep(true);
    customPlot->yAxis->setTickLabelColor(QColor(Qt::red));
    // give the axes some labels:
    customPlot->xAxis->setLabel("Timestamp");
    customPlot->yAxis->setLabel("Multiple axes");
    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(dates[0],dates[eldh.elData->size()-1]);
    customPlot->yAxis->setRange(voltages[0]-10,voltages[eldh.elData->size()-1]+20);
    customPlot->addGraph();
    customPlot->graph(1)->setName(QString("power(watt)").arg(customPlot->graphCount()-1));
    customPlot->graph(1)->setData(dates, power);
    customPlot->graph(1)->setLineStyle((QCPGraph::LineStyle)(rand()%5+1));
    if (rand()%100 > 50)
    customPlot->graph(1)->setScatterStyle(QCPScatterStyle((QCPScatterStyle::ScatterShape)(rand()%14+1)));
    QPen graphPen;
    graphPen.setColor(QColor(rand()%245+10, rand()%245+10, rand()%245+10));
    graphPen.setWidthF(rand()/(double)RAND_MAX*2+1);
    customPlot->graph(1)->setPen(graphPen);
    customPlot->addGraph();
    customPlot->graph(2)->setName(QString("puissance active(p*cosphi)").arg(customPlot->graphCount()-1));
    customPlot->graph(2)->setData(dates, va);
    customPlot->graph(2)->setLineStyle((QCPGraph::LineStyle)(rand()%5+1));
    if (rand()%100 > 50)
    customPlot->graph(1)->setScatterStyle(QCPScatterStyle((QCPScatterStyle::ScatterShape)(rand()%14+1)));
    graphPen.setColor(QColor(rand()%245+10, rand()%245+10, rand()%245+10));
    graphPen.setWidthF(rand()/(double)RAND_MAX*2+1);
    customPlot->graph(2)->setPen(graphPen);
    customPlot->replot();
    customPlot->rescaleAxes();
    // connect slot that ties some axis selections together (especially opposite axes):
    connect(customPlot, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));
    // connect slots that takes care that when an axis is selected, only that direction can be dragged and zoomed:
    connect(customPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
    connect(customPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));

    // make bottom and left axes transfer their ranges to top and right axes:
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

    // connect some interaction slots:
    connect(customPlot, SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)), this, SLOT(axisLabelDoubleClick(QCPAxis*,QCPAxis::SelectablePart)));
    connect(customPlot, SIGNAL(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)), this, SLOT(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*)));

    // setup policy and connect slot for context menu popup:
    customPlot->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(customPlot, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));


}

void QEnergyLoggerDecoder::updateProgressBar(int numberOfNewBytesProcessed)
{
    int newValue = progressBar->value() + numberOfNewBytesProcessed;
    progressBar->setValue(newValue);
}

void QEnergyLoggerDecoder::generateReport()
{

    QString sDate = QDateTime::currentDateTime().toString("yyyy_MM_dd");
    QString path =outputFileLineEdit->text()+"/"+combo2->currentText()+"_"+sDate;

    QFile output(path+"/"+combo2->currentText()+"_"+sDate+".report.txt");


    output.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
    QTextStream stream(&output);
        stream << "Date;" << sDate << endl;
        stream << "InPutFile;" << inputFileLineEdit->text() << endl;
        stream << "OutPutFile;" << outputFileLineEdit->text() << endl;
        stream << "User;" << combo->currentText() << endl;
        stream << "outletID;" << combo2->currentText() << endl;
        stream << "Comment;" <<  textEdit->toPlainText()  << endl;
        if(checkbox->isChecked())
        {
            stream <<"Validity;"<< 1 <<endl;
        }
        else {
            stream <<"Validity;"<< 0 <<endl;
        }

    output.close();
    statusBar()->showMessage("Report file successfully written");

}


void QEnergyLoggerDecoder::runExportToCsv()
{
    QString sDate = QDateTime::currentDateTime().toString("yyyy_MM_dd");

    QString path =outputFileLineEdit->text()+"/"+combo2->currentText()+"_"+sDate;

    QFile output(path+"/"+combo2->currentText()+"_"+sDate+".csv");

    output.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
    QTextStream stream(&output);
    for(int k=0;k<eldh.elData->size();k++)
    {
        stream << eldh.elData->at(k).date.toString(Qt::ISODate) << ";" << eldh.elData->at(k).voltage << ";" << eldh.elData->at(k).current << ";" << eldh.elData->at(k).cosPhi << "\n";
    } // eldh.elData->at(k).date.toString(Qt::ISODate)
    output.close();
    statusBar()->showMessage("CSV file successfully written");

}

void QEnergyLoggerDecoder::populateTable()
{
    progressBar->setValue(progressBar->maximum());

       tableWidget->hide();
       tableWidget->clearContents();
       tableWidget->setRowCount(eldh.elData->size());

       double powerConsumed=0;

       QVector<elDataHandler::DataInfo>::const_reverse_iterator it;
       it = eldh.elData->crbegin();
       int k = 0;

       while (it != eldh.elData->crend())
       {
           tableWidget->setItem(k, K_TABLE_WIDGET_DATE, new QTableWidgetItem(it->date.toString(Qt::ISODate)));
           tableWidget->setItem(k, K_TABLE_WIDGET_TENSION, new QTableWidgetItem(QString::number(it->voltage)));
           tableWidget->setItem(k, K_TABLE_WIDGET_CURRENT, new QTableWidgetItem(QString::number(it->current)));
           tableWidget->setItem(k, K_TABLE_WIDGET_COSPHI, new QTableWidgetItem(QString::number(((it->cosPhi)+1)/100)));

           tableWidget->setItem(k, K_TABLE_WIDGET_WATT, new QTableWidgetItem(QString::number(it->voltage*it->current, 'f', 2)));
           tableWidget->setItem(k, K_TABLE_WIDGET_VA, new QTableWidgetItem(QString::number(it->voltage*it->current*((it->cosPhi+1)/100), 'f', 2)));

           powerConsumed=powerConsumed + (it->consumedPower);
           tableWidget->setItem(k, K_TABLE_WIDGET_CUMULATED_CONSUMPTION, new QTableWidgetItem(QString::number(powerConsumed, 'f', 2)));
           tableWidget->setItem(k, K_TABLE_WIDGET_ID, new QTableWidgetItem(QString::number(it->id)));

           it++;
           k++;
       }

       tableWidget->show();

       tableWidget->resizeColumnsToContents();
       tableWidget->setMinimumWidth(tableWidget->horizontalHeader()->length() + tableWidget->verticalHeader()->width() +30);

       runPushButton->setText(tr("Run"));
       runPushButton->setEnabled(true);

       statusBar()->showMessage("Files decoding successful");
}

void QEnergyLoggerDecoder::selectFiles()
{
    QString filename = (QFileDialog::getExistingDirectory(this, "Select directory where binary files are located", QDir::homePath()));
    inputFileLineEdit->setText(filename);
}

void QEnergyLoggerDecoder::copyPath(QString src, QString dst)
{
    QDir dir(src);
    if (! dir.exists())
        return;

    foreach (QString d, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QString dst_path = dst + QDir::separator() + d;
        dir.mkpath(dst_path);
        copyPath(src+ QDir::separator() + d, dst_path);
        qDebug() << "test  1 2 ";
        qDebug() << inputFileLineEdit->text();


    }

    foreach (QString f, dir.entryList(QDir::Files)) {
        QFile::copy(src + QDir::separator() + f, dst + QDir::separator() + f);
        qDebug() << "test debuggage 1 2 ";
        qDebug() << outputFileLineEdit->text();
    }
}


void QEnergyLoggerDecoder::selectOutputFile()
{
   // QString filename = QFileDialog::getSaveFileName(this, tr("Select where to save CSV output file"), QDir::homePath(), tr("CSV files (*.csv)"));
    QString filename = (QFileDialog::getExistingDirectory(this, "Select your output directory, for your CSV, and report files", QDir::homePath()));

    outputFileLineEdit->setText(filename);

}

/* void QEnergyLoggerDecoder::selectRapportFile()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Select where to save txt report file"), QDir::homePath(), tr("TXT files (*.txt)"));
    outputRapportLineEdit->setText(filename);
} */


void QEnergyLoggerDecoder::selectConfigFile()
{
    QString filename = (QFileDialog::getOpenFileName(this, "Select path of the username file ", QDir::homePath(),tr("txt files (*.txt)")));
    configFileLineEdit->setText(filename);
    QString contenu = configFileLineEdit->text();

    qDebug() << contenu;
    combo->clear();
    combo->addItem("Pick user:");

    QFile inputFile(contenu);
    inputFile.open(QIODevice::ReadOnly);

      QTextStream in(&inputFile);
      while (!in.atEnd())
      {
         QString line = in.readLine();
         combo->addItem(line);
      }
      inputFile.close();
      statusBar()->showMessage("Users successfully imported.");

}
void QEnergyLoggerDecoder::selectidFile()
{
    QString filename = (QFileDialog::getOpenFileName(this, "Select path of the outlet IDs file ", QDir::homePath(),tr("txt files (*.txt)")));
    idFileLineEdit->setText(filename);
    QString contenu2 = idFileLineEdit->text();

    qDebug() << contenu2;
    combo2->clear();
    combo2->addItem("Pick outlet ID:");

    QFile inputFile(contenu2);
    inputFile.open(QIODevice::ReadOnly);

      QTextStream in(&inputFile);
      while (!in.atEnd())
      {
         QString line = in.readLine();
         combo2->addItem(line);
      }
      inputFile.close();
      statusBar()->showMessage("Plug IDs successfully imported.");

}


void QEnergyLoggerDecoder::sumupConsumption()
{
    QList<QTableWidgetItem*> selectedItem = tableWidget->selectedItems();
    QList<QTableWidgetSelectionRange> ranges = tableWidget->selectedRanges();
    double cumulatedCons = 0;
    int numberOfSamples = 0;
    if(ranges.size() > 0)
    {
        //for each range, look for consumption and sum each line between top and bottom row
        for (int k = 0; k < ranges.size() ; k++)
        {
            for (int j = ranges.at(k).topRow() ; j <= ranges.at(k).bottomRow() ; j++)
            {
                cumulatedCons = cumulatedCons + eldh.elData->at(eldh.elData->size()-1-j).consumedPower;
                numberOfSamples++;
            }
        }
        selectedCumulatedConsumption->setText(QString::number(cumulatedCons, 'f' , 2) + " Wh in " + QString::number(numberOfSamples) + " minutes");
    }
}

void QEnergyLoggerDecoder::about()
{
    QMessageBox::about(this, tr("About QEnergyLoggerDecoder"),
            tr("<b>QEnergyLoggerDecoder v 1.1</b> < br />< br /> "
               "A software aimed to decode binary files produced by Voltcraft Energy Logger 4000 (and maybe 3500). <br /><br />"
               "You can download updates on the official website of the project"
               " <a href=\"http://afterflight.org\">http://afterflight.org</a>.<br />< br/>"
               //"Icon by <a href=\"https://www.iconfinder.com/icons/897231/airplane_destination_fly_map_resolutions_travel_vacation_icon\">Laura Reen</a>.< br />< br />< br />"
               " This program is free software: you can redistribute it and/or modify"
               " it under the terms of the GNU General Public License as published by"
               " the Free Software Foundation, either version 3 of the License, or"
               " (at your option) any later version."
               "<br />< br/>"
               "This program is distributed in the hope that it will be useful,"
               " but WITHOUT ANY WARRANTY; without even the implied warranty of"
               " MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the"
               " GNU General Public License for more details."
               "<br />< br/>"
               "You should have received a copy of the GNU General Public License"
               " along with this program.  If not, see <a href=\"http://www.gnu.org/licenses/\">http://www.gnu.org/licenses/</a>."));
}

QEnergyLoggerDecoder::~QEnergyLoggerDecoder()
{

}

void QEnergyLoggerDecoder::axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part)
{
  // Set an axis label by double clicking on it
  if (part == QCPAxis::spAxisLabel) // only react when the actual axis label is clicked, not tick label or axis backbone
  {
    bool ok;
    QString newLabel = QInputDialog::getText(this, "QCustomPlot example", "New axis label:", QLineEdit::Normal, axis->label(), &ok);
    if (ok)
    {
      axis->setLabel(newLabel);
      customPlot->replot();
    }
  }
}

void QEnergyLoggerDecoder::legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item)
{
  // Rename a graph by double clicking on its legend item
  Q_UNUSED(legend)
  if (item) // only react if item was clicked (user could have clicked on border padding of legend where there is no item, then item is 0)
  {
    QCPPlottableLegendItem *plItem = qobject_cast<QCPPlottableLegendItem*>(item);
    bool ok;
    QString newName = QInputDialog::getText(this, "QCustomPlot example", "New graph name:", QLineEdit::Normal, plItem->plottable()->name(), &ok);
    if (ok)
    {
      plItem->plottable()->setName(newName);
      customPlot->replot();
    }
  }
}

void QEnergyLoggerDecoder::selectionChanged()
{

  // make top and bottom axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || customPlot->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      customPlot->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || customPlot->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    customPlot->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    customPlot->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }
  // make left and right axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || customPlot->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      customPlot->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || customPlot->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    customPlot->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    customPlot->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }

  // synchronize selection of graphs with selection of corresponding legend items:
  for (int i=0; i<customPlot->graphCount(); ++i)
  {
    QCPGraph *graph = customPlot->graph(i);
    QCPPlottableLegendItem *item = customPlot->legend->itemWithPlottable(graph);
    if (item->selected() || graph->selected())
    {
      item->setSelected(true);
    }
  }
}

void QEnergyLoggerDecoder::mousePress()
{
  // if an axis is selected, only allow the direction of that axis to be dragged
  // if no axis is selected, both directions may be dragged

  if (customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    customPlot->axisRect()->setRangeDrag(customPlot->xAxis->orientation());
  else if (customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    customPlot->axisRect()->setRangeDrag(customPlot->yAxis->orientation());
  else
    customPlot->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

void QEnergyLoggerDecoder::mouseWheel()
{
  // if an axis is selected, only allow the direction of that axis to be zoomed
  // if no axis is selected, both directions may be zoomed

  if (customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    customPlot->axisRect()->setRangeZoom(customPlot->xAxis->orientation());
  else if (customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    customPlot->axisRect()->setRangeZoom(customPlot->yAxis->orientation());
  else
    customPlot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}

void QEnergyLoggerDecoder::removeSelectedGraph()
{
  if (customPlot->selectedGraphs().size() > 0)
  {
    customPlot->removeGraph(customPlot->selectedGraphs().first());
    customPlot->replot();
  }
}

void QEnergyLoggerDecoder::removeAllGraphs()
{
  customPlot->clearGraphs();
  customPlot->replot();
}

void QEnergyLoggerDecoder::contextMenuRequest(QPoint pos)
{
  QMenu *menu = new QMenu(this);
  menu->setAttribute(Qt::WA_DeleteOnClose);

  if (customPlot->legend->selectTest(pos, false) >= 0) // context menu on legend requested
  {
    menu->addAction("Move to top left", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignLeft));
    menu->addAction("Move to top center", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignHCenter));
    menu->addAction("Move to top right", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignRight));
    menu->addAction("Move to bottom right", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignRight));
    menu->addAction("Move to bottom left", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignLeft));
  } else  // general context menu on graphs requested
  {
    menu->addAction("remake plots", this, SLOT(makePlot()));
    if (customPlot->selectedGraphs().size() > 0)
      menu->addAction("Remove selected graph", this, SLOT(removeSelectedGraph()));
    if (customPlot->graphCount() > 0)
      menu->addAction("Remove all graphs", this, SLOT(removeAllGraphs()));
  }

  menu->popup(customPlot->mapToGlobal(pos));
}

void QEnergyLoggerDecoder::moveLegend()
{
  if (QAction* contextAction = qobject_cast<QAction*>(sender())) // make sure this slot is really called by a context menu action, so it carries the data we need
  {
    bool ok;
    int dataInt = contextAction->data().toInt(&ok);
    if (ok)
    {
      customPlot->axisRect()->insetLayout()->setInsetAlignment(0, (Qt::Alignment)dataInt);
      customPlot->replot();
    }
  }
}


