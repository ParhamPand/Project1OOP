#ifndef SINUSOIDALSOURCEDIALOG_H
#define SINUSOIDALSOURCEDIALOG_H

#include <QDialog>

namespace Ui {
    class SinusoidalSourceDialog;
}

class SinusoidalSourceDialog : public QDialog
{
Q_OBJECT

public:
    explicit SinusoidalSourceDialog(QWidget *parent = nullptr);
    ~SinusoidalSourceDialog();

    // Functions to set initial values
    void setOffset(double offset);
    void setAmplitude(double amplitude);
    void setFrequency(double frequency);
    void setPhase(double phase);

    // Functions to get new values
    double getOffset() const;
    double getAmplitude() const;
    double getFrequency() const;
    double getPhase() const;


private:
    Ui::SinusoidalSourceDialog *ui;
};

#endif // SINUSOIDALSOURCEDIALOG_H