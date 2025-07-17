#ifndef GROUPCHATCONFIG_H
#define GROUPCHATCONFIG_H

#include <QString>
#include <QVector>
#include <cstring>
#include <qglobal.h>
#ifndef MessageIDType
#define MessageIDType qulonglong
/*通信层数据*/
struct Comm_SendMessage{ // 12 数据下发
    quint32     size; /*消息数据长度*/  // 0，4
    qulonglong  data; /*指向消息数据（字符数组）的指针*/ // 4，8
    quint32     dstid;/*目的端对象的编码，预留（将来根据此值判断用何种协议格式）*/  // 12，4
    quint32     dstip;/*目的地址*/  // 16，4
    quint16     dstport;/*目的端口*/
    Comm_SendMessage() {memset(this, 0, sizeof(Comm_SendMessage));}
};
struct Comm_ReceiveMessage{   // 20 数据接收
    quint32     size; /*消息数据长度*/ // 0，4
    qulonglong  data; /*指向消息数据（字符数组）的指针*/  // 4，8
    quint32     srcid;/*源端对象的编码，预留（将来根据此值判断用何种协议格式）*/  // 12，4
    quint32     srcip;/*源地址*/  // 16，4
    Comm_ReceiveMessage() {memset(this, 0, sizeof(Comm_ReceiveMessage));}
};
struct Comm_ReplyMessage{  // 消息回复
    quint32     size; /*消息数据长度*/
    qulonglong  data; /*指向消息数据（字符数组）的指针*/
    quint32     srcid;/*源端对象的编码，预留（将来根据此值判断用何种协议格式）*/
    quint32     srcip;/*源地址*/
    quint16     msg_type;/*消息类型*/
    quint16     ask_code;/*应答码*/
    Comm_ReplyMessage() {memset(this, 0, sizeof(Comm_ReplyMessage));}
};
/*
数据发送使用方式：
            Comm_SendMessage send;
            send.data = (qulonglong)payload_data.data();
            send.size = payload_data.size();
            if(!PostMessage(MSGDATA_ACCOUNT_SYNC_RE,&send,sizeof(Comm_SendMessage))){
                qDebug()<<"false post MSGDATA_ACCOUNT_SYNC_RE";
            }
数据接收使用方式：
    if(type == MSGDATA_SOA_REQUEST_DATA&&length == sizeof(Comm_ReceiveMessage)){
        Comm_ReceiveMessage* rcv_msg = (Comm_ReceiveMessage*) msg;
        QByteArray data;
        data.append((char *)rcv_msg->data, rcv_msg->size);
*/
// 席位群聊发送
const MessageIDType MSGDATA_MULTICAST_GROUP_CHAT_SEND = 0x030061A23AAA1000;
const MessageIDType MSGDATA_MULTICAST_GROUP_CHAT_SEND_FIELD1 = MSGDATA_MULTICAST_GROUP_CHAT_SEND+1;
const MessageIDType MSGDATA_MULTICAST_GROUP_CHAT_SEND_FIELD2 = MSGDATA_MULTICAST_GROUP_CHAT_SEND+2;
const MessageIDType MSGDATA_MULTICAST_GROUP_CHAT_SEND_FIELD3 = MSGDATA_MULTICAST_GROUP_CHAT_SEND+3;
const MessageIDType MSGDATA_MULTICAST_GROUP_CHAT_SEND_FIELD4 = MSGDATA_MULTICAST_GROUP_CHAT_SEND+4;
const MessageIDType MSGDATA_MULTICAST_GROUP_CHAT_SEND_FIELD5 = MSGDATA_MULTICAST_GROUP_CHAT_SEND+5;
// 席位群聊接收
const MessageIDType MSGDATA_MULTICAST_GROUP_CHAT_RECEIVE = 0x030061A23AAA0000;
const MessageIDType MSGDATA_MULTICAST_GROUP_CHAT_RECEIVE_FIELD1 = MSGDATA_MULTICAST_GROUP_CHAT_RECEIVE+1;
const MessageIDType MSGDATA_MULTICAST_GROUP_CHAT_RECEIVE_FIELD2 = MSGDATA_MULTICAST_GROUP_CHAT_RECEIVE+2;
const MessageIDType MSGDATA_MULTICAST_GROUP_CHAT_RECEIVE_FIELD3 = MSGDATA_MULTICAST_GROUP_CHAT_RECEIVE+3;
const MessageIDType MSGDATA_MULTICAST_GROUP_CHAT_RECEIVE_FIELD4 = MSGDATA_MULTICAST_GROUP_CHAT_RECEIVE+4;

