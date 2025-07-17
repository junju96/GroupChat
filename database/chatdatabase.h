// chatdatabase.h
#ifndef CHATDATABASE_H
#define CHATDATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QDebug>
#include <QStringList>

class ChatDatabase : public QObject
{
    Q_OBJECT
public:
    explicit ChatDatabase(QObject *parent = nullptr);
    ~ChatDatabase();

    // 初始化数据库
    bool initDatabase(const QString& dbPath = "chat_history.db");

    // 群组管理
    bool addGroup(const QString& groupName, const QStringList& members, const QString& myRole);
    bool deleteGroup(const QString& groupName);
    QStringList getAllGroups();
    QStringList getGroupMembers(const QString& groupName);
    QString getMyRole(const QString& groupName);

    // 消息管理
    bool saveMessage(const QString& groupName, const QString& sender,
                    const QString& message, const QDateTime& time);
    QList<QPair<QString, QPair<QString, QDateTime>>>
        getGroupMessages(const QString& groupName, int limit = 1000);
    bool cleanupOldMessages(const QString& groupName, int keepCount = 1000);

private:
    QSqlDatabase m_database;
};

#endif // CHATDATABASE_H
