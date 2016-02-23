#ifndef ASYNCSQLTABLEMODEL_H
#define ASYNCSQLTABLEMODEL_H

#include <QAbstractTableModel>
#include <QtSql>
#include "queryrequest.h"
#include "queryresult.h"

namespace AsyncSql {
class AsyncSqlTableModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(int currentRow READ currentRow WRITE setCurrentRow NOTIFY currentRowChanged)
public:
    enum SelectType {Required, All};
    explicit AsyncSqlTableModel(QObject *parent = nullptr);
    virtual ~AsyncSqlTableModel();

    void beginTransaction();
    void commitTransaction();

    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    virtual bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int = Qt::EditRole);
    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    virtual QModelIndexList match(const QModelIndex & start, int role, const QVariant & value,
                                  int hits = 1,
                                  Qt::MatchFlags flags = Qt::MatchFlags( Qt::MatchStartsWith | Qt::MatchWrap )) const;

    int fieldIndex(const QString &) const;

    virtual void setFilter(const QString &);
    QString filter() const;
    virtual void setSort(int column, Qt::SortOrder);
    void setTable(const QString &);
    QString tableName() const;

    bool isDirty() const;
    bool isDirty(const QModelIndex &index) const;

    virtual QSqlRecord record() const;
    QSqlRecord record(int row) const;
    bool setRecord(int row, const QSqlRecord &);
    bool appendRecord(const QSqlRecord &);

    QList<QSqlRecord> getInsertedRecords() const;
    QMap<int, QSqlRecord> getUpdatedRecords() const;
    QList<QSqlRecord> getRemovedRecords() const;

    virtual void setLimit(int);
    int limit() const;

    QSqlError lastError() const;

    void setSelectType(SelectType);
    SelectType getSelectType() const;

    void setCurrentRow(int); // For use with QML
    int currentRow() const;

    // Methods that are intended for use with QML
    Q_INVOKABLE QVariant field(const QString &columnName) const;
    Q_INVOKABLE void customInsert(const QVariantMap &);
protected:
    void setRecords(const QList<QSqlRecord> &);
    void setOriginalRecords(const QList<QSqlRecord> &);
    QList<QSqlRecord> getRecords() const;
    QList<QSqlRecord> getOriginalRecords() const;

    void setSelectedSignalSuppressed(bool);
    bool isSelectedSignalSuppressed() const;
    virtual bool validateModel();
    QList<int> getInsertedRows() const;

    void setLastError(const QSqlError &);
    QSqlRecord getLastRecord() const;
signals:
    void execute(const QueryRequest &);
    void selected(bool successful);
    void submitted(bool successful);

    void currentRowChanged(int); // For use with QML
protected slots:
    virtual bool getResults(const QueryResult &);
public slots:
    virtual void select();
    virtual void submitAll();
    void revertAll();
private:
    QString filter_;
    QString tableName_;
    Qt::SortOrder order;
    int sortColumn;
    int limit_;
    QSqlIndex primaryIndex;
    QList<QSqlRecord> records;
    QList<QSqlRecord> originalRecords;
    QMap<int, QSqlRecord> updatedRecordMap, originalUpdatedRecordMap;
    QList<int> insertedRows, originalInsertedRows;
    QList<int> removedRows, originalRemovedRows;
    QSqlRecord emptyRecord;
    bool submitCalled;
    QSqlError error;
    QSqlRecord lastRecord;
    static int transactionState;

    bool selectedSignalSuppressed;
    SelectType selectType;

    int currentRow_; // For use with QML
};
}

#endif // ASYNCSQLTABLEMODEL_H

