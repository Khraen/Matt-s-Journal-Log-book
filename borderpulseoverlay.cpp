#include "borderpulseoverlay.h"
#include <QPainter>
#include <QPainterPath>
#include <QEvent>
#include <QtMath>  // for qAtan2, qRadiansToDegrees

const int extraPadding = 10;
QPainterPath borderPath;
qreal borderPathLength = 0;
constexpr int pulseHalfWidth = 10; // half the width of your teardrop pulse shape
const int totalPadding = extraPadding + pulseHalfWidth;



BorderPulseOverlay::BorderPulseOverlay(QWidget *target, QWidget *parent)
    : QWidget(parent), targetWidget(target), progress(0.0)
{
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TranslucentBackground);
    setParent(target->parentWidget());

    // // Set size & position including padding
    // resize(target->width() + 2 * totalPadding, target->height() + 2 * totalPadding);
    // move(target->x() - totalPadding, target->y() - totalPadding);

    animation = new QPropertyAnimation(this, "progress");
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    //animation->setDuration(7000);
    animation->setLoopCount(-1);
    animation->setEasingCurve(QEasingCurve::Linear);

    targetWidget->installEventFilter(this);

    // updateBorderPath();


    // animation->start();
}
void BorderPulseOverlay::updateBorderPath() {
    QRectF rect = this->rect().adjusted(totalPadding, totalPadding, -totalPadding, -totalPadding);
    qreal radius = 12.0;  // match your widget's corner radius
    borderPath = QPainterPath();
    borderPath.addRoundedRect(rect, radius, radius);

    // Sample the path evenly
    const int segments = 1000;
    pathSamples.clear();
    qreal totalLength = 0;
    QPointF lastPoint = borderPath.pointAtPercent(0);
    pathSamples.append({0.0, 0.0, lastPoint, borderPath.angleAtPercent(0)});

    for (int i = 1; i <= segments; ++i) {
        qreal t = (qreal)i / segments;
        QPointF currentPoint = borderPath.pointAtPercent(t);
        qreal segmentLength = QLineF(lastPoint, currentPoint).length();
        totalLength += segmentLength;
        qreal angle = borderPath.angleAtPercent(t);
        pathSamples.append({t, totalLength, currentPoint, angle});
        lastPoint = currentPoint;
    }

    borderPathLength = totalLength;

    // Update animation duration based on path length and speed
    if (animation) {
        animation->setDuration(7000);  // e.g. 7 seconds for full loop on all window sizes

    }
}




qreal BorderPulseOverlay::getProgress() const {
    return progress;
}

bool BorderPulseOverlay::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == targetWidget && (event->type() == QEvent::Resize || event->type() == QEvent::Move)) {
        const int totalPadding = extraPadding + pulseHalfWidth;
        //const int extraPadding = 10;
        resize(targetWidget->width() + 2 * totalPadding, targetWidget->height() + 2 * totalPadding);
        move(targetWidget->x() - totalPadding, targetWidget->y() - totalPadding);

        // Recalculate path after resize/move
        updateBorderPath();

        // // Recalculate animation duration based on updated path length
        int newDurationMs = static_cast<int>((borderPathLength / cometSpeed) * 1000);
        animation->stop();
        animation->setDuration(newDurationMs);
        animation->start();

        update();
    }
    return QWidget::eventFilter(watched, event);
}