// 群组构建/调整接收
const MessageIDType MSGDATA_SOA_GROUP_CONSTRUCTING_DATA = 0x030061A35FF50000;
const MessageIDType MSGDATA_SOA_GROUP_CONSTRUCTING_DATA_FIELD1 = MSGDATA_SOA_GROUP_CONSTRUCTING_DATA + 1;
const MessageIDType MSGDATA_SOA_GROUP_CONSTRUCTING_DATA_FIELD2 = MSGDATA_SOA_GROUP_CONSTRUCTING_DATA + 2;
const MessageIDType MSGDATA_SOA_GROUP_CONSTRUCTING_DATA_FIELD3 = MSGDATA_SOA_GROUP_CONSTRUCTING_DATA + 3;
const MessageIDType MSGDATA_SOA_GROUP_CONSTRUCTING_DATA_FIELD4 = MSGDATA_SOA_GROUP_CONSTRUCTING_DATA + 4;

// 群组构建/调整反馈
const MessageIDType MSGDATA_SOA_GROUP_CONSTRUCTING_FEEDBACK_DATA = 0x030061A35FF51000;
const MessageIDType MSGDATA_SOA_GROUP_CONSTRUCTING_FEEDBACK_DATA_FIELD1 = MSGDATA_SOA_GROUP_CONSTRUCTING_FEEDBACK_DATA + 1;
const MessageIDType MSGDATA_SOA_GROUP_CONSTRUCTING_FEEDBACK_DATA_FIELD2 = MSGDATA_SOA_GROUP_CONSTRUCTING_FEEDBACK_DATA + 2;
const MessageIDType MSGDATA_SOA_GROUP_CONSTRUCTING_FEEDBACK_DATA_FIELD3 = MSGDATA_SOA_GROUP_CONSTRUCTING_FEEDBACK_DATA + 3;
const MessageIDType MSGDATA_SOA_GROUP_CONSTRUCTING_FEEDBACK_DATA_FIELD4 = MSGDATA_SOA_GROUP_CONSTRUCTING_FEEDBACK_DATA + 4;
const MessageIDType MSGDATA_SOA_GROUP_CONSTRUCTING_FEEDBACK_DATA_FIELD5 = MSGDATA_SOA_GROUP_CONSTRUCTING_FEEDBACK_DATA + 5;


// 群组解散接收
const MessageIDType MSGDATA_SOA_GROUP_DISBANDING_DATA = 0x030061A35FF30000;
const MessageIDType MSGDATA_SOA_GROUP_DISBANDING_DATA_FIELD1 = MSGDATA_SOA_GROUP_DISBANDING_DATA + 1;
const MessageIDType MSGDATA_SOA_GROUP_DISBANDING_DATA_FIELD2 = MSGDATA_SOA_GROUP_DISBANDING_DATA + 2;
const MessageIDType MSGDATA_SOA_GROUP_DISBANDING_DATA_FIELD3 = MSGDATA_SOA_GROUP_DISBANDING_DATA + 3;
const MessageIDType MSGDATA_SOA_GROUP_DISBANDING_DATA_FIELD4 = MSGDATA_SOA_GROUP_DISBANDING_DATA + 4;

// 群组解散反馈
const MessageIDType MSGDATA_SOA_GROUP_DISBANDING_FEEDBACK_DATA = 0x030061A35FF31000;
const MessageIDType MSGDATA_SOA_GROUP_DISBANDING_FEEDBACK_DATA_FIELD1 = MSGDATA_SOA_GROUP_DISBANDING_FEEDBACK_DATA + 1;
const MessageIDType MSGDATA_SOA_GROUP_DISBANDING_FEEDBACK_DATA_FIELD2 = MSGDATA_SOA_GROUP_DISBANDING_FEEDBACK_DATA + 2;
const MessageIDType MSGDATA_SOA_GROUP_DISBANDING_FEEDBACK_DATA_FIELD3 = MSGDATA_SOA_GROUP_DISBANDING_FEEDBACK_DATA + 3;
const MessageIDType MSGDATA_SOA_GROUP_DISBANDING_FEEDBACK_DATA_FIELD4 = MSGDATA_SOA_GROUP_DISBANDING_FEEDBACK_DATA + 4;
const MessageIDType MSGDATA_SOA_GROUP_DISBANDING_FEEDBACK_DATA_FIELD5 = MSGDATA_SOA_GROUP_DISBANDING_FEEDBACK_DATA + 5;

