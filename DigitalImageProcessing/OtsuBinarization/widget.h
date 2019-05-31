#ifndef WIDGET_H
#define WIDGET_H

#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtGui/QImage>
#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtDataVisualization/QHeightMapSurfaceDataProxy>
#include <QtDataVisualization/QSurface3DSeries>
#include <QtWidgets/QSlider>

QT_BEGIN_NAMESPACE
class QComboBox;
class QCheckBox;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QChart;
QT_CHARTS_END_NAMESPACE

typedef QPointF Data;
typedef QList<Data> DataList;
typedef QList<DataList> DataTable;

QT_CHARTS_USE_NAMESPACE

using namespace QtDataVisualization;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

    void adjustXMin(int min);
    void adjustXMax(int max);
    void adjustZMin(int min);
    void adjustZMax(int max);
    void setBlackToYellowGradient();
    void setGreenToRedGradient();
    void toggleModeNone() { m_graph->setSelectionMode(QAbstract3DGraph::SelectionNone); }
    void toggleModeItem() { m_graph->setSelectionMode(QAbstract3DGraph::SelectionItem); }
    void toggleModeSliceRow() { m_graph->setSelectionMode(QAbstract3DGraph::SelectionItemAndRow
                                                          | QAbstract3DGraph::SelectionSlice); }
    void toggleModeSliceColumn() { m_graph->setSelectionMode(QAbstract3DGraph::SelectionItemAndColumn
                                                             | QAbstract3DGraph::SelectionSlice); }

public slots:
    void on_pushbutton_1_clicked();
    void on_pushbutton_2_clicked();
    void on_pushbutton_3_clicked();
    void on_pushbutton_4_clicked();
    void on_pushbutton_5_clicked();

private Q_SLOTS:
    void updateUI();

private:
    QComboBox *createThemeBox() const;
    QChart *createSplineChart() const;

private:
    QGridLayout *m_baseLayout;
    QHBoxLayout *m_hLayout;
    QVBoxLayout *m_vLayout;
    QChartView *m_chartView;
    QComboBox *m_themeComboBox;
    QList<QChartView *> m_charts;
    DataTable m_dataTable;
    QGraphicsView *m_graphicsView;
    QPushButton *m_pushbutton_1;
    QPushButton *m_pushbutton_2;
    QPushButton *m_pushbutton_3;
    QPushButton *m_pushbutton_4;
    QPushButton *m_pushbutton_5;
    QImage *m_inputImage;
    QImage *m_grayImage;
    QImage *m_binaryImage;

    Q3DSurface *m_graph;
    QWidget *m_container;

    QHeightMapSurfaceDataProxy *m_heightMapProxy;
    QSurface3DSeries *m_heightMapSeries;
    int m_heightMapWidth;
    int m_heightMapHeight;
    QSlider *m_axisMinSliderX;
    QSlider *m_axisMaxSliderX;
    QSlider *m_axisMinSliderZ;
    QSlider *m_axisMaxSliderZ;
    float m_rangeMinX;
    float m_rangeMinZ;
    float m_stepX;
    float m_stepZ;
};

#endif // WIDGET_H
