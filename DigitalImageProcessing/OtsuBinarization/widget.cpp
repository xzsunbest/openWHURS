#include "widget.h"
#include <cstring>

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QAbstractBarSeries>
#include <QtCharts/QPercentBarSeries>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QLegend>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtGui/QPalette>
#include <QtGui/QPixmap>
#include <QtGui/QRgb>
#include <QtCore/QMap>
#include <QtCore/QTime>
#include <QtCore/QSize>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtCharts/QBarCategoryAxis>
#include <QtCore/QDebug>

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QGroupBox>
#include <QtGui/QPainter>
#include <QtGui/QScreen>

const int heightMapGridStepX = 2;
const int heightMapGridStepZ = 2;

Widget::Widget(QWidget *parent)
    : QWidget(parent),
      m_baseLayout(new QGridLayout()),
      m_hLayout(new QHBoxLayout()),
      m_themeComboBox(createThemeBox()),
      m_graphicsView(new QGraphicsView()),
      m_pushbutton_1(new QPushButton("Open")),
      m_pushbutton_2(new QPushButton("Gradation")),
      m_pushbutton_3(new QPushButton("Binarization")),
      m_pushbutton_5(new QPushButton("Binarization_2")),
      m_pushbutton_4(new QPushButton("Save")),
      m_inputImage(new QImage()),
      m_axisMaxSliderX(new QSlider(Qt::Horizontal)),
      m_axisMaxSliderZ(new QSlider(Qt::Horizontal)),
      m_axisMinSliderX(new QSlider(Qt::Horizontal)),
      m_axisMinSliderZ(new QSlider(Qt::Horizontal)),
      m_container(nullptr),
      m_chartView(nullptr)
{
    connect(m_themeComboBox,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &Widget::updateUI);
    connect(m_pushbutton_1,SIGNAL(clicked()),
            this,SLOT(on_pushbutton_1_clicked()));
    connect(m_pushbutton_2,SIGNAL(clicked()),
            this,SLOT(on_pushbutton_2_clicked()));
    connect(m_pushbutton_3,SIGNAL(clicked()),
            this,SLOT(on_pushbutton_3_clicked()));
    connect(m_pushbutton_4,SIGNAL(clicked()),
            this,SLOT(on_pushbutton_4_clicked()));
    connect(m_pushbutton_5,SIGNAL(clicked()),
            this,SLOT(on_pushbutton_5_clicked()));

    QHBoxLayout *settingsLayout=new QHBoxLayout();
    settingsLayout->addWidget(m_pushbutton_1);
    settingsLayout->addWidget(m_pushbutton_2);
    settingsLayout->addWidget(m_pushbutton_3);
    settingsLayout->addWidget(m_pushbutton_5);
    settingsLayout->addWidget(m_pushbutton_4);
    settingsLayout->addWidget(new QLabel("Theme:"));
    settingsLayout->addWidget(m_themeComboBox);
    settingsLayout->setMargin(10);
    settingsLayout->setSpacing(10);
    settingsLayout->addStretch();
    m_baseLayout->addLayout(settingsLayout, 0, 0, 1, 2);

    m_hLayout->addWidget(m_graphicsView,1);
    m_baseLayout->addLayout(m_hLayout,1,0);
    setLayout(m_baseLayout);

    m_vLayout = new QVBoxLayout();
    m_hLayout->addLayout(m_vLayout,0);
    m_vLayout->setAlignment(Qt::AlignRight);

    QGroupBox *selectionGroupBox = new QGroupBox(QStringLiteral("Selection Mode"));

    QRadioButton *modeNoneRB = new QRadioButton();
    modeNoneRB->setText(QStringLiteral("No selection"));
    modeNoneRB->setChecked(false);

    QRadioButton *modeItemRB = new QRadioButton();
    modeItemRB->setText(QStringLiteral("Item"));
    modeItemRB->setChecked(false);

    QRadioButton *modeSliceRowRB = new QRadioButton();
    modeSliceRowRB->setText(QStringLiteral("Row Slice"));
    modeSliceRowRB->setChecked(false);

    QRadioButton *modeSliceColumnRB = new QRadioButton();
    modeSliceColumnRB->setText(QStringLiteral("Column Slice"));
    modeSliceColumnRB->setChecked(false);

    QVBoxLayout *selectionVBox = new QVBoxLayout;
    selectionVBox->addWidget(modeNoneRB);
    selectionVBox->addWidget(modeItemRB);
    selectionVBox->addWidget(modeSliceRowRB);
    selectionVBox->addWidget(modeSliceColumnRB);
    selectionGroupBox->setLayout(selectionVBox);

    m_axisMinSliderX->setMinimum(0);
    m_axisMinSliderX->setTickInterval(1);
    m_axisMinSliderX->setEnabled(true);

    m_axisMaxSliderX->setMinimum(1);
    m_axisMaxSliderX->setTickInterval(1);
    m_axisMaxSliderX->setEnabled(true);

    m_axisMinSliderZ->setMinimum(0);
    m_axisMinSliderZ->setTickInterval(1);
    m_axisMinSliderZ->setEnabled(true);

    m_axisMaxSliderZ->setMinimum(1);
    m_axisMaxSliderZ->setTickInterval(1);
    m_axisMaxSliderZ->setEnabled(true);

    QGroupBox *colorGroupBox = new QGroupBox(QStringLiteral("Custom gradient"));

    QLinearGradient grBtoY(0, 0, 1, 100);
    grBtoY.setColorAt(1.0, Qt::black);
    grBtoY.setColorAt(0.67, Qt::blue);
    grBtoY.setColorAt(0.33, Qt::red);
    grBtoY.setColorAt(0.0, Qt::yellow);
    QPixmap pm(24, 100);
    QPainter pmp(&pm);
    pmp.setBrush(QBrush(grBtoY));
    pmp.setPen(Qt::NoPen);
    pmp.drawRect(0, 0, 24, 100);
    QPushButton *gradientBtoYPB = new QPushButton();
    gradientBtoYPB->setIcon(QIcon(pm));
    gradientBtoYPB->setIconSize(QSize(24, 100));

    QLinearGradient grGtoR(0, 0, 1, 100);
    grGtoR.setColorAt(1.0, Qt::darkGreen);
    grGtoR.setColorAt(0.5, Qt::yellow);
    grGtoR.setColorAt(0.2, Qt::red);
    grGtoR.setColorAt(0.0, Qt::darkRed);
    pmp.setBrush(QBrush(grGtoR));
    pmp.drawRect(0, 0, 24, 100);
    QPushButton *gradientGtoRPB = new QPushButton();
    gradientGtoRPB->setIcon(QIcon(pm));
    gradientGtoRPB->setIconSize(QSize(24, 100));

    QHBoxLayout *colorHBox = new QHBoxLayout;
    colorHBox->addWidget(gradientBtoYPB);
    colorHBox->addWidget(gradientGtoRPB);
    colorGroupBox->setLayout(colorHBox);

    m_vLayout->addWidget(selectionGroupBox);
    m_vLayout->addWidget(new QLabel(QStringLiteral("Column range")));
    m_vLayout->addWidget(m_axisMinSliderX);
    m_vLayout->addWidget(m_axisMaxSliderX);
    m_vLayout->addWidget(new QLabel(QStringLiteral("Row range")));
    m_vLayout->addWidget(m_axisMinSliderZ);
    m_vLayout->addWidget(m_axisMaxSliderZ);
    m_vLayout->addWidget(colorGroupBox);
    m_vLayout->addStretch();

    connect(modeNoneRB, &QRadioButton::toggled,
                     this, &toggleModeNone);
    connect(modeItemRB,  &QRadioButton::toggled,
                     this, &toggleModeItem);
    connect(modeSliceRowRB,  &QRadioButton::toggled,
                     this, &toggleModeSliceRow);
    connect(modeSliceColumnRB,  &QRadioButton::toggled,
                     this, &toggleModeSliceColumn);
    connect(m_axisMinSliderX, &QSlider::valueChanged,
                     this, &adjustXMin);
    connect(m_axisMaxSliderX, &QSlider::valueChanged,
                     this, &adjustXMax);
    connect(m_axisMinSliderZ, &QSlider::valueChanged,
                     this, &adjustZMin);
    connect(m_axisMaxSliderZ, &QSlider::valueChanged,
                     this, &adjustZMax);
    connect(gradientBtoYPB, &QPushButton::pressed,
                     this, &setBlackToYellowGradient);
    connect(gradientGtoRPB, &QPushButton::pressed,
                     this, &setGreenToRedGradient);

    updateUI();
}

