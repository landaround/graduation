#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QtSql>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // З'єднання з базою даних SQLite
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("finances.db"); // Назва вашої бази даних SQLite

    if (!db.open()) {
        qDebug() << "Не вдалося відкрити базу даних";
        return;
    }

    // Приєднання кнопки до слоту
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::on_pushButton_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    // Отримання даних з lineEdit та перевірка їх на правильність
    QString input = ui->lineEdit_refill->text();
    QStringList parts = input.split(" ");

    if (parts.size() != 2) {
        QMessageBox::warning(this, "Помилка", "Невірний формат вводу. Введіть account_id та суму через пробіл.");
        return;
    }

    int account_id = parts[0].toInt();
    double refillAmount = parts[1].toDouble();

    if (account_id < 1 || account_id > 7) {
        QMessageBox::warning(this, "Помилка", "Недійсний номер рахунку. Введіть число від 1 до 7.");
        return;
    }

    // Виконання запиту на оновлення балансу рахунку
    QSqlQuery query;
    query.prepare("UPDATE accounts SET balance = balance + :refillAmount WHERE account_id = :account_id");
    query.bindValue(":refillAmount", refillAmount);
    query.bindValue(":account_id", account_id);

    if (!query.exec()) {
        qDebug() << "Помилка при оновленні балансу рахунку:" << query.lastError().text();
        return;
    }

    // Виведення результату
    if (query.numRowsAffected() > 0) {
        qDebug() << "Баланс рахунку " << account_id << " оновлено успішно";
    } else {
        qDebug() << "Не вдалося оновити баланс рахунку " << account_id;
    }

    // Отримання та виведення балансу рахунку після оновлення
    QSqlQuery balanceQuery;
    balanceQuery.prepare("SELECT balance FROM accounts WHERE account_id = :account_id");
    balanceQuery.bindValue(":account_id", account_id);

    if (!balanceQuery.exec()) {
        qDebug() << "Помилка при виборі балансу рахунку:" << balanceQuery.lastError().text();
        return;
    }

    if (balanceQuery.next()) {
        double balance = balanceQuery.value(0).toDouble();
        qDebug() << "Поточний баланс рахунку " << account_id << " становить " << balance;
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    // Отримання даних з lineEdit та перевірка їх на правильність
    void MainWindow::on_pushButton_2_clicked()
    {
        QString input = ui->lineEdit_draw->text();
        QStringList parts = input.split(" ");

        if (parts.size() != 2) {
            QMessageBox::warning(this, "Помилка", "Невірний формат вводу. Введіть account_id та суму через пробіл.");
            return;
        }

        int account_id = parts[0].toInt();
        double drawAmount = parts[1].toDouble();

        // Діалогове вікно для введення імені, прізвища та пароля
        QDialog dialog(this);
        QFormLayout form(&dialog);
        form.addRow(new QLabel("Введіть ім'я, прізвище та пароль:"));

        // Поля для введення даних
        QLineEdit *lineEditOwner = new QLineEdit(&dialog);
        QString labelOwner = QString("Ім'я та прізвище:");
        form.addRow(labelOwner, lineEditOwner);

        QLineEdit *lineEditPassword = new QLineEdit(&dialog);
        lineEditPassword->setEchoMode(QLineEdit::Password); // Заховати пароль
        QString labelPassword = QString("Пароль:");
        form.addRow(labelPassword, lineEditPassword);

        // Додавання кнопок Ок та Скасувати
        QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
        form.addRow(&buttonBox);
        QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
        QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

        // Показати діалогове вікно
        if (dialog.exec() == QDialog::Accepted) {
            QString owner = lineEditOwner->text();
            QString password = lineEditPassword->text();

            // Перевірка введених даних з базою даних
            QSqlQuery query;
            query.prepare("SELECT owner, password FROM bills WHERE account_id = :account_id");
            query.bindValue(":account_id", account_id);

            if (!query.exec()) {
                qDebug() << "Помилка при отриманні інформації про рахунок:" << query.lastError().text();
                return;
            }

            if (query.next()) {
                QString dbOwner = query.value(0).toString();
                QString dbPassword = query.value(1).toString();

                if (dbOwner == owner && dbPassword == password) {
                    // Якщо дані співпадають, виконуємо зняття коштів
                    QSqlQuery withdrawQuery;
                    withdrawQuery.prepare("UPDATE bills SET balance = balance - :drawAmount WHERE account_id = :account_id");
                    withdrawQuery.bindValue(":drawAmount", drawAmount);
                    withdrawQuery.bindValue(":account_id", account_id);

                    if (!withdrawQuery.exec()) {
                        qDebug() << "Помилка при знятті коштів з рахунку:" << withdrawQuery.lastError().text();
                        return;
                    }

                    qDebug() << "Кошти знято успішно";
                } else {
                    // Якщо дані не співпадають, виводимо повідомлення про помилку
                    QMessageBox::warning(this, "Помилка", "Невірне ім'я користувача або пароль.");
                    return;
                }
            }
        }
    }






void MainWindow::on_pushButton_3_clicked()
{
    QString currency = ui -> lineEdit_currency -> text();
    QString fund_type = ui -> lineEdit_fund -> text();
    QString institution = ui -> lineEdit_institution -> text();
    QString account_type = ui -> lineEdit_accounttype -> text();
    QString balance = ui -> lineEdit_balance_2 -> text();
    QString owner = ui -> lineEdit_owner -> text();
    QString password = ui -> lineEdit_password -> text();

    QSqlQuery query;
    query.prepare("INSERT INTO bills (currency, fund_type, institution, account_type, balance) VALUES (:currency, :fund_type, :institution, :account_type, :balance, :owner, :password)");
    query.bindValue(":currency", currency);
    query.bindValue(":fund_type", fund_type);
    query.bindValue(":institution", institution);
    query.bindValue(":account_type", account_type);
    query.bindValue(":balance", balance);
    query.bindValue(":owner", owner);
    query.bindValue(":password", password);

    if (!query.exec()) {
        qDebug() << "Не вдалося додати дані до таблиці:" << query.lastError().text();
        return 1;
    }
}
void MainWindow::generateDailyReport() {
    // Отримання поточної дати
    QDate currentDate = QDate::currentDate();

    // Форматування дати в рядок у відповідному форматі
    QString dateString = currentDate.toString("yyyy-MM-dd");

    // Виконання запиту для отримання витрат за поточний день
    QSqlQuery query;
    query.prepare("SELECT * FROM Expenses WHERE date = :date");
    query.bindValue(":date", dateString);

    if (!query.exec()) {
        qDebug() << "Помилка при виборі витрат за день:" << query.lastError().text();
        return;
    }

    // Формування звіту у відповідному форматі
    QString report;
    while (query.next()) {
        QString account_id = query.value("account_id").toString();
        QString amount = query.value("amount").toString();
        QString category = getCategory(query.value("category_id").toInt());

        report += "Рахунок: " + account_id + ", Сума: " + amount + ", Категорія: " + category + "\n";
    }

    // Збереження звіту у текстовий файл
    QString fileName = "daily_report_" + dateString + ".txt";
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << report;
        file.close();
        qDebug() << "Звіт за день збережено у файл:" << fileName;
    } else {
        qDebug() << "Не вдалося зберегти звіт за день у файл";
    }
}

