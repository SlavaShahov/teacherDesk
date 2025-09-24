#ifndef COACHPAGE_H
#define COACHPAGE_H

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
#include <QTableWidget>
#include <QCalendarWidget>
#include <QCheckBox>
#include <QInputDialog>
#include <QHeaderView>

// Database headers
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace Ui {
class CoachPage;
}

class CoachPage : public QMainWindow
{
    Q_OBJECT

public:
    explicit CoachPage(QWidget *parent = nullptr);
    ~CoachPage();

private:
    Ui::CoachPage *ui;
    void populateAttendanceTable(QTableWidget *attendanceTable, int trainerId, int lessonId);
    void updateAttendanceRecord(int studentId, int lessonId, QDate date, bool wasPresent);
};

#endif // COACHPAGE_H