Widget::~Widget()
{
    delete m_graph;
}

QComboBox *Widget::createThemeBox() const
{
    QComboBox *themeComboBox = new QComboBox();
    themeComboBox->addItem("Light", QChart::ChartThemeLight);
    themeComboBox->addItem("Dark", QChart::ChartThemeDark);
    themeComboBox->addItem("Blue Cerulean", QChart::ChartThemeBlueCerulean);
    themeComboBox->addItem("Blue NCS", QChart::ChartThemeBlueNcs);
    themeComboBox->addItem("Blue Icy", QChart::ChartThemeBlueIcy);
    themeComboBox->addItem("Brown Sand", QChart::ChartThemeBrownSand);
    themeComboBox->addItem("Qt", QChart::ChartThemeQt);
    themeComboBox->addItem("High Contrast", QChart::ChartThemeHighContrast);
    return themeComboBox;
}

QChart *Widget::createSplineChart() const
{
    QChart *chart = new QChart();
    chart->setTitle("Spline chart");
    int nameIndex = 0;
    QMap<int,QString> rgb;
    rgb[0]=QString("B");
    rgb[1]=QString("G");
    rgb[2]=QString("R");
    for (const DataList &list : m_dataTable) {
        QSplineSeries *series = new QSplineSeries(chart);
        for (const Data &data : list)
            series->append(data);
        series->setName(rgb[nameIndex]);
        nameIndex++;
        chart->addSeries(series);
    }
    chart->createDefaultAxes();
    return chart;
}

