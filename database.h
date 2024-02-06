#ifndef DATABASE_H
#define DATABASE_H

#include <QTableWidget>
#include <QSql>
#include <QSqlError>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QSqlQuery>

#define POSTGRE_DRIVER "QPSQL"
#define DB_NAME "MyDB"

//Количество полей данных необходимых для подключения к БД
#define NUM_DATA_FOR_CONNECT_TO_DB 5

//Перечисление полей данных
enum fieldsForConnect{
    hostName = 0,
    dbName = 1,
    login = 2,
    pass = 3,
    port = 4
};

//Типы запросов
enum requestType{

    requestAllFilms = 1,
    requestComedy   = 2,
    requestHorrors  = 3

};



class DataBase : public QObject
{
    Q_OBJECT

public:
    explicit DataBase(QObject *parent = nullptr);
    ~DataBase();

    void AddDataBase(QString driver, QString nameDB = "");
    void DisconnectFromDataBase(QString nameDb = "");
    void RequestToDB(int requestType);
    QSqlError GetLastError(void);
    void ConnectToDataBase(QVector<QString> dataForConnect);
    void ReadAnswerFromDB(QString request,int requestType);

signals:

   void sig_SendDataFromDB(QAbstractItemModel *QAbstractItemModel, int typeR);
   void sig_SendStatusConnection(bool);
   void sig_SendStatusRequest(QSqlError err, int requestType);


private:

    QSqlDatabase* dataBase;
    QSqlTableModel* tableModel;
    QSqlQueryModel* queryModel;
    QSqlQuery* simpleQuery;



};

#endif // DATABASE_H
