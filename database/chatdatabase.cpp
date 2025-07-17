// chatdatabase.cpp
#include "chatdatabase.h"

ChatDatabase::ChatDatabase(QObject *parent) : QObject(parent)
{
}

ChatDatabase::~ChatDatabase()
{
    if (m_database.isOpen()) {
        m_database.close();
    }
}

bool ChatDatabase::initDatabase(const QString& dbPath)
{
    // 创建SQLite数据库连接
    m_database = QSqlDatabase::addDatabase("QSQLITE");
    m_database.setDatabaseName(dbPath);

    if (!m_database.open()) {
        qDebug() << "无法打开数据库:" << m_database.lastError().text();
        return false;
    }

    // 创建用户表（如果不存在）
    QSqlQuery query(m_database);
    bool success = query.exec(R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            role TEXT NOT NULL UNIQUE
        )
    )");

    if (!success) {
        qDebug() << "创建users表失败:" << query.lastError().text();
        return false;
    }

    // 预填充用户角色（如果不存在）
    QStringList defaultRoles = {"协同席", "一号操控席", "二号操控席", "三号操控席", "四号操控席"};
    for (const QString& role : defaultRoles) {
        query.prepare("INSERT OR IGNORE INTO users (role) VALUES (:role)");
        query.bindValue(":role", role);
        query.exec();
    }

    // 创建群组表（如果不存在）
    success = query.exec(R"(
        CREATE TABLE IF NOT EXISTS groups (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL UNIQUE,
            created_at DATETIME NOT NULL
        )
    )");

    if (!success) {
        qDebug() << "创建groups表失败:" << query.lastError().text();
        return false;
    }

    // 创建群组成员关联表（如果不存在）
    success = query.exec(R"(
        CREATE TABLE IF NOT EXISTS group_members (
            group_id INTEGER NOT NULL,
            user_id INTEGER NOT NULL,
            is_me INTEGER NOT NULL DEFAULT 0,
            PRIMARY KEY (group_id, user_id),
            FOREIGN KEY (group_id) REFERENCES groups(id),
            FOREIGN KEY (user_id) REFERENCES users(id)
        )
    )");

    if (!success) {
        qDebug() << "创建group_members表失败:" << query.lastError().text();
        return false;
    }

    // 创建消息表（如果不存在）
    success = query.exec(R"(
        CREATE TABLE IF NOT EXISTS messages (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            group_id INTEGER NOT NULL,
            sender TEXT NOT NULL,
            message TEXT NOT NULL,
            timestamp DATETIME NOT NULL,
            FOREIGN KEY (group_id) REFERENCES groups(id)
        )
    )");

    if (!success) {
        qDebug() << "创建messages表失败:" << query.lastError().text();
        return false;
    }

    // 创建索引
    success = query.exec("CREATE INDEX IF NOT EXISTS group_index ON messages (group_id)");
    if (!success) {
        qDebug() << "创建group_index索引失败:" << query.lastError().text();
    }

    success = query.exec("CREATE INDEX IF NOT EXISTS time_index ON messages (timestamp)");
    if (!success) {
        qDebug() << "创建time_index索引失败:" << query.lastError().text();
    }

    return true;
}