void Widget::updateUI()
{
    QChart::ChartTheme theme = static_cast<QChart::ChartTheme>(
                m_themeComboBox->itemData(m_themeComboBox->currentIndex()).toInt());
    if(m_charts.empty())return;
    const auto charts = m_charts;
    if (m_charts.at(0)->chart()->theme() != theme) {
        for (QChartView *chartView : charts)
            chartView->chart()->setTheme(theme);

        QPalette pal = window()->palette();
        if (theme == QChart::ChartThemeLight) {
            pal.setColor(QPalette::Window, QRgb(0xf0f0f0));
            pal.setColor(QPalette::WindowText, QRgb(0x404044));
        } else if (theme == QChart::ChartThemeDark) {
            pal.setColor(QPalette::Window, QRgb(0x121218));
            pal.setColor(QPalette::WindowText, QRgb(0xd6d6d6));
        } else if (theme == QChart::ChartThemeBlueCerulean) {
            pal.setColor(QPalette::Window, QRgb(0x40434a));
            pal.setColor(QPalette::WindowText, QRgb(0xd6d6d6));
        } else if (theme == QChart::ChartThemeBrownSand) {
            pal.setColor(QPalette::Window, QRgb(0x9e8965));
            pal.setColor(QPalette::WindowText, QRgb(0x404044));
        } else if (theme == QChart::ChartThemeBlueNcs) {
            pal.setColor(QPalette::Window, QRgb(0x018bba));
            pal.setColor(QPalette::WindowText, QRgb(0x404044));
        } else if (theme == QChart::ChartThemeHighContrast) {
            pal.setColor(QPalette::Window, QRgb(0xffab03));
            pal.setColor(QPalette::WindowText, QRgb(0x181818));
        } else if (theme == QChart::ChartThemeBlueIcy) {
            pal.setColor(QPalette::Window, QRgb(0xcee7f0));
            pal.setColor(QPalette::WindowText, QRgb(0x404044));
        } else {
            pal.setColor(QPalette::Window, QRgb(0xf0f0f0));
            pal.setColor(QPalette::WindowText, QRgb(0x404044));
        }
        window()->setPalette(pal);
    }
}

