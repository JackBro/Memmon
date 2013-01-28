#ifndef USAGEINFOPAD_H
#define USAGEINFOPAD_H

#include <QWidget>
#include <QVBoxLayout>

class UsageInfoPad : public QWidget
{
    Q_OBJECT
public:
    explicit UsageInfoPad(QWidget *parent = 0);
    
public:
    void addWidget(QWidget* w);

protected:
    void closeEvent(QCloseEvent *);

private:
    void setupLayout();
    void initSettings();

private:
    QVBoxLayout* _layout;

Q_SIGNALS:
    void sig_closed();
    
};

#endif // USAGEINFOPAD_H
