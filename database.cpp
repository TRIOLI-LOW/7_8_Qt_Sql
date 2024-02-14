#include "database.h"

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{

    dataBase = new QSqlDatabase();
    simpleQuery = new QSqlQuery();


    queryModel = new QSqlQueryModel();

}

DataBase::~DataBase()
{
    delete dataBase;
}

/*!
 * \brief Метод добавляет БД к экземпляру класса QSqlDataBase
 * \param driver драйвер БД
 * \param nameDB имя БД (Если отсутствует Qt задает имя по умолчанию)
 */
void DataBase::AddDataBase(QString driver, QString nameDB)
{

    *dataBase = QSqlDatabase::addDatabase(driver, nameDB);

}

/*!
 * \brief Метод подключается к БД
 * \param для удобства передаем контейнер с данными необходимыми для подключения
 * \return возвращает тип ошибки
 */
void DataBase::ConnectToDataBase(QVector<QString> data)
{


    dataBase->setHostName(data[hostName]);
    dataBase->setDatabaseName(data[dbName]);
    dataBase->setUserName(data[login]);
    dataBase->setPassword(data[pass]);
    dataBase->setPort(data[port].toInt());


    bool status;
    status = dataBase->open();
    if (status) {
        qDebug() << "Database opened successfully.";
        tableModel = new QSqlTableModel(this, *dataBase);
        queryModel = new QSqlQueryModel();

    } else {
        qDebug() << "Failed to open database. Error: " << dataBase->lastError().text();
    }

    emit sig_SendStatusConnection(status);
}
/*!
 * \brief Метод производит отключение от БД
 * \param Имя БД
 */
void DataBase::DisconnectFromDataBase(QString nameDb)
{

    *dataBase = QSqlDatabase::database(nameDb);
    dataBase->close();

}
/*!
 * \brief Метод формирует запрос к БД.
 * \param request - SQL запрос
 * \return
 */
void DataBase::ReadAnswerFromDB(QString request,int requestType)
{
    *simpleQuery = QSqlQuery(request, *dataBase);
    QSqlError err;

    if(simpleQuery->exec(request) == false){
        err = simpleQuery->lastError();
        qDebug() << "22Error executing query: " << err.text();
    }
    emit sig_SendStatusRequest(err, requestType);

    ///Тут должен быть код ДЗ

}
void DataBase::RequestToDB( int requestType)
{
    switch(requestType){

    case requestAllFilms:{

        tableModel->setTable("film");
        tableModel->select();

        tableModel->setHeaderData(0, Qt::Horizontal, ("title"));
        tableModel->setHeaderData(1, Qt::Horizontal, ("description"));


        emit sig_SendDataFromDB(tableModel, requestAllFilms);
        break;
    }

    case requestHorrors:{

        queryModel->setQuery("SELECT title, description FROM film  "
                             "JOIN film_category fc on f.film_id = fc.film_id "
                             "JOIN category c on c.category_id = fc.category_id "
                             "WHERE c.name = 'Horror'");

        queryModel->setHeaderData(1,Qt::Horizontal,"title");
        queryModel->setHeaderData(2,Qt::Horizontal,"description");


        emit sig_SendDataFromDB(queryModel, requestHorrors);
        break;
    }

    case requestComedy:{

        queryModel->setQuery("SELECT title, description FROM film f "
                             "JOIN film_category fc on f.film_id = fc.film_id "
                             "JOIN category c on c.category_id = fc.category_id"
                             " WHERE c.name = 'Comedy'");

        queryModel->setHeaderData(0, Qt::Horizontal, "title");
        queryModel->setHeaderData(1, Qt::Horizontal, "description");


        emit sig_SendDataFromDB(queryModel, requestComedy);
        break;
    }

    break;
    }
}

/*!
 * @brief Метод возвращает последнюю ошибку БД
 */
QSqlError DataBase::GetLastError()
{
    return dataBase->lastError();
}