void Widget::on_pushbutton_1_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
                           this, "Open File",".",
                           "Image files (*.bmp *.jpg );;All files (*.*)");

    if(fileName != ""){
        if(m_inputImage->load(fileName)){
            if(m_inputImage->format()!=QImage::Format_RGB32)
                *m_inputImage=m_inputImage->convertToFormat(QImage::Format_RGB32);
            if(m_container!=nullptr)m_container->hide();
            int B[256],G[256],R[256];
            std::memset(B,0,sizeof(B));
            std::memset(G,0,sizeof(G));
            std::memset(R,0,sizeof(R));
            unsigned char* inputImagePtr=m_inputImage->bits();
            int height=m_inputImage->height();
            int width=m_inputImage->width();
            for(int i=0;i<height;i++){
                for(int j=0;j<width;j++){
                    B[*inputImagePtr]++;
                    inputImagePtr++;
                    G[*inputImagePtr]++;
                    inputImagePtr++;
                    R[*inputImagePtr]++;
                    inputImagePtr+=2;
                }
            }

            if(m_chartView!=nullptr)m_chartView->hide();
            m_dataTable.clear();
            m_charts.clear();
            DataList dataList;
            dataList.clear();
            for(int i=0;i<256;i++){
                Data value(static_cast<qreal>(i),
                           static_cast<qreal>(static_cast<double>(B[i])/
                                              (height*width)));
                dataList<<value;
            }
            m_dataTable<<dataList;
            dataList.clear();
            for(int i=0;i<256;i++){
                Data value(static_cast<qreal>(i),
                           static_cast<qreal>(static_cast<double>(G[i])/
                                              (height*width)));
                dataList<<value;
            }
            m_dataTable<<dataList;
            dataList.clear();
            for(int i=0;i<256;i++){
                Data value(static_cast<qreal>(i),
                           static_cast<qreal>(static_cast<double>(R[i])/
                                              (height*width)));
                dataList<<value;
            }
            m_dataTable<<dataList;
            dataList.clear();

            QGraphicsScene *scene = new QGraphicsScene;
            scene->addPixmap(QPixmap::fromImage(*m_inputImage));
            m_graphicsView->setScene(scene);
            m_graphicsView->show();

            m_chartView=new QChartView(createSplineChart());
            m_hLayout->addWidget(m_chartView,1);
            m_charts<<m_chartView;

            updateUI();
        }
    }
}

