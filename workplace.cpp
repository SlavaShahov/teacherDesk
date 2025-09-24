#include "workplace.h"
#include "ui_workplace.h"

WorkPlace::WorkPlace(QWidget *parent)
    : QMainWindow(parent),
    centralWidget(new QWidget(this)),
    stackedWidget(new QStackedWidget(this)),
    addButton(new QPushButton("Добавить", this)),
    removeButton(new QPushButton("Удалить", this)),
    trainerButtonLayout(new QGridLayout()),
    trainerButtonContainer(new QWidget(this)),
    ui(new Ui::WorkPlace)
{
    ui->setupUi(this);
    setWindowTitle("Спортивный клуб СибГУТИ");

    QFile styleFile(":/styles/styles.css");
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        this->setStyleSheet(styleSheet);
    }

    // connectedDatabase();
    loadDataUser();

    addButton->setObjectName("functionButtonAdd");
    removeButton->setObjectName("functionButtonDel");

    QSettings settings("MyTeacher", "UserLogin");
    QString nowRole = settings.value("role").toString();

    setCentralWidget(centralWidget);

    // Основной макет
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Верхняя панель с кнопками
    QHBoxLayout *headerLayout = new QHBoxLayout();

    QLabel *ruleHeader = new QLabel(nowRole);
    ruleHeader->setObjectName("HeaderLabel");
    ruleHeader->setAlignment(Qt::AlignCenter);

    headerLayout->addWidget(addButton);
    headerLayout->addWidget(ruleHeader);
    headerLayout->addWidget(removeButton);

    // Добавление верхней панели и QStackedWidget в основной макет
    mainLayout->addLayout(headerLayout);
    mainLayout->addWidget(stackedWidget);

    // Инициализация контейнера для кнопок тренеров
    trainerButtonContainer->setLayout(trainerButtonLayout);
    stackedWidget->addWidget(trainerButtonContainer);

    // Подключение сигналов к слотам
    connect(addButton, &QPushButton::clicked, this, &WorkPlace::onAddTrainer);
    connect(removeButton, &QPushButton::clicked, this, &WorkPlace::onRemoveTrainer);


    loadDataCoach();
}

WorkPlace::~WorkPlace()
{
    delete ui;
}

void WorkPlace::onAddTrainer()
{
    // Создаем диалог
    QDialog dialog(this);
    dialog.setFixedSize(450, 240);
    dialog.setWindowTitle(tr("Добавить тренера"));

    // Создаем поля
    QLineEdit *fullNameEdit = new QLineEdit(&dialog);
    QLineEdit *usernameEdit = new QLineEdit(&dialog);
    QLineEdit *passwordEdit = new QLineEdit(&dialog);
    passwordEdit->setEchoMode(QLineEdit::Password);
    QDateEdit *birthdayEdit = new QDateEdit(&dialog);
    birthdayEdit->setCalendarPopup(true);
    birthdayEdit->setDisplayFormat("dd.MM.yyyy");
    birthdayEdit->setDate(QDate::currentDate());

    fullNameEdit->setFixedHeight(30);
    usernameEdit->setFixedHeight(30);
    passwordEdit->setFixedHeight(30);
    birthdayEdit->setFixedHeight(35);

    // Создаем форму
    QFormLayout *formLayout = new QFormLayout();
    formLayout->addRow(tr("ФИО:"), fullNameEdit);
    formLayout->addRow(tr("Имя пользователя:"), usernameEdit);
    formLayout->addRow(tr("Пароль:"), passwordEdit);
    formLayout->addRow(tr("Дата рождения:"), birthdayEdit);

    // Создаем кнопки
    QPushButton *okButton = new QPushButton(tr("Добавить"), &dialog);
    QPushButton *cancelButton = new QPushButton(tr("Отмена"), &dialog);

    okButton->setFixedSize(120, 35);
    cancelButton->setFixedSize(120, 35);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    // Обертка для центровки кнопок
    QVBoxLayout *centeredButtonLayout = new QVBoxLayout();
    centeredButtonLayout->addLayout(buttonLayout);
    centeredButtonLayout->setAlignment(buttonLayout, Qt::AlignCenter);

    // Создаем контейнер для кнопок
    QWidget *buttonContainer = new QWidget(&dialog);
    buttonContainer->setLayout(centeredButtonLayout);

    // Добавляем контейнер с кнопками в форму
    formLayout->setSpacing(12);
    formLayout->addRow(buttonContainer);

    // Подключение кнопок
    connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    // Устанавливаем форму в диалог
    dialog.setLayout(formLayout);

    // Показываем диалог
    if (dialog.exec() == QDialog::Accepted) {
        QString fullName = fullNameEdit->text().trimmed();
        QString username = usernameEdit->text().trimmed();
        QString password = passwordEdit->text();
        QDate birthday = birthdayEdit->date();

        // Проверка на пустые поля
        if (fullName.isEmpty() || username.isEmpty() || password.isEmpty()) {
            QMessageBox::warning(this, tr("Ошибка"), tr("Все поля должны быть заполнены!"));
            return;  // Возвращаемся, чтобы диалог не закрывался
        }

        // Сохранение данных в базу
        QSqlQuery query;
        query.prepare("INSERT INTO Users (full_name, username, password, birthday) "
                      "VALUES (:full_name, :username, :password, :birthday)");
        query.bindValue(":full_name", fullName);
        query.bindValue(":username", username);
        query.bindValue(":password", password); // Пароль хранить в открытом виде не рекомендуется
        query.bindValue(":birthday", birthday.toString("yyyy-MM-dd"));

        if (!query.exec()) {
            qDebug() << "Ошибка при добавлении пользователя:" << query.lastError().text();
            QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось добавить пользователя в базу данных. Проверьте уникальность имени пользователя."));
            return;
        }

        loadDataCoach();

        QMessageBox::information(this, tr("Успех"), tr("Тренер успешно добавлен!"));
    }
}