bool ChatDatabase::addGroup(const QString& groupName, const QStringList& members, const QString& myRole)
{
    if (!m_database.isOpen() || groupName.isEmpty() || members.isEmpty()) {
        return false;
    }

    // 开始事务
    m_database.transaction();

    // 添加群组
    QSqlQuery query(m_database);
    query.prepare("INSERT INTO groups (name, created_at) VALUES (:name, :created_at)");
    query.bindValue(":name", groupName);
    query.bindValue(":created_at", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    if (!query.exec()) {
        m_database.rollback();
        return false;
    }

    // 获取群组ID
    int groupId = query.lastInsertId().toInt();

    // 添加群组成员
    for (const QString& member : members) {
        // 获取用户ID
        query.prepare("SELECT id FROM users WHERE role = :role");
        query.bindValue(":role", member);

        if (!query.exec() || !query.next()) {
            m_database.rollback();
            return false;
        }

        int userId = query.value(0).toInt();

        // 判断是否是当前用户
        int isMe = (member == myRole) ? 1 : 0;

        // 添加到群组成员表
        query.prepare("INSERT INTO group_members (group_id, user_id, is_me) VALUES (:group_id, :user_id, :is_me)");
        query.bindValue(":group_id", groupId);
        query.bindValue(":user_id", userId);
        query.bindValue(":is_me", isMe);

        if (!query.exec()) {
            m_database.rollback();
            return false;
        }
    }

    // 提交事务
    m_database.commit();
    return true;
}

bool ChatDatabase::deleteGroup(const QString& groupName)
{
    if (!m_database.isOpen() || groupName.isEmpty()) {
        return false;
    }

    // 获取群组ID
    QSqlQuery query(m_database);
    query.prepare("SELECT id FROM groups WHERE name = :name");
    query.bindValue(":name", groupName);

    if (!query.exec() || !query.next()) {
        return false;
    }

    int groupId = query.value(0).toInt();

    // 开始事务
    m_database.transaction();

    // 删除群组成员
    query.prepare("DELETE FROM group_members WHERE group_id = :group_id");
    query.bindValue(":group_id", groupId);

    if (!query.exec()) {
        m_database.rollback();
        return false;
    }

    // 删除消息
    query.prepare("DELETE FROM messages WHERE group_id = :group_id");
    query.bindValue(":group_id", groupId);

    if (!query.exec()) {
        m_database.rollback();
        return false;
    }

    // 删除群组
    query.prepare("DELETE FROM groups WHERE id = :id");
    query.bindValue(":id", groupId);

    bool success = query.exec();

    // 提交事务
    if (success) {
        m_database.commit();
    } else {
        m_database.rollback();
    }

    return success;
}

QStringList ChatDatabase::getAllGroups()
{
    QStringList groups;

    if (!m_database.isOpen()) {
        return groups;
    }

    QSqlQuery query(m_database);
    if (query.exec("SELECT name FROM groups ORDER BY created_at ASC")) {
        while (query.next()) {
            groups.append(query.value(0).toString());
        }
    }

    return groups;
}

QStringList ChatDatabase::getGroupMembers(const QString& groupName)
{
    QStringList members;

    if (!m_database.isOpen() || groupName.isEmpty()) {
        return members;
    }

    // 获取群组ID
    QSqlQuery query(m_database);
    query.prepare("SELECT id FROM groups WHERE name = :name");
    query.bindValue(":name", groupName);

    if (!query.exec() || !query.next()) {
        return members;
    }

    int groupId = query.value(0).toInt();

    // 查询群组成员
    query.prepare("SELECT u.role FROM group_members gm "
                  "JOIN users u ON gm.user_id = u.id "
                  "WHERE gm.group_id = :group_id");
    query.bindValue(":group_id", groupId);

    if (query.exec()) {
        while (query.next()) {
            members.append(query.value(0).toString());
        }
    }

    return members;
}

QString ChatDatabase::getMyRole(const QString& groupName)
{
    if (!m_database.isOpen() || groupName.isEmpty()) {
        return "";
    }

    // 获取群组ID
    QSqlQuery query(m_database);
    query.prepare("SELECT id FROM groups WHERE name = :name");
    query.bindValue(":name", groupName);

    if (!query.exec() || !query.next()) {
        return "";
    }

    int groupId = query.value(0).toInt();

    // 查询我的角色
    query.prepare("SELECT u.role FROM group_members gm "
                  "JOIN users u ON gm.user_id = u.id "
                  "WHERE gm.group_id = :group_id AND gm.is_me = 1");
    query.bindValue(":group_id", groupId);

    if (query.exec() && query.next()) {
        return query.value(0).toString();
    }

    return "";
}

bool ChatDatabase::saveMessage(const QString& groupName, const QString& sender,
                              const QString& message, const QDateTime& time)
{
    if (!m_database.isOpen() || groupName.isEmpty()) {
        return false;
    }

    // 获取群组ID
    QSqlQuery query(m_database);
    query.prepare("SELECT id FROM groups WHERE name = :name");
    query.bindValue(":name", groupName);

    if (!query.exec() || !query.next()) {
        return false;
    }

    int groupId = query.value(0).toInt();

    // 保存消息
    query.prepare("INSERT INTO messages (group_id, sender, message, timestamp) "
                  "VALUES (:group_id, :sender, :message, :timestamp)");
    query.bindValue(":group_id", groupId);
    query.bindValue(":sender", sender);
    query.bindValue(":message", message);
    query.bindValue(":timestamp", time.toString("yyyy-MM-dd hh:mm:ss"));

    if (!query.exec()) {
        return false;
    }

    // 清理旧消息
    return cleanupOldMessages(groupName);
}

QList<QPair<QString, QPair<QString, QDateTime>>>
ChatDatabase::getGroupMessages(const QString& groupName, int limit)
{
    QList<QPair<QString, QPair<QString, QDateTime>>> messages;

    if (!m_database.isOpen() || groupName.isEmpty()) {
        return messages;
    }

    // 获取群组ID
    QSqlQuery query(m_database);
    query.prepare("SELECT id FROM groups WHERE name = :name");
    query.bindValue(":name", groupName);

    if (!query.exec() || !query.next()) {
        return messages;
    }

    int groupId = query.value(0).toInt();

    // 查询消息
    query.prepare("SELECT sender, message, timestamp FROM messages "
                  "WHERE group_id = :group_id "
                  "ORDER BY timestamp ASC "
                  "LIMIT :limit");
    query.bindValue(":group_id", groupId);
    query.bindValue(":limit", limit);

    if (query.exec()) {
        while (query.next()) {
            QString sender = query.value(0).toString();
            QString message = query.value(1).toString();
            QDateTime time = QDateTime::fromString(query.value(2).toString(), "yyyy-MM-dd hh:mm:ss");

            messages.append(qMakePair(message, qMakePair(sender, time)));
        }
    }

    return messages;
}

bool ChatDatabase::cleanupOldMessages(const QString& groupName, int keepCount)
{
    if (!m_database.isOpen() || groupName.isEmpty()) {
        return false;
    }

    // 获取群组ID
    QSqlQuery query(m_database);
    query.prepare("SELECT id FROM groups WHERE name = :name");
    query.bindValue(":name", groupName);

    if (!query.exec() || !query.next()) {
        return false;
    }

    int groupId = query.value(0).toInt();

    // 获取需要保留的最新消息的ID
    query.prepare("SELECT id FROM messages "
                  "WHERE group_id = :group_id "
                  "ORDER BY timestamp DESC "
                  "LIMIT :keep_count");
    query.bindValue(":group_id", groupId);
    query.bindValue(":keep_count", keepCount);

    if (!query.exec()) {
        return false;
    }

    QList<int> idsToKeep;
    while (query.next()) {
        idsToKeep.append(query.value(0).toInt());
    }

    // 如果消息数量小于等于保留数量，则不需要删除
    if (idsToKeep.size() >= keepCount) {
        return true;
    }

    // 构建删除查询
    if (!idsToKeep.isEmpty()) {
        QString placeholders = QString("NOT IN (%1)").arg(
            QString("?, ").repeated(idsToKeep.size()).chopped(2));

        query.prepare(QString("DELETE FROM messages "
                              "WHERE group_id = :group_id "
                              "AND id %1").arg(placeholders));
        query.bindValue(0, groupId);

        for (int i = 0; i < idsToKeep.size(); ++i) {
            query.bindValue(i + 1, idsToKeep[i]);
        }

        if (!query.exec()) {
            return false;
        }
    }

    return true;
}
