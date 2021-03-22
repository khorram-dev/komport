#include <QLabel>
#include <QTimer>

class ScaleTextlabel : public QLabel
{
    Q_OBJECT
//    Q_DISABLE_COPY(ScaleTextlabel)

public:
    explicit ScaleTextlabel(QWidget* parent=Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags()) : QLabel(parent,f)
    {
       initTimer();
    }
    explicit ScaleTextlabel(const QString &text, QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags())
        :QLabel(text,parent,f)
    {
       initTimer();
    }

protected:
    void resizeEvent(QResizeEvent* event) override
    {
        QLabel::resizeEvent(event);
        m_resizeTimer->start();
    }

protected slots:
    void resizeFont()
    {
        QFont basefont = font();
        const QRect baseRect = contentsRect();
        const QString baseText = text();
        int fontSizeGuess = qMax(1,basefont.pixelSize());
        for(;;++fontSizeGuess){
            QFont testFont(basefont);
            testFont.setPixelSize(fontSizeGuess);
            const QRect fontRect=QFontMetrics(testFont).boundingRect(baseText);
            if(fontRect.height()>baseRect.height() || fontRect.width()>baseRect.width())
                break;
        }
        for(;fontSizeGuess>1;--fontSizeGuess){
            QFont testFont(basefont);
            testFont.setPixelSize(fontSizeGuess);
            const QRect fontRect=QFontMetrics(testFont).boundingRect(baseText);
            if(fontRect.height()<=baseRect.height() && fontRect.width()<=baseRect.width())
                break;
        }
        basefont.setPixelSize(fontSizeGuess);
        setFont(basefont);
    }

private:
    void initTimer()
    {
        m_resizeTimer=new QTimer(this);
        m_resizeTimer->setInterval(100);
        m_resizeTimer->setSingleShot(true);
        connect(m_resizeTimer,&QTimer::timeout,this,&ScaleTextlabel::resizeFont);
    }

    QTimer* m_resizeTimer;
};