void WorkPlace::onRemoveTrainer()
{
    // Создаем диалог для ввода ФИО тренера
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Удалить тренера"));
    dialog.setFixedSize(300, 120);

    // Создаем поле для ввода ФИО
    QLineEdit *fullNameEdit = new QLineEdit(&dialog);
    fullNameEdit->setPlaceholderText(tr("Введите ФИО тренера"));
    fullNameEdit->setFixedHeight(35);

    // Создаем кнопки
    QPushButton *deleteButton = new QPushButton(tr("Удалить"), &dialog);
    QPushButton *cancelButton = new QPushButton(tr("Отмена"), &dialog);

    deleteButton->setFixedSize(120, 35);
    cancelButton->setFixedSize(120, 35);

    // Создаем горизонтальный макет для кнопок
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(cancelButton);

    // Создаем вертикальный макет для формы
    QVBoxLayout *formLayout = new QVBoxLayout();
    formLayout->addWidget(fullNameEdit);
    formLayout->setSpacing(10);
    formLayout->addLayout(buttonLayout);

    // Устанавливаем макет в диалог
    dialog.setLayout(formLayout);

    // Подключение кнопок
    connect(deleteButton, &QPushButton::clicked, this, [&]() {
        QString fullName = fullNameEdit->text().trimmed();

        if (fullName.isEmpty()) {
            QMessageBox::warning(this, tr("Ошибка"), tr("ФИО не может быть пустым!"));
            return;
        }

        // Выполняем SQL-запрос для удаления тренера
        QSqlQuery query;
        query.prepare("DELETE FROM Users WHERE full_name = :full_name AND role = 'Тренер'");
        query.bindValue(":full_name", fullName);

        if (!query.exec()) {
            qDebug() << "Ошибка при удалении тренера: " << query.lastError().text();
            QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось удалить тренера. Проверьте правильность введенного ФИО."));
            return;
        }

        loadDataCoach();

        dialog.accept();
        QMessageBox::information(this, tr("Успех"), tr("Тренер успешно удален!"));
    });

    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    // Показываем диалог
    dialog.exec();
}

void WorkPlace::connectedDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    QString dbPath = QCoreApplication::applicationDirPath() + "../../../database.db";
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        qDebug() << "Ошибка подключения к базе данных:" << db.lastError().text();
        return;
    }

    qDebug() << "База данных успешно подключена!";
}

void WorkPlace::loadDataUser() {
    QSettings settings("MyTeacher", "UserLogin");
    QString username = settings.value("username").toString();

    if (username.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Имя пользователя отсутствует в настройках.");
        return;
    }

    // Выполняем запрос к базе данных
    QSqlQuery query;
    query.prepare("SELECT id, username, full_name, role, birthday FROM Users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        QMessageBox::critical(this, "Ошибка базы данных", "Не удалось выполнить запрос: " + query.lastError().text());
        return;
    }

    if (!query.next()) {
        QMessageBox::warning(this, "Ошибка", "Пользователь с таким именем не найден.");
        return;
    }

    // Извлекаем данные пользователя
    int userId = query.value("id").toInt();
    QString fullName = query.value("full_name").toString();
    QString role = query.value("role").toString();
    QString birthday = query.value("birthday").toString();

    settings.setValue("id", userId);
    settings.setValue("username", username);
    settings.setValue("full_name", fullName);
    settings.setValue("role", role);
    settings.setValue("birthday", birthday);
}

void WorkPlace::loadDataCoach() {
    clearLayout(trainerButtonLayout);

    static int trainerCount = 1;
    trainerCount = 1;

    // Выполняем SQL-запрос для получения тренеров
    QSqlQuery query;
    query.prepare("SELECT id, full_name FROM Users WHERE role = 'Тренер'");

    if (!query.exec()) {
        qDebug() << "Ошибка при загрузке тренеров: " << query.lastError().text();
        return;
    }

    // Обрабатываем результат запроса
    while (query.next()) {
        int trainerId = query.value(0).toInt(); // id тренера
        QString fullName = query.value(1).toString(); // full_name тренера

        // Создаем кнопку для тренера
        QPushButton *trainerButton = new QPushButton(fullName, this);
        trainerButton->setObjectName("trainerButton");

        // Сохраняем id тренера в кнопке с использованием setProperty
        trainerButton->setProperty("trainerId", trainerId);

        // Добавляем кнопку в trainerButtonLayout
        int row = (trainerCount - 1) / 3; // 3 кнопки в строке
        int col = (trainerCount - 1) % 3;

        trainerButtonLayout->addWidget(trainerButton, row, col);

        // Увеличиваем счетчик тренеров
        trainerCount++;
    }
}

void WorkPlace::clearLayout(QLayout *layout) {
    if (!layout) return;
    while (QLayoutItem *item = layout->takeAt(0)) {
        // Если это виджет, то удаляем его
        if (QWidget *widget = item->widget()) {
            delete widget;
        }
        // Если это дочерний layout, рекурсивно очищаем его
        else if (QLayout *childLayout = item->layout()) {
            clearLayout(childLayout);
        }
        delete item;
    }
}
