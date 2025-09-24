#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Спортивный клуб СибГУТИ");

    connectedDatabase();

    workplace = new WorkPlace();
    coachpage = new CoachPage();

    // Основной виджет и макет
    QWidget *mainWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(mainWidget);

    // Левая часть: форма входа
    QWidget *loginWidget = new QWidget(mainWidget);
    QVBoxLayout *loginLayout = new QVBoxLayout(loginWidget);

    QLabel *loginLabel = new QLabel("Форма входа", loginWidget);
    QLineEdit *usernameInput = new QLineEdit(loginWidget);
    usernameInput->setPlaceholderText("Имя пользователя");
    QLineEdit *passwordInput = new QLineEdit(loginWidget);
    passwordInput->setPlaceholderText("Пароль");
    passwordInput->setEchoMode(QLineEdit::Password);
    QPushButton *loginButton = new QPushButton("Войти", loginWidget);

    usernameInput->setFixedSize(300, 45);
    passwordInput->setFixedSize(300, 45);
    loginButton->setFixedSize(150, 45);

    loginLayout->addWidget(loginLabel, 0, Qt::AlignHCenter); // Центрирование текста
    loginLayout->addSpacing(10);
    loginLayout->addWidget(usernameInput, 0, Qt::AlignHCenter);
    loginLayout->addWidget(passwordInput, 0, Qt::AlignHCenter);
    loginLayout->addSpacing(10);
    loginLayout->addWidget(loginButton, 0, Qt::AlignHCenter);
    loginLayout->addStretch(); // Для растяжения вниз

    // Центрирование формы входа
    mainLayout->addWidget(loginWidget, 1, Qt::AlignCenter);

    // Правая часть: слайдшоу
    QWidget *slideshowWidget = new QWidget(mainWidget);
    QVBoxLayout *slideshowLayout = new QVBoxLayout(slideshowWidget);

    QLabel *slideshowLabel = new QLabel(slideshowWidget);
    slideshowLabel->setFixedSize(550, 600); // Размер слайдера
    slideshowLabel->setAlignment(Qt::AlignCenter);

    slideshowLayout->addWidget(slideshowLabel);
    mainLayout->addWidget(slideshowWidget, 0, Qt::AlignRight);

    // Установка основного виджета
    setCentralWidget(mainWidget);

    // Настройка слайдшоу
    QStringList imagePaths = {
        ":/img/image1",
        ":/img/image2",
        ":/img/image3",
        ":/img/image4"
    };

    QTimer *timer = new QTimer(this);
    int duration = 5000; // Интервал между слайдами
    timer->start(duration);

    int *currentSlideIndex = new int(0);

    connect(timer, &QTimer::timeout, this, [=]() mutable {
        // Эффект для текущего слайда
        QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(slideshowLabel);
        slideshowLabel->setGraphicsEffect(opacityEffect);

        // Анимация исчезновения текущего изображения
        QPropertyAnimation *fadeOutAnimation = new QPropertyAnimation(opacityEffect, "opacity", this);
        fadeOutAnimation->setDuration(1000);
        fadeOutAnimation->setStartValue(1.0); // Полностью видимое изображение
        fadeOutAnimation->setEndValue(0.0);   // Полностью прозрачное изображение

        // Обработка окончания анимации исчезновения
        connect(fadeOutAnimation, &QPropertyAnimation::finished, this, [=]() mutable {
            *currentSlideIndex = (*currentSlideIndex + 1) % imagePaths.size();
            QPixmap nextImage(imagePaths[*currentSlideIndex]);

            // Устанавливаем следующее изображение
            slideshowLabel->setPixmap(nextImage);

            // Эффект для следующего слайда
            QGraphicsOpacityEffect *newOpacityEffect = new QGraphicsOpacityEffect(slideshowLabel);
            slideshowLabel->setGraphicsEffect(newOpacityEffect);

            // Анимация появления следующего изображения
            QPropertyAnimation *fadeInAnimation = new QPropertyAnimation(newOpacityEffect, "opacity", this);
            fadeInAnimation->setDuration(1000);
            fadeInAnimation->setStartValue(0.0); // Полностью прозрачное изображение
            fadeInAnimation->setEndValue(1.0);   // Полностью видимое изображение
            fadeInAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        });

        // Запуск анимации исчезновения
        fadeOutAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    });


    // Инициализация первого слайда
    if (!imagePaths.isEmpty()) {
        slideshowLabel->setPixmap(QPixmap(imagePaths.first()));
    }


    connect(loginButton, &QPushButton::clicked, this, [=](){
        QString username = usernameInput->text();
        QString password = passwordInput->text();

        if (username.isEmpty() || password.isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Поля имени пользователя и пароля не должны быть пустыми.");
            return;
        }

        QSqlQuery query;
        query.prepare("SELECT id, username, full_name, role, birthday FROM Users WHERE username = :username AND password = :password");
        query.bindValue(":username", username);
        query.bindValue(":password", password);

        if (!query.exec()) {
            QMessageBox::critical(this, "Ошибка базы данных", "Не удалось выполнить запрос: " + query.lastError().text());
            return;
        }

        // Проверка результата
        if (query.next()) {
            QString fullName = query.value("full_name").toString();
            int userId = query.value("id").toInt();
            QString role = query.value("role").toString();
            QString birthday = query.value("birthday").toString();

            // Сохраняем данные пользователя в QSettings
            QSettings settings("MyTeacher", "UserLogin");
            settings.setValue("id", userId);
            settings.setValue("username", username);
            settings.setValue("full_name", fullName);
            settings.setValue("role", role);
            settings.setValue("birthday", birthday);
            settings.setValue("Auth", true);
            QMessageBox::information(this, "Вход в систему", QString("Добро пожаловать, %1!").arg(fullName));

            if (role == "Администратор") {
                workplace->show();

            } else if (role == "Тренер") {
                coachpage->show();
            }

            this->close();

        } else {
            QMessageBox::warning(this, "Ошибка", "Неверное имя пользователя или пароль.");
        }

    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectedDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    QString dbPath = QCoreApplication::applicationDirPath() + "../../../database.db";
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        qDebug() << "Ошибка подключения к базе данных:" << db.lastError().text();
        return;
    }

    qDebug() << "База данных успешно подключена!";
}
