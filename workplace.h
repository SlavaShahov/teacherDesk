#ifndef WORKPLACE_H
#define WORKPLACE_H

#include <QMainWindow>

#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QFormLayout>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QDate>
#include <QDateEdit>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QSettings>
#include <QDebug>
#include <QFile>

// Database headers
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace Ui {
class WorkPlace;
}

class WorkPlace : public QMainWindow
{
    Q_OBJECT

private slots:
    void onAddTrainer();
    void onRemoveTrainer();

public:
    explicit WorkPlace(QWidget *parent = nullptr);
    ~WorkPlace();

private:
    Ui::WorkPlace *ui;
    int id;
    QString username;
    QString full_name;
    QString birthday;
    QString role;

    void connectedDatabase();
    void loadDataUser();
    void loadDataCoach();

    QWidget *centralWidget;
    QStackedWidget *stackedWidget;
    QPushButton *addButton;
    QPushButton *removeButton;
    QGridLayout *trainerButtonLayout;
    QWidget *trainerButtonContainer;
    void clearLayout(QLayout *layout);

};

#endif // WORKPLACE_H
