#ifndef QRADIOLIST_H
#define QRADIOLIST_H

#include <QWidget>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QScrollArea>

namespace QRL
{
    const int ItemHeight = 20;
}

class QRadioList : public QWidget
{
    Q_OBJECT
public:
    explicit QRadioList(QWidget *parent = 0);
    
public:

    void addItem(const QString& strItem);
    void clear();
    int count();
    void stop();

private:
    void initLayout();
    QRadioButton* createRadioByName(const QString& strText);
private:

    QWidget* _hostWidget;
    QVBoxLayout* _hostWidgetLayout;
    QVBoxLayout* _mainLayout;
    QScrollArea* _area;


    int _count;
    QVector<QRadioButton*> _buttons;

private Q_SLOTS:
    void slog_radioHandler();

Q_SIGNALS:
    void itemClicked(const QString& strText);
    void itemClicked(QRadioButton* radioButton);
};

#endif // QRADIOLIST_H