void BorderPulseOverlay::setProgress(qreal p) {
    progress = p;
    if (pathSamples.isEmpty())
        return;

    // Desired length along the path
    qreal desiredLength = p * borderPathLength;

    // Small offset length backward for smoothing (adjust as needed)
    qreal deltaLength = 5.0; // pixels behind current position

    // Calculate previous length with clamp
    qreal prevLength = qMax(0.0, desiredLength - deltaLength);

    // Helper lambda to find PathSample at a given length (binary search)
    auto findSampleAtLength = [&](qreal length) -> PathSample {
        int left = 0;
        int right = pathSamples.size() - 1;
        while (left < right) {
            int mid = (left + right) / 2;
            if (pathSamples[mid].length < length)
                left = mid + 1;
            else
                right = mid;
        }
        int idx1 = qMax(0, left - 1);
        int idx2 = qMin(pathSamples.size() - 1, left);

        const PathSample &s1 = pathSamples[idx1];
        const PathSample &s2 = pathSamples[idx2];
        qreal segLen = s2.length - s1.length;
        qreal segProg = (segLen > 0) ? (length - s1.length) / segLen : 0;

        QPointF pt = s1.point + (s2.point - s1.point) * segProg;
        qreal ang = interpolateAngle(s1.angle, s2.angle, segProg);
        return {0, length, pt, ang};
    };

    // Find current and previous samples
    PathSample currentSample = findSampleAtLength(desiredLength);
    PathSample prevSample = findSampleAtLength(prevLength);

    // Interpolate position and angle between previous and current for smooth rotation
    qreal segmentLength = currentSample.length - prevSample.length;
    qreal segmentProgress = (segmentLength > 0) ? (desiredLength - prevSample.length) / segmentLength : 0;

    QPointF smoothPoint = prevSample.point + (currentSample.point - prevSample.point) * segmentProgress;
    qreal smoothAngle = interpolateAngle(prevSample.angle, currentSample.angle, segmentProgress);

    trailPositions.append(qMakePair(smoothPoint, smoothAngle));
    while (trailPositions.size() > maxTrailLength) {
        trailPositions.removeFirst();
    }

    update();
}

void BorderPulseOverlay::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);

    resize(targetWidget->width() + 2 * totalPadding, targetWidget->height() + 2 * totalPadding);
    move(targetWidget->x() - totalPadding, targetWidget->y() - totalPadding);

    updateBorderPath();

    int newDurationMs = static_cast<int>((borderPathLength / cometSpeed) * 1000);
    animation->setDuration(newDurationMs);

    animation->start();
}



void BorderPulseOverlay::paintEvent(QPaintEvent *) {
    if (!targetWidget)
        return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const qreal baseScale = 1.5;

    int count = trailPositions.size();
    for (int i = 0; i < count; ++i) {
        QPointF point = trailPositions.at(i).first;

        qreal angle = 0;
        if (i > 0) {
            QPointF prevPoint = trailPositions.at(i - 1).first;
            QPointF delta = point - prevPoint;
            angle = std::atan2(delta.y(), delta.x()) * 180 / M_PI;  // radians to degrees
        } else {
            angle = trailPositions.at(i).second;  // fallback for first point
        }

        painter.save();
        painter.translate(point);
        painter.rotate(angle);

        qreal factor = (qreal)(i + 1) / count;
        int alpha = int(200 * factor);
        qreal scale = factor;

        QPainterPath teardrop;
        teardrop.moveTo(0, 0);
        teardrop.quadTo(10 * scale * baseScale, -5 * scale * baseScale, 20 * scale * baseScale, 0);
        teardrop.quadTo(10 * scale * baseScale, 5 * scale * baseScale, 0, 0);

        QRadialGradient gradient(QPointF(10 * scale * baseScale, 0), 20 * scale * baseScale);
        QColor inner = pulseColor;
        QColor mid = pulseColor;
        QColor outer = pulseColor;

        inner.setAlpha(alpha);
        mid.setAlpha(alpha / 3);
        outer.setAlpha(0);

        gradient.setColorAt(0, inner);
        gradient.setColorAt(0.6, mid);
        gradient.setColorAt(1, outer);


        painter.setBrush(gradient);
        painter.setPen(Qt::NoPen);
        painter.drawPath(teardrop);

        painter.restore();
    }
}

void BorderPulseOverlay::setPulseColor(const QColor &color){
    pulseColor = color;
    update();  // trigger repaint with new color
}