void Widget::on_pushbutton_2_clicked()
{
    QSize colourImgSize = m_inputImage->size();
    m_grayImage = new QImage(colourImgSize,QImage::Format_Grayscale8);
    *m_grayImage=m_inputImage->convertToFormat(QImage::Format_Grayscale8);

    QVector<QRgb> grayColourTable;
    unsigned int rgb = 0;
    for(int i = 0; i < 256; i++){
       grayColourTable.append(rgb);
       rgb += 0x00010101;
    }

    m_grayImage->setColorTable(grayColourTable);

    QGraphicsScene *scene = new QGraphicsScene;
    scene->addPixmap(QPixmap::fromImage(*m_grayImage));
    m_graphicsView->setScene(scene);
    m_graphicsView->show();

    m_graph = new Q3DSurface();
    m_container = QWidget::createWindowContainer(m_graph);

    QSize screenSize = m_graph->screen()->size();
    m_container->setMinimumSize(QSize(screenSize.width() / 2, screenSize.height() / 1.6));
    m_container->setMaximumSize(screenSize);
    m_container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_container->setFocusPolicy(Qt::StrongFocus);

    m_chartView->hide();
    m_hLayout->removeWidget(m_chartView);
    m_charts.clear();
    m_hLayout->addWidget(m_container,1);

    m_graph->setAxisX(new QValue3DAxis);
    m_graph->setAxisY(new QValue3DAxis);
    m_graph->setAxisZ(new QValue3DAxis);
    QImage heightMapImage=m_grayImage->convertToFormat(QImage::Format_RGB32);
    heightMapImage=heightMapImage.scaled(800, 600, Qt::KeepAspectRatio, Qt::FastTransformation).
                                  scaled(260, 140, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_heightMapProxy = new QHeightMapSurfaceDataProxy(heightMapImage);
    m_heightMapSeries = new QSurface3DSeries(m_heightMapProxy);
    m_heightMapSeries->setItemLabelFormat(QStringLiteral("(@xLabel, @zLabel): @yLabel"));
    m_heightMapWidth = heightMapImage.width();
    m_heightMapHeight = heightMapImage.height();

    m_heightMapSeries->setDrawMode(QSurface3DSeries::DrawSurface);
    m_heightMapSeries->setFlatShadingEnabled(false);
    m_graph->axisX()->setRange(0.0f,10.0f);
    m_graph->axisY()->setAutoAdjustRange(true);
    m_graph->axisZ()->setRange(0.0f,10.0f);
    m_graph->addSeries(m_heightMapSeries);
    int mapGridCountX = m_heightMapWidth / heightMapGridStepX;
    int mapGridCountZ = m_heightMapHeight / heightMapGridStepZ;
    m_rangeMinX = 0.0f;
    m_rangeMinZ = 0.0f;
    m_stepX = 10.0f / float(mapGridCountX - 1);
    m_stepZ = 10.0f / float(mapGridCountZ - 1);

    m_axisMinSliderX->setMaximum(mapGridCountX - 2);
    m_axisMinSliderX->setValue(0);
    m_axisMaxSliderX->setMaximum(mapGridCountX - 1);
    m_axisMaxSliderX->setValue(mapGridCountX - 1);
    m_axisMinSliderZ->setMaximum(mapGridCountZ - 2);
    m_axisMinSliderZ->setValue(0);
    m_axisMaxSliderZ->setMaximum(mapGridCountZ - 1);
    m_axisMaxSliderZ->setValue(mapGridCountZ - 1);

    m_graph->activeTheme()->setType(Q3DTheme::Theme(2));

    update();
}

void Widget::on_pushbutton_3_clicked()
{
    QSize colourImgSize = m_inputImage->size();
    int rows=m_grayImage->height();
    int cols=m_grayImage->width();
    int T = 0;
    int hist[256];
    memset(hist,0,sizeof(hist));
    int S0, S1, S2;
    double lP, lP1;
    double avg1, avg2;
    double Dispersion1, Dispersion2, ClassInD, ClassOutD, Max;
    int graymax = 0, graymin = 255;
    unsigned char *grayImgDataPtr = m_grayImage->bits();
    for (int i = 0; i<rows; i++) {
        for (int j = 0; j<cols; j++) {
            int t_gray = *grayImgDataPtr;
            hist[t_gray]++;
            if (t_gray>graymax) graymax = t_gray;
            if (t_gray<graymin) graymin = t_gray;
            grayImgDataPtr++;
        }
    }
    lP = 0.0;
    S0 = 0;
    if (graymin == 0) graymin++;
    for (int k = graymin; k <= graymax; k++) {
        lP += static_cast<double>(k*hist[k]);
        S0 += hist[k];
    }
    Max = 0;
    S1 = 0;
    S2 = 0;
    lP1 = 0.0;
    ClassInD = 0.0;
    ClassOutD = 0.0;
    double ratio = 0.0;
    for (int k = graymin; k <= graymax; k++) {
        S1 += hist[k];
        if (!S1) { continue; }
        S2 = S0 - S1;
        if (S2 == 0) { break; }
        lP1 += static_cast<double>(k*hist[k]);
        Dispersion1 = 0.0;
        Dispersion2 = 0.0;
        avg1 = lP1 / S1;
        for (int n = graymin; n <= k; n++) {
            Dispersion1 += std::pow((n - avg1), 2)*hist[n];
        }
        avg2 = (lP - lP1) / S2;
        for (int m = k + 1; m <= graymax; m++) {
            Dispersion2 += std::pow((m - avg2), 2)*hist[m];
        }
        ClassInD = Dispersion1 + Dispersion2;
        ClassOutD = std::pow((avg1 - avg2)*S1*S2, 2);
        if (ClassInD != 0) ratio = ClassOutD / ClassInD;
        if (ratio >Max) {
            Max = ratio;
            T = k;
        }
    }

    QMessageBox::information(this, tr("阈值"),
                             tr("你选取的阈值是")+QString::number(T));

    m_binaryImage = new QImage(colourImgSize,QImage::Format_Grayscale8);

    unsigned char *binaryImgDataPtr=m_binaryImage->bits();
    grayImgDataPtr=m_grayImage->bits();
    for (int i=0;i<rows;i++){
        for (int j=0;j<cols;j++){
            if (*grayImgDataPtr<T) *binaryImgDataPtr = 0;
            else *binaryImgDataPtr = 255;
            binaryImgDataPtr++;
            grayImgDataPtr++;
        }
    }

    QVector<QRgb> grayColourTable;
    unsigned int rgb = 0;
    for(int i = 0; i < 256; i++){
       grayColourTable.append(rgb);
       rgb += 0x00010101;
    }

    m_binaryImage->setColorTable(grayColourTable);

    QGraphicsScene *scene = new QGraphicsScene;
    scene->addPixmap(QPixmap::fromImage(*m_binaryImage));
    m_graphicsView->setScene(scene);
    m_graphicsView->show();
    if(m_container!=nullptr)m_container->hide();

    updateUI();
}

void Widget::on_pushbutton_4_clicked()
{
    QString filename = QFileDialog::getSaveFileName(
                this, tr("Save File"), "./binarization.bmp", tr("Images (*.bmp)"));
    if(filename!=""){
        m_binaryImage->save(filename);
    }
}

void Widget::adjustXMin(int min)
{
    float minX = m_stepX * float(min) + m_rangeMinX;

    int max = m_axisMaxSliderX->value();
    if (min >= max) {
        max = min + 1;
        m_axisMaxSliderX->setValue(max);
    }
    float maxX = m_stepX * max + m_rangeMinX;

    m_graph->axisX()->setRange(minX,maxX);
}

void Widget::adjustXMax(int max)
{
    float maxX = m_stepX * float(max) + m_rangeMinX;

    int min = m_axisMinSliderX->value();
    if (max <= min) {
        min = max - 1;
        m_axisMinSliderX->setValue(min);
    }
    float minX = m_stepX * min + m_rangeMinX;

    m_graph->axisX()->setRange(minX,maxX);
}

void Widget::adjustZMin(int min)
{
    float minZ = m_stepZ * float(min) + m_rangeMinZ;

    int max = m_axisMaxSliderZ->value();
    if (min >= max) {
        max = min + 1;
        m_axisMaxSliderZ->setValue(max);
    }
    float maxZ = m_stepZ * max + m_rangeMinZ;

    m_graph->axisZ()->setRange(minZ,maxZ);
}

void Widget::adjustZMax(int max)
{
    float maxZ = m_stepZ * float(max) + m_rangeMinZ;

    int min = m_axisMinSliderZ->value();
    if (max <= min) {
        min = max - 1;
        m_axisMinSliderZ->setValue(min);
    }
    float minZ = m_stepZ * min + m_rangeMinZ;

    m_graph->axisZ()->setRange(minZ,maxZ);
}

void Widget::setBlackToYellowGradient()
{
    QLinearGradient gr;
    gr.setColorAt(0.0, Qt::black);
    gr.setColorAt(0.33, Qt::blue);
    gr.setColorAt(0.67, Qt::red);
    gr.setColorAt(1.0, Qt::yellow);

    m_graph->seriesList().at(0)->setBaseGradient(gr);
    m_graph->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
}

void Widget::setGreenToRedGradient()
{
    QLinearGradient gr;
    gr.setColorAt(0.0, Qt::darkGreen);
    gr.setColorAt(0.5, Qt::yellow);
    gr.setColorAt(0.8, Qt::red);
    gr.setColorAt(1.0, Qt::darkRed);

    m_graph->seriesList().at(0)->setBaseGradient(gr);
    m_graph->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
}

void Widget::on_pushbutton_5_clicked()
{
    QSize colourImgSize = m_inputImage->size();
    int rows=m_grayImage->height();
    int cols=m_grayImage->width();
    unsigned char* grayImgDataPtr=m_grayImage->bits();

    double dHistogram[256][256];
    double dTrMatrix = 0.0;
    int N = rows*cols;
    for (int i = 0; i < 256; i++){
        for (int j = 0; j < 256; j++)
            dHistogram[i][j] = 0.0;
    }

    for (int i = 0; i < rows; i++)  {
        for (int j = 0; j < cols; j++)  {
            unsigned char nData1 = grayImgDataPtr[i*rows+j];
            unsigned char nData2 = 0;
            int nData3 = 0;
            for (int m = i - 1; m <= i + 1; m++){
                for (int n = j - 1; n <= j + 1; n++){
                    if ((m >= 0) && (m < rows) && (n >= 0) && (n < cols))
                        nData3 += grayImgDataPtr[m*rows+n];
                }
            }
            nData2 = (unsigned char)(nData3 / 9);
            dHistogram[nData1][nData2]++;
        }
    }
    for (int i = 0; i < 256; i++)
        for (int j = 0; j < 256; j++)
            dHistogram[i][j] /= N;

    double Pti = 0.0;
    double Ptj = 0.0;
    double W0 = 0.0;
    double W1 = 0.0;
    double dData1 = 0.0;
    double dData2 = 0.0;
    int nThreshold_s = 0;
    int nThreshold_t = 0;
    int nThreshold;
    double temp = 0.0;
    for (int i = 0; i < 256; i++){
        for (int j = 0; j < 256; j++){
            Pti += i*dHistogram[i][j];
            Ptj += j*dHistogram[i][j];
        }
    }
    for (int i = 0; i < 256; i++){
        for (int j = 0; j < 256; j++){
            W0 += dHistogram[i][j];
            dData1 += i*dHistogram[i][j];
            dData2 += j*dHistogram[i][j];
            W1 = 1 - W0;

            dTrMatrix = ((W0 * Pti - dData1) * (W0 * Pti - dData1) + (W0 * Ptj - dData2) * (W0 * Ptj - dData2)) / (W0 * W1);
            if (dTrMatrix > temp){
                temp = dTrMatrix;
                nThreshold_s = i;
                nThreshold_t = j;
            }
        }
    }
    nThreshold = (nThreshold_s + nThreshold_t) / 2;


    QMessageBox::information(this, tr("阈值"),
                             tr("你选取的阈值是")+QString::number(nThreshold));

    m_binaryImage = new QImage(colourImgSize,QImage::Format_Grayscale8);

    unsigned char *binaryImgDataPtr=m_binaryImage->bits();
    grayImgDataPtr=m_grayImage->bits();
    for (int i=0;i<rows;i++){
        for (int j=0;j<cols;j++){
            if (*grayImgDataPtr<nThreshold) *binaryImgDataPtr = 0;
            else *binaryImgDataPtr = 255;
            binaryImgDataPtr++;
            grayImgDataPtr++;
        }
    }

    QVector<QRgb> grayColourTable;
    unsigned int rgb = 0;
    for(int i = 0; i < 256; i++){
       grayColourTable.append(rgb);
       rgb += 0x00010101;
    }

    m_binaryImage->setColorTable(grayColourTable);

    QGraphicsScene *scene = new QGraphicsScene;
    scene->addPixmap(QPixmap::fromImage(*m_binaryImage));
    m_graphicsView->setScene(scene);
    m_graphicsView->show();
    if(m_container!=nullptr)m_container->hide();

    updateUI();
}
