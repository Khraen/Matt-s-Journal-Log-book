#ifndef BORDERPULSEOVERLAY_H
#define BORDERPULSEOVERLAY_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QVector>
#include <QPointF>

class BorderPulseOverlay : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal progress READ getProgress WRITE setProgress)

public:
    explicit BorderPulseOverlay(QWidget *target, QWidget *parent = nullptr);

    qreal getProgress() const;
    void setProgress(qreal p);
    void updateBorderPath();
    void setPulseColor(const QColor &color);

protected:
    void paintEvent(QPaintEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;
    //helper
    static qreal interpolateAngle(qreal angle1, qreal angle2, qreal t) {
        qreal delta = angle2 - angle1;

        while (delta > 180) delta -= 360;
        while (delta < -180) delta += 360;

        return angle1 + delta * t;
    }
    void showEvent(QShowEvent* event) override;


private:
    QWidget *targetWidget;
    QPropertyAnimation *animation;
    qreal progress;
    QList<QPair<QPointF, qreal>> trailPositions;
    int maxTrailLength = 15;           // number of trail points
    static constexpr qreal cometSpeed = 100.0; // pixels per second, adjust to your liking
    struct PathSample {
        qreal percent;
        qreal length;
        QPointF point;
        qreal angle;
    };
    QColor pulseColor = QColor(0, 234, 255); // default blue

    QVector<PathSample> pathSamples;


};

#endif // BORDERPULSEOVERLAY_H
