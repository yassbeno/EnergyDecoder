#ifndef QENERGYLOGGERDECODER_H
#define QENERGYLOGGERDECODER_H

#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QVector>
#include <QDateTime>
#include <QThread>
#include <QTextEdit>
#include <QFileDialog>
#include <QDir>

//GUI items headers
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QProgressBar>
#include <QStatusBar>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QTableWidget>
#include "qcustomplot.h"
#include "eldatahandler.h"
#include <QComboBox>




class QEnergyLoggerDecoder : public QMainWindow
{
    Q_OBJECT

public:
    QEnergyLoggerDecoder(QWidget *parent = 0);
    ~QEnergyLoggerDecoder();

private:

    QWidget *centralwidget;
    QLabel *texte;
    QMenu *fileMenu;
    QMenu *helpMenu;
    QAction *aboutQtAction;
    QAction *aboutAction;
    QTextEdit *textEdit;
    QComboBox *combo;
    QComboBox *combo2;
    QCheckBox *checkbox;
    QPushButton *runPushButton;
    QPushButton *exportPushButton;
    QPushButton *calculatePushButton;
    QPushButton *outputRapportPushButton;

    QProgressBar *progressBar;
    QLabel *selectedCumulatedConsumption;

    QLineEdit *inputFileLineEdit;
    QPushButton *inputFilePushButton;
    QLineEdit *outputFileLineEdit;
    QPushButton *outputFilePushButton;
    QPushButton *generatePushButton;

    QLineEdit *outputRapportLineEdit;
    QLineEdit *configFileLineEdit;
    QLineEdit *idFileLineEdit;
    QPushButton *configFilePushButton;
    QPushButton *idFilePushButton;


    QTableWidget *tableWidget;

    //Layouts
    QHBoxLayout *mainLayout;

    QCustomPlot *customPlot;


    elDataHandler eldh;

    //QThread* thread;

    static const int K_TABLE_WIDGET_DATE;
    static const int K_TABLE_WIDGET_TENSION;
    static const int K_TABLE_WIDGET_CURRENT;
    static const int K_TABLE_WIDGET_COSPHI;
    static const int K_TABLE_WIDGET_WATT;
    static const int K_TABLE_WIDGET_VA;
    static const int K_TABLE_WIDGET_CUMULATED_CONSUMPTION;
    static const int K_TABLE_WIDGET_ID;

private slots:
    void about();
    void selectFiles();
    void selectOutputFile();
    void runFilesDecoding();
    void runExportToCsv();
    void populateTable();
    void setProgressBarMaximum(int byteToProceed);
    void updateProgressBar(int numberOfNewBytesProcessed);
    void noFileToDecode();
    void sumupConsumption();
    void makePlot();
    void axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part);
    void legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item);
    void selectionChanged();
    void mousePress();
    void mouseWheel();
    void removeSelectedGraph();
    void removeAllGraphs();
    void contextMenuRequest(QPoint pos);
    void moveLegend();
    void selectConfigFile();
    void inputOutputConfigFileLineEditChange();
    void generateReport();
//    void selectRapportFile();
    void selectidFile();
    void copyPath(QString src, QString dst);
};

#endif // QENERGYLOGGERDECODER_H
