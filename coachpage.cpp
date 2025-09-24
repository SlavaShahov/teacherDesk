#include "coachpage.h"
#include "ui_coachpage.h"

CoachPage::CoachPage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CoachPage)
{
    ui->setupUi(this);
    setWindowTitle("Спортивный клуб СибГУТИ");

    // Инициализируем QStackedWidget
    QStackedWidget *stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    // Создаем первую страницу (Грид-сетка)
    QWidget *firstPage = new QWidget(stackedWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(firstPage);

    // Создаем горизонтальный лэйаут для шапки и кнопки
    QHBoxLayout *headerLayout = new QHBoxLayout();

    // Создаем шапку с фамилией тренера
    QSettings settings("MyTeacher", "UserLogin");
    QString trainerName = settings.value("full_name", "Неизвестный тренер").toString();
    QLabel *headerLabel = new QLabel(trainerName, firstPage);
    headerLabel->setAlignment(Qt::AlignCenter);
    headerLabel->setStyleSheet("font-size: 20px; font-weight: bold; padding: 10px;");

    // Создаем кнопку "Выход"
    QPushButton *exitButton = new QPushButton("Выход", firstPage);
    exitButton->setStyleSheet("padding: 10px; background-color: #AA1803; color: white; border-radius: 5px;");

    // Добавляем элементы в горизонтальный лэйаут
    headerLayout->addStretch();
    headerLayout->addWidget(headerLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(exitButton);

    // Добавляем headerLayout в основной лэйаут с выравниванием по верху
    mainLayout->addLayout(headerLayout);
    mainLayout->setAlignment(headerLayout, Qt::AlignTop);

    // Создаем кнопки для спорта
    QGridLayout *gridLayout = new QGridLayout();
    QStringList sports = {"Теннис", "Баскетбол", "Волейбол", "Футбол"};
    QMap<QString, int> sportToLessonId = {
        {"Теннис", 1},
        {"Баскетбол", 2},
        {"Волейбол", 3},
        {"Футбол", 4}
    };
    QList<QPushButton *> sportButtons;

    for (int i = 0; i < sports.size(); ++i) {
        QPushButton *button = new QPushButton(sports[i], firstPage);
        button->setFixedSize(250, 250);
        button->setStyleSheet(
            "QPushButton {"
            "    padding: 15px;"
            "    font-size: 16px;"
            "    background-color: #4D8FC3;"
            "    color: white;"
            "    border-radius: 5px;"
            "    border: none;"
            "}"
            "QPushButton:hover {"
            "    background-color: #5DA9E5;"
            "}"
            "QPushButton:pressed {"
            "    background-color: #3A6D94;"
            "    color: #E8F1F8;"
            "}"
            );

        sportButtons.append(button);
        gridLayout->addWidget(button, 0, i);
    }

    // Центрируем грид-сетку
    QWidget *buttonsWidget = new QWidget(firstPage);
    QVBoxLayout *centeredLayout = new QVBoxLayout(buttonsWidget);
    centeredLayout->addLayout(gridLayout);
    centeredLayout->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(buttonsWidget, 1);

    // Добавляем первую страницу в QStackedWidget
    stackedWidget->addWidget(firstPage);

    // Подключаем слот для кнопки "Выход"
    connect(exitButton, &QPushButton::clicked, this, &QWidget::close);

    // Добавляем страницы для каждого спорта
    for (int i = 0; i < sports.size(); ++i) {
        QSettings settings("MyTeacher", "UserLogin");

        int lessonId = sportToLessonId[sports[i]];
        int trainerId = settings.value("id").toInt();

        QWidget *sportPage = new QWidget(stackedWidget);
        QVBoxLayout *sportLayout = new QVBoxLayout(sportPage);

        // Горизонтальный лэйаут для верхней части
        QHBoxLayout *topLayout = new QHBoxLayout();

        QPushButton *backButton = new QPushButton("Назад", sportPage);
        backButton->setStyleSheet(
            "padding: 10px; font-size: 16px; background-color: #778D45; color: white; border-radius: 5px;"
            );

        QLabel *sportLabel = new QLabel(sports[i], sportPage);
        sportLabel->setAlignment(Qt::AlignCenter);
        sportLabel->setStyleSheet("font-size: 24px; font-weight: bold;");

        QPushButton *addButton = new QPushButton("Добавить", sportPage);
        addButton->setStyleSheet(
            "padding: 10px; font-size: 16px; background-color: #4CAF50; color: white; border-radius: 5px;"
            );

        QPushButton *deleteButton = new QPushButton("Удалить", sportPage);
        deleteButton->setStyleSheet(
            "padding: 10px; font-size: 16px; background-color: #F44336; color: white; border-radius: 5px;"
            );

        topLayout->addWidget(backButton, 0, Qt::AlignLeft);
        topLayout->addWidget(sportLabel, 1);
        topLayout->addWidget(addButton, 0, Qt::AlignRight);
        topLayout->addWidget(deleteButton, 0, Qt::AlignRight);

        sportLayout->addLayout(topLayout);

        // Добавляем поле поиска над таблицей
        QLineEdit *searchEdit = new QLineEdit(sportPage);
        searchEdit->setFixedSize(250, 35);
        searchEdit->setPlaceholderText("Поиск по ФИО...");

        // Кнопка поиска
        QPushButton *searchButton = new QPushButton("Поиск", sportPage);
        searchButton->setStyleSheet(
            "padding: 10px; font-size: 16px; background-color: #FF9800; color: white; border-radius: 5px;"
            );

        // Создаем горизонтальный лэйаут для поля поиска и кнопки
        QHBoxLayout *searchLayout = new QHBoxLayout();
        searchLayout->addWidget(searchEdit);
        searchLayout->addWidget(searchButton);

        // Устанавливаем выравнивание по левому краю
        searchLayout->setAlignment(Qt::AlignLeft);

        // Устанавливаем фиксированные размеры для кнопки поиска
        searchButton->setFixedSize(100, 35);  // Размер кнопки поиска

        // Добавляем лэйаут в основное окно для спорта
        sportLayout->addLayout(searchLayout);

        // Создаем таблицу
        QTableWidget *attendanceTable = new QTableWidget(sportPage);
        attendanceTable->setColumnCount(32);
        attendanceTable->setRowCount(10);

        QStringList headers;
        headers << "ФИО";

        // Устанавливаем начальную дату
       // QDate startDate(2024, 12, 1);

        // Добавляем даты от 1 января 2024 года до 31 января 2024 года
       // for (int i = 0; i < 31; ++i) {
       //     headers << startDate.toString("dd.MM.yyyy");
       //     startDate = startDate.addDays(1);  // Переходим к следующему дню
        //}

        QDate currentDate = QDate::currentDate();
        QDate startDate = currentDate.addDays(-15); // За 15 дней до текущей даты
        QDate endDate = currentDate.addDays(15); // До 15 дней после текущей даты

        while (startDate <= endDate) {
            headers << startDate.toString("dd.MM.yyyy");
            startDate = startDate.addDays(1);
        }


        attendanceTable->setHorizontalHeaderLabels(headers);



        attendanceTable->setColumnWidth(0, 200);
        for (int col = 1; col < attendanceTable->columnCount(); ++col) {
            attendanceTable->setColumnWidth(col, 100);
        }

        // for (int row = 0; row < attendanceTable->rowCount(); ++row) {
        //     QTableWidgetItem *fioItem = new QTableWidgetItem();
        //     fioItem->setFlags(fioItem->flags() & ~Qt::ItemIsEditable);
        //     attendanceTable->setItem(row, 0, fioItem);

        //     for (int col = 1; col < attendanceTable->columnCount(); ++col) {
        //         QWidget *checkBoxWidget = new QWidget();
        //         QCheckBox *checkBox = new QCheckBox();
        //         QHBoxLayout *checkBoxLayout = new QHBoxLayout(checkBoxWidget);
        //         checkBoxLayout->addWidget(checkBox);
        //         checkBoxLayout->setAlignment(Qt::AlignCenter);
        //         checkBoxLayout->setContentsMargins(0, 0, 0, 0);
        //         attendanceTable->setCellWidget(row, col, checkBoxWidget);
        //     }
        // }

        attendanceTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        attendanceTable->setSelectionMode(QAbstractItemView::NoSelection);

        sportLayout->addWidget(attendanceTable);
        stackedWidget->addWidget(sportPage);

        // Подключаем кнопку поиска
        connect(searchButton, &QPushButton::clicked, [searchEdit, attendanceTable]() {
            QString searchTerm = searchEdit->text().trimmed();

            // Фильтруем таблицу по введенному поисковому запросу
            for (int row = 0; row < attendanceTable->rowCount(); ++row) {
                QTableWidgetItem *fioItem = attendanceTable->item(row, 0);  // Получаем элемент в первом столбце (ФИО)

                if (fioItem) {
                    QString fio = fioItem->text();
                    if (!fio.contains(searchTerm, Qt::CaseInsensitive)) {
                        attendanceTable->setRowHidden(row, true);  // Скрываем строку, если она не соответствует запросу
                    } else {
                        attendanceTable->setRowHidden(row, false);  // Показываем строку, если она соответствует запросу
                    }
                }
            }
        });

        connect(sportButtons[i], &QPushButton::clicked, [this, stackedWidget, sportPage, attendanceTable, trainerId, lessonId]() {
            populateAttendanceTable(attendanceTable, trainerId, lessonId);
            stackedWidget->setCurrentWidget(sportPage);
        });

        connect(backButton, &QPushButton::clicked, [stackedWidget, firstPage]() {
            stackedWidget->setCurrentWidget(firstPage);
        });

        connect(addButton, &QPushButton::clicked, [this, attendanceTable, trainerId, lessonId]() {
            // Создаем диалоговое окно для ввода данных
            QDialog *dialog = new QDialog();
            dialog->setWindowTitle("Добавить ученика");
            dialog->setFixedSize(420, 150);

            // Создаем элементы формы
            QFormLayout *formLayout = new QFormLayout();

            // ФИО
            QLineEdit *fioEdit = new QLineEdit(dialog);
            formLayout->addRow("ФИО:", fioEdit);

            // Дата рождения
            QLineEdit *birthDateEdit = new QLineEdit(dialog);
            formLayout->addRow("Дата рождения (ДД.ММ.ГГГГ):", birthDateEdit);

            // Номер телефона
            QLineEdit *phoneNumberEdit = new QLineEdit(dialog);
            formLayout->addRow("Номер на форме:", phoneNumberEdit);

            // Кнопки
            QPushButton *addButton = new QPushButton("Добавить", dialog);
            QPushButton *cancelButton = new QPushButton("Отмена", dialog);

            addButton->setFixedSize(150, 35);
            cancelButton->setFixedSize(150, 35);

            QHBoxLayout *buttonLayout = new QHBoxLayout();
            buttonLayout->addWidget(addButton);
            buttonLayout->addWidget(cancelButton);

            // Главное окно для диалога
            QVBoxLayout *mainLayout = new QVBoxLayout(dialog);
            mainLayout->addLayout(formLayout);
            mainLayout->addLayout(buttonLayout);

            // Соединяем кнопки с действиями
            QObject::connect(cancelButton, &QPushButton::clicked, dialog, &QDialog::reject);
            QObject::connect(addButton, &QPushButton::clicked, [this, dialog, fioEdit, birthDateEdit, phoneNumberEdit, trainerId, lessonId, attendanceTable]() {
                // Получаем введенные данные
                QString fio = fioEdit->text();
                QString birthDate = birthDateEdit->text();
                QString number = phoneNumberEdit->text();

                if (!fio.isEmpty() && !birthDate.isEmpty() && !number.isEmpty()) {
                    // Добавляем нового ученика в базу данных
                    QSqlQuery query;
                    query.prepare("INSERT INTO Students (full_name, birthdate, number, id_lessons, id_user) VALUES (:full_name, :birthdate, :number, :id_lessons, :id_user)");
                    query.bindValue(":full_name", fio);
                    query.bindValue(":birthdate", birthDate);
                    query.bindValue(":number", number);
                    query.bindValue(":id_lessons", lessonId);
                    query.bindValue(":id_user", trainerId);

                    if (!query.exec()) {
                        QMessageBox::critical(dialog, "Ошибка", "Не удалось добавить ученика: " + query.lastError().text());
                        return;
                    }

                    // Добавляем нового ученика в таблицу
                    int newRow = attendanceTable->rowCount();
                    attendanceTable->insertRow(newRow);

                    QTableWidgetItem *fioItem = new QTableWidgetItem(fio);
                    fioItem->setFlags(fioItem->flags() & ~Qt::ItemIsEditable);
                    attendanceTable->setItem(newRow, 0, fioItem);

                    for (int col = 1; col < attendanceTable->columnCount(); ++col) {
                        QWidget *checkBoxWidget = new QWidget();
                        QCheckBox *checkBox = new QCheckBox();
                        QHBoxLayout *checkBoxLayout = new QHBoxLayout(checkBoxWidget);
                        checkBoxLayout->addWidget(checkBox);
                        checkBoxLayout->setAlignment(Qt::AlignCenter);
                        checkBoxLayout->setContentsMargins(0, 0, 0, 0);
                        attendanceTable->setCellWidget(newRow, col, checkBoxWidget);
                    }

                    dialog->accept();
                } else {
                    QMessageBox::warning(dialog, "Ошибка", "Все поля должны быть заполнены.");
                }
            });

            dialog->exec();
        });

        connect(deleteButton, &QPushButton::clicked, [this, attendanceTable, trainerId, lessonId]() {
            QInputDialog dialog(nullptr);
            dialog.setWindowTitle("Удалить запись");
            dialog.setLabelText("Введите ФИО ученика для удаления:");
            dialog.resize(250, 200);

            bool ok = dialog.exec();
            QString fioToDelete = dialog.textValue();

            if (ok && !fioToDelete.isEmpty()) {
                QSqlQuery query;
                query.prepare("DELETE FROM Students WHERE full_name = :full_name AND id_user = :id_user AND id_lessons = :id_lessons");
                query.bindValue(":full_name", fioToDelete);
                query.bindValue(":id_user", trainerId);
                query.bindValue(":id_lessons", lessonId);

                if (!query.exec()) {
                    QMessageBox::critical(nullptr, "Ошибка", "Не удалось удалить ученика: " + query.lastError().text());
                    return;
                }

                bool found = false;
                for (int row = 0; row < attendanceTable->rowCount(); ++row) {
                    QTableWidgetItem *fioItem = attendanceTable->item(row, 0);
                    if (fioItem && fioItem->text() == fioToDelete) {
                        attendanceTable->removeRow(row);
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    QMessageBox::warning(nullptr, "Ошибка", "Запись с таким ФИО не найдена в таблице!");
                }
            }
        });
    }
}

CoachPage::~CoachPage()
{
    delete ui;
}

void CoachPage::populateAttendanceTable(QTableWidget *attendanceTable, int trainerId, int lessonId)
{
    qDebug() << "trainerId:" << trainerId << "lessonId:" << lessonId;

    QSqlQuery query;
    query.prepare("SELECT id, full_name FROM Students WHERE id_user = :trainerId AND id_lessons = :lessonId");
    query.bindValue(":trainerId", trainerId);
    query.bindValue(":lessonId", lessonId);

    if (!query.exec()) {
        QMessageBox::critical(this, "Ошибка запроса", "Не удалось выполнить запрос: " + query.lastError().text());
        return;
    }

    attendanceTable->setRowCount(0); // Очистка таблицы
    int row = 0;

    while (query.next()) {
        int studentId = query.value("id").toInt();
        QString fullName = query.value("full_name").toString();

        // Добавляем строку в таблицу
        attendanceTable->insertRow(row);

        // Устанавливаем ФИО
        QTableWidgetItem *fioItem = new QTableWidgetItem(fullName);
        fioItem->setFlags(fioItem->flags() & ~Qt::ItemIsEditable);
        attendanceTable->setItem(row, 0, fioItem);

        // Устанавливаем чекбоксы
        for (int col = 1; col < attendanceTable->columnCount(); ++col) {
            QWidget *checkBoxWidget = new QWidget();
            QCheckBox *checkBox = new QCheckBox();

            // Подключаем слот изменения состояния
            connect(checkBox, &QCheckBox::stateChanged, this, [this, studentId, lessonId, col, checkBox]() {
                QDate date = QDate(2024, 12, 1).addDays(col - 1); // Определяем дату на основе столбца
                bool wasPresent = (checkBox->checkState() == Qt::Checked);
                updateAttendanceRecord(studentId, lessonId, date, wasPresent);
            });

            QHBoxLayout *checkBoxLayout = new QHBoxLayout(checkBoxWidget);
            checkBoxLayout->addWidget(checkBox);
            checkBoxLayout->setAlignment(Qt::AlignCenter);
            checkBoxLayout->setContentsMargins(0, 0, 0, 0);
            attendanceTable->setCellWidget(row, col, checkBoxWidget);

            // Устанавливаем начальное состояние чекбокса из базы данных
            QSqlQuery attendanceQuery;
            attendanceQuery.prepare(
                "SELECT was_present FROM DateStore WHERE id_student = :studentId AND id_lessons = :lessonId AND date = :date"
                );
            attendanceQuery.bindValue(":studentId", studentId);
            attendanceQuery.bindValue(":lessonId", lessonId);
            attendanceQuery.bindValue(":date", QDate(2024, 12, 1).addDays(col - 1).toString("yyyy-MM-dd"));

            if (attendanceQuery.exec() && attendanceQuery.next()) {
                bool wasPresent = attendanceQuery.value("was_present").toBool();
                checkBox->setChecked(wasPresent);
            }
        }

        row++;
    }
}

void CoachPage::updateAttendanceRecord(int studentId, int lessonId, QDate date, bool wasPresent)
{
    qDebug() << studentId << lessonId << date << wasPresent;

    QSqlQuery query;
    query.prepare(
        "INSERT INTO DateStore (id_student, id_lessons, date, was_present) "
        "VALUES (:studentId, :lessonId, :date, :wasPresent) "
        "ON CONFLICT (id_student, id_lessons, date) "
        "DO UPDATE SET was_present = :wasPresent"
        );
    query.bindValue(":studentId", studentId);
    query.bindValue(":lessonId", lessonId);
    query.bindValue(":date", date.toString("yyyy-MM-dd"));
    query.bindValue(":wasPresent", wasPresent);

    if (!query.exec()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось обновить запись посещаемости: " + query.lastError().text());
    }
}