/*

// 成员信息结构
struct member {
    char member[30]; // 成员ID（席位VMF）

    // 从ByteArray解析
    static struct member fromByteArray(const QByteArray& data) {
        struct member m;
        QDataStream stream(data);
        stream.setByteOrder(QDataStream::BigEndian);

        // 读取固定长度字符串
        stream.readRawData(m.member, sizeof(m.member));
        return m;
    }

    // 转换为ByteArray
    QByteArray toByteArray() const {
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::BigEndian);

        // 写入固定长度字符串
        stream.writeRawData(member, sizeof(member));
        return data;
    }
};

// 群组信息结构
struct Group {
    unsigned int groupID; // 群组ID
    char groupType; // 群组动作：1：构建，2：更新
    char groupName[32]; // 群组名称
    int actionType; // 群组主题
    int number; // 群组成员数量
    member groupMember[5]; // 群组成员信息

    // 从ByteArray解析
    static Group fromByteArray(const QByteArray& data) {
        Group g;
        QDataStream stream(data);
//        stream.setByteOrder(QDataStream::BigEndian);
        stream >> g.groupID;
        stream.readRawData(&g.groupType, sizeof(g.groupType));
        stream.readRawData(g.groupName, sizeof(g.groupName));
        stream >> g.actionType;
        stream >> g.number;
        // 读取成员数组
        for (int i = 0; i < qMin(g.number, 5); ++i) {
            QByteArray memberData(30, Qt::Uninitialized);
            stream.readRawData(memberData.data(), 30);
            g.groupMember[i] = member::fromByteArray(memberData);
        }

        return g;
    }

    // 转换为ByteArray
    QByteArray toByteArray() const {
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);
//        stream.setByteOrder(QDataStream::BigEndian);

        stream << groupID;
        stream.writeRawData(&groupType, sizeof(groupType));
        stream.writeRawData(groupName, sizeof(groupName));
        stream << actionType;
        stream << number;

        // 写入成员数组
        for (int i = 0; i < qMin(number, 5); ++i) {
            data.append(groupMember[i].toByteArray());
        }

        return data;
    }
};

// 构建反馈结构
struct GroupFeedback {
    long int time; // 构建时间
    unsigned int groupID; // 群组ID
    char groupName[40]; // 群组名称
    member groupMember[5]; // 群组成员信息

    // 从ByteArray解析
    static GroupFeedback fromByteArray(const QByteArray& data) {
        GroupFeedback gf;
        QDataStream stream(data);
        stream.setByteOrder(QDataStream::BigEndian);

        qint64 tempTime; // 使用qint64临时存储
        stream >> tempTime;
        gf.time = static_cast<long int>(tempTime); // 转换回long

        stream >> gf.groupID;
        stream.readRawData(gf.groupName, sizeof(gf.groupName));

        // 读取成员数组
        for (int i = 0; i < 5; ++i) {
            QByteArray memberData(41, Qt::Uninitialized);
            stream.readRawData(memberData.data(), 41);
            gf.groupMember[i] = member::fromByteArray(memberData);
        }

        return gf;
    }

    // 转换为ByteArray
    QByteArray toByteArray() const {
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);
//        stream.setByteOrder(QDataStream::BigEndian);

        stream << static_cast<qint64>(time); // 转换为qint64
        stream << groupID;
        stream.writeRawData(groupName, sizeof(groupName));

        // 写入成员数组
        for (int i = 0; i < 5; ++i) {
            data.append(groupMember[i].toByteArray());
        }

        return data;
    }
};

// 群组解散结构
struct GroupDelete {
    unsigned int groupID; // 群组ID

    // 从ByteArray解析
    static GroupDelete fromByteArray(const QByteArray& data) {
        GroupDelete gd;
        QDataStream stream(data);
//        stream.setByteOrder(QDataStream::BigEndian);

        stream >> gd.groupID;
        return gd;
    }

    // 转换为ByteArray
    QByteArray toByteArray() const {
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::BigEndian);

        stream << groupID;
        return data;
    }
};

// 解散反馈结构
struct GroupDeleteFeedback {
    long time; // 解散时间
    unsigned int groupID; // 群组ID
    char groupName[32]; // 群组名称

    // 从ByteArray解析
    static GroupDeleteFeedback fromByteArray(const QByteArray& data) {
        GroupDeleteFeedback gdf;
        QDataStream stream(data);
        stream.setByteOrder(QDataStream::BigEndian);

        qint64 tempTime; // 使用qint64临时存储
        stream >> tempTime;
        gdf.time = static_cast<long>(tempTime); // 转换回long

        stream >> gdf.groupID;
        stream.readRawData(gdf.groupName, sizeof(gdf.groupName));

        return gdf;
    }

    // 转换为ByteArray
    QByteArray toByteArray() const {
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::BigEndian);

        stream << static_cast<qint64>(time); // 转换为qint64
        stream << groupID;
        stream.writeRawData(groupName, sizeof(groupName));

        return data;
    }

    QJsonDocument toJson() const {
        QJsonObject obj;
        obj["time"] = static_cast<qint64>(time);
        obj["groupID"] = static_cast<qint64>(groupID);
        obj["groupName"] = QString::fromUtf8(groupName).trimmed();

        return QJsonDocument(obj);
    }
};
    /// 群组构建
    if(type == MSGDATA_SOA_GROUP_CONSTRUCTING_DATA&&sizeof (Comm_ReceiveMessage) == length){
        Comm_ReceiveMessage* rcv_msg = (Comm_ReceiveMessage*) msg;
        QByteArray data;
        data.append((char *)rcv_msg->data, rcv_msg->size);
        if (!rcv_msg || rcv_msg->size <= 0) {
            qDebug() << "Invalid UDP message received (null or empty)";
            return;
        }

        Group group;
        group = group.fromByteArray(data);
        qDebug()<<group.groupID;

        GroupFeedback feedback;
        feedback.groupID = group.groupID;
        memcpy(&feedback.groupName,group.groupName,sizeof(group.groupName));
        memcpy(&m_group_name,group.groupName,sizeof(group.groupName));
        feedback.time = QDateTime::currentDateTimeUtc().toSecsSinceEpoch();
        for(int i=0;i<5;i++){
            feedback.groupMember[i] = group.groupMember[i];
        }

        QByteArray feed_data = feedback.toByteArray();
        Comm_SendMessage send;
        send.data = (qulonglong)feed_data.data();
        send.size = feed_data.size();
        if(!PostMessage(MSGDATA_SOA_GROUP_CONSTRUCTING_FEEDBACK_DATA,&send,sizeof(Comm_SendMessage))){
            qDebug()<<"false post MSGDATA_ACCOUNT_SYNC_RE";
        }


    }
    /// 群组取消
    if(type == MSGDATA_SOA_GROUP_DISBANDING_DATA&&sizeof (Comm_ReceiveMessage) == length){
        Comm_ReceiveMessage* rcv_msg = (Comm_ReceiveMessage*) msg;
        QByteArray data;
        data.append((char *)rcv_msg->data, rcv_msg->size);
        if (!rcv_msg || rcv_msg->size <= 0) {
            qDebug() << "Invalid UDP message received (null or empty)";
            return;
        }
        GroupDelete group;
        group = group.fromByteArray(data);
        qDebug()<<group.groupID;
        GroupDeleteFeedback feedback;
        feedback.groupID = group.groupID;
        memcpy(&feedback.groupName,m_group_name,sizeof(m_group_name));
        feedback.time = QDateTime::currentDateTimeUtc().toSecsSinceEpoch();

        QByteArray feed_data = feedback.toByteArray();
        Comm_SendMessage send;
        send.data = (qulonglong)feed_data.data();
        send.size = feed_data.size();
        if(!PostMessage(MSGDATA_SOA_GROUP_DISBANDING_FEEDBACK_DATA,&send,sizeof(Comm_SendMessage))){
            qDebug()<<"false post MSGDATA_ACCOUNT_SYNC_RE";
        }
    }
*/
#endif // GROUPCHATCONFIG_H