void MainWindow::generateMonthlyReport() {
    // Отримання першого дня поточного місяця
    QDate currentDate = QDate::currentDate();
    QDate firstDayOfMonth = currentDate.addDays(-currentDate.day() + 1);

    // Форматування дати в рядок у відповідному форматі
    QString startDateString = firstDayOfMonth.toString("yyyy-MM-dd");
    QString endDateString = currentDate.toString("yyyy-MM-dd");

    // Виконання запиту для отримання витрат за поточний місяць
    QSqlQuery query;
    query.prepare("SELECT * FROM Expenses WHERE date BETWEEN :startDate AND :endDate");
    query.bindValue(":startDate", startDateString);
    query.bindValue(":endDate", endDateString);

    if (!query.exec()) {
        qDebug() << "Помилка при виборі витрат за місяць:" << query.lastError().text();
        return;
    }

    // Формування звіту у відповідному форматі
    QString report;
    while (query.next()) {
        QString account_id = query.value("account_id").toString();
        QString amount = query.value("amount").toString();
        QString category = getCategory(query.value("category_id").toInt());

        report += "Рахунок: " + account_id + ", Сума: " + amount + ", Категорія: " + category + "\n";
    }

    // Збереження звіту у текстовий файл
    QString fileName = "monthly_report_" + currentDate.toString("yyyy_MM") + ".txt";
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << report;
        file.close();
        qDebug() << "Звіт за місяць збережено у файл:" << fileName;
    } else {
        qDebug() << "Не вдалося зберегти звіт за місяць у файл";
    }
}

