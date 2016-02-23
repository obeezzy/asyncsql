#ifndef QUERYREQUEST_H
#define QUERYREQUEST_H

#include <QObject>
#include <QSqlRecord>
#include <QSqlIndex>
#include <QVariant>
#include "databaseconnection.h"

#include <functional>

namespace AsyncSql {
class QueryRequest
{
public:
    enum RequestType {None = -1, Select, Insert, Update, Delete,
                      BeginTransaction, CommitTransaction, Command,
                      CustomOperation};
    explicit QueryRequest(QObject *receiver = 0, const QString &query = "", const QString &tableName = "",
                          RequestType type = Select);

    void setReceiver(QObject *);
    QObject* getReceiver() const;

    void setTableName(const QString &);
    QString getTableName() const;

    void setQuery(const QString &);
    QString getQuery() const;

    void setRequestType(RequestType);
    RequestType getRequestType() const;

    void setRecords(const QList<QSqlRecord> &records);
    QList<QSqlRecord> getRecords() const;

    void setRecord(const QSqlRecord &record);

    void setPrimaryIndex(const QSqlIndex &);
    QSqlIndex getPrimaryIndex() const;

    void setSortColumn(int);
    int getSortColumn() const;

    void setRunBefore(const std::function<void(void)> &process);
    std::function<void(void)> getRunBefore() const;

    void setRunAfter(const std::function<void(void)> &process);
    std::function<void(void)> getRunAfter() const;

    void setCustomOperation(const std::function<void(void)> &process);
    std::function<void(void)> getCustomOperation() const;

    void setConnection(const DatabaseConnection &);
    DatabaseConnection getConnection() const;
private:
    QObject *receiver;
    QString tableName;
    QString query;
    RequestType type;
    QList<QSqlRecord> records;
    QSqlIndex primaryIndex;
    QVariant primaryValue;
    int sortColumn;
    DatabaseConnection connection;

    std::function<void (void)> runBefore;
    std::function<void (void)> customOperation;
    std::function<void (void)> runAfter;
};
}

#endif // QUERYREQUEST_H
