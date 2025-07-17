#include "groupchatwindow.h"
#include "messageitem/messageitem.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QDateTime>
#include <QTimer>
#include <QRandomGenerator>
#include <QInputDialog>
#include <QScrollBar>

GroupChatWindow::GroupChatWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("群组消息发送界面");
    setMinimumSize(800, 600);

    initUsers();

    // 初始化数据库
    chatDatabase = new ChatDatabase(this);
    if (!chatDatabase->initDatabase()) {
        QMessageBox::warning(this, "数据库错误", "无法初始化聊天记录数据库！");
    }

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    createGroupListPanel(mainLayout);
    createChatArea(mainLayout);

    connect(sendButton, &QPushButton::clicked, this, &GroupChatWindow::sendMessage);
    connect(groupListWidget, &QListWidget::itemClicked, this, &GroupChatWindow::onGroupSelected);
    connect(addGroupButton, &QPushButton::clicked, this, &GroupChatWindow::addGroup);
    connect(deleteGroupButton, &QPushButton::clicked, this, &GroupChatWindow::deleteGroup);

    // 从数据库加载群组
    loadGroupsFromDatabase();

    // 如果没有群组，显示提示信息
    if (groupListWidget->count() == 0) {
        clearMessages();
        addMessage("系统消息", "当前没有群组，请点击'新增群组'按钮创建一个新群组",
                   QDateTime::currentDateTime(), MessageBubble::Other);
    } else {
        groupListWidget->setCurrentRow(0);
        onGroupSelected(groupListWidget->currentItem());
    }
}

void GroupChatWindow::loadGroupsFromDatabase()
{
    // 清空现有群组列表
    groupListWidget->clear();

    // 从数据库加载群组
    QStringList groups = chatDatabase->getAllGroups();

    // 添加到列表中
    foreach (const QString& group, groups) {
        groupListWidget->addItem(group);
    }
}

void GroupChatWindow::initUsers() {
    // 直接初始化列表
    users = QStringList() << "张三" << "李四" << "王五" << "赵六"
                          << "小明" << "小红" << "小刚" << "小丽"
                          << "小林" << "小华";

    replies = QStringList() << "好的，我知道了" << "收到，谢谢"
                            << "哈哈，有意思" << "明天再讨论吧"
                            << "我觉得可以" << "没问题"
                            << "等我一下" << "已阅读"
                            << "太棒了" << "大家有什么想法？"
                            << "我稍后回复" << "今天天气不错"
                            << "会议什么时候开始？" << "我准备好了"
                            << "再见";
}

void GroupChatWindow::createGroupListPanel(QHBoxLayout* mainLayout) {
    QWidget* groupPanel = new QWidget(this);
    QVBoxLayout* groupLayout = new QVBoxLayout(groupPanel);
    groupLayout->setContentsMargins(5, 5, 5, 5);

    // 群组列表标题
    QLabel* titleLabel = new QLabel("群组列表", this);
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; padding: 5px;");
    groupLayout->addWidget(titleLabel);

    // 按钮区域
    QHBoxLayout* buttonLayout = new QHBoxLayout;

    addGroupButton = new QPushButton("新增群组", this);
    addGroupButton->setStyleSheet(R"(
            QPushButton {
                background-color: #2196F3;
                color: white;
                border-radius: 5px;
                min-width: 120px; /* 增加最小宽度 */
                padding: 8px;
            }
            QPushButton:hover {
                background-color: #1976D2;
            }
        )");
    buttonLayout->addWidget(addGroupButton);

    deleteGroupButton = new QPushButton("删除选中群组", this);
    deleteGroupButton->setStyleSheet(R"(
            QPushButton {
                background-color: #F44336;
                color: white;
                border-radius: 5px;
                min-width: 120px; /* 增加最小宽度 */
                padding: 8px;
            }
            QPushButton:hover {
                background-color: #D32F2F;
            }
        )");
    buttonLayout->addWidget(deleteGroupButton);

    groupLayout->addLayout(buttonLayout);

    // 群组列表
    groupListWidget = new QListWidget(this);
    groupListWidget->addItems({"林萃西里&槐树岭", "微信运动", "茄子快传工作组", "2025人朝清朝阳...",
                               "三年级7.7-7.17 15...", "招商银行信用卡", "大众点评", "妈妈湖北",
                               "刘志刚@一汽大众", "菜鸟裹裹", "俊发七彩服务"});
    groupListWidget->setStyleSheet(R"(
        QListWidget {
            background-color: #F5F5F5;
            border: none;
        }
        QListWidget::item {
            padding: 10px;
            border-bottom: 1px solid #EEEEEE;
            height: 60px;
        }
        QListWidget::item:selected {
            background-color: #E0E0E0;
            color: #000000;
        }
    )");
    groupLayout->addWidget(groupListWidget, 1);

    mainLayout->addWidget(groupPanel, 1);
}

void GroupChatWindow::createChatArea(QHBoxLayout* mainLayout) {
    QWidget* chatWidget = new QWidget(this);
    QVBoxLayout* chatLayout = new QVBoxLayout(chatWidget);

    // 消息显示区域
    messageDisplay = new QScrollArea(this);
    messageDisplay->setWidgetResizable(true);
    messageDisplay->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    messageDisplay->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    messageDisplay->setStyleSheet("background-color: #F0F0F0;");

    messageContainer = new QWidget(messageDisplay);
    messageLayout = new QVBoxLayout(messageContainer);
    messageLayout->setAlignment(Qt::AlignTop);
    messageLayout->setSpacing(0);
    messageLayout->addStretch();

    messageDisplay->setWidget(messageContainer);
    chatLayout->addWidget(messageDisplay, 5);

    // 消息输入区域
    QWidget* inputWidget = new QWidget(this);
    QHBoxLayout* inputLayout = new QHBoxLayout(inputWidget);
    inputLayout->setContentsMargins(5, 5, 5, 5);

    messageInput = new QTextEdit(this);
    messageInput->setPlaceholderText("请输入消息...");
    messageInput->setMaximumHeight(100);
    messageInput->setStyleSheet("border: 1px solid #CCCCCC; border-radius: 5px; padding: 5px;");
    inputLayout->addWidget(messageInput, 4);

    sendButton = new QPushButton("发送", this);
    sendButton->setMinimumHeight(40);
    sendButton->setStyleSheet("background-color: #4CAF50; color: white; border-radius: 5px;");
    inputLayout->addWidget(sendButton, 1);

    chatLayout->addWidget(inputWidget, 1);

    mainLayout->addWidget(chatWidget, 3);
}

void GroupChatWindow::sendMessage()
{
    QString message = messageInput->toPlainText().trimmed();
    if (message.isEmpty()) {
        QMessageBox::warning(this, "警告", "消息不能为空！");
        return;
    }

    QDateTime now = QDateTime::currentDateTime();

    // 获取当前用户在该群组中的角色
    QString myRole = chatDatabase->getMyRole(currentGroup);

    // 打印调试信息，检查角色是否正确
    qDebug() << "当前用户在群组" << currentGroup << "中的角色是:" << myRole;

    if (myRole.isEmpty()) {
        myRole = "我"; // 默认使用"我"
    }

    addMessage(myRole, message, now, MessageBubble::Self);

    chatDatabase->saveMessage(currentGroup, myRole, message, now);

    messageInput->clear();

    // 模拟收到回复
    if (QRandomGenerator::global()->bounded(2) == 0) {
        QTimer::singleShot(1000 + QRandomGenerator::global()->bounded(2000), [this]() {
            QStringList members = chatDatabase->getGroupMembers(currentGroup);
            QString myRole = chatDatabase->getMyRole(currentGroup);

            // 从成员中排除自己
            members.removeOne(myRole);

            if (!members.isEmpty()) {
                QString sender = members[QRandomGenerator::global()->bounded(members.size())];
                QString reply = replies[QRandomGenerator::global()->bounded(replies.size())];
                QDateTime time = QDateTime::currentDateTime();

                addMessage(sender, reply, time, MessageBubble::Other);
                chatDatabase->saveMessage(currentGroup, sender, reply, time);
            }
        });
    }
}

void GroupChatWindow::onGroupSelected(QListWidgetItem* item) {
    currentGroup = item->text();
    clearMessages();

    // 添加欢迎消息
    addMessage("系统消息", "欢迎来到 " + currentGroup, QDateTime::currentDateTime(), MessageBubble::Other);

    // 加载历史消息
    loadGroupMessages(currentGroup);
}

// 添加加载历史消息的方法
void GroupChatWindow::loadGroupMessages(const QString& groupName) {
    if (groupName.isEmpty()) return;

    QList<QPair<QString, QPair<QString, QDateTime>>> messages =
            chatDatabase->getGroupMessages(groupName);

    // 获取当前用户在该群组中的角色
    QString myRole = chatDatabase->getMyRole(groupName);

    // 打印调试信息，检查角色是否正确
    qDebug() << "当前用户在群组" << groupName << "中的角色是:" << myRole;

    for (const auto& msgPair : messages) {
        QString message = msgPair.first;
        QString sender = msgPair.second.first;
        QDateTime time = msgPair.second.second;

        // 判断是否是当前用户发送的消息
        MessageBubble::BubbleType type = (sender == myRole) ?
                    MessageBubble::Self : MessageBubble::Other;

        addMessage(sender, message, time, type);
    }
}

void GroupChatWindow::addGroup()
{
    GroupMemberDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        QString groupName = dialog.getGroupName();
        QStringList members = dialog.getSelectedMembers();
        QString myRole = dialog.getMyRole();

        // 检查群组是否已存在
        for (int i = 0; i < groupListWidget->count(); ++i) {
            if (groupListWidget->item(i)->text() == groupName) {
                QMessageBox::warning(this, "警告", "该群组名称已存在！");
                return;
            }
        }

        // 添加到数据库
        if (chatDatabase->addGroup(groupName, members, myRole)) {
            // 添加到列表
            groupListWidget->addItem(groupName);

            // 选中新添加的群组
            int newRow = groupListWidget->count() - 1;
            groupListWidget->setCurrentRow(newRow);
            onGroupSelected(groupListWidget->item(newRow));
        } else {
            QMessageBox::warning(this, "错误", "无法创建群组！");
        }
    }
}

// 修改 deleteGroup 方法
void GroupChatWindow::deleteGroup()
{
    QListWidgetItem* currentItem = groupListWidget->currentItem();
    if (!currentItem) {
        QMessageBox::information(this, "提示", "请先选择要删除的群组！");
        return;
    }

    QString groupName = currentItem->text();
    if (QMessageBox::question(this, "确认删除", "确定要删除群组 '" + groupName + "' 吗？") == QMessageBox::Yes) {
        // 从数据库删除
        if (chatDatabase->deleteGroup(groupName)) {
            if (groupName == currentGroup) {
                clearMessages();
                currentGroup = "";
            }

            // 从列表删除
            delete groupListWidget->takeItem(groupListWidget->currentRow());

            // 更新界面
            if (groupListWidget->count() > 0) {
                groupListWidget->setCurrentRow(0);
                onGroupSelected(groupListWidget->currentItem());
            } else {
                clearMessages();
                addMessage("系统消息", "当前没有群组，请点击'新增群组'按钮创建一个新群组",
                           QDateTime::currentDateTime(), MessageBubble::Other);
            }
        } else {
            QMessageBox::warning(this, "错误", "无法删除群组！");
        }
    }
}

void GroupChatWindow::addMessage(const QString& sender, const QString& message, const QDateTime& time,
                                 MessageBubble::BubbleType type) {
    // 移除底部的拉伸项
    if (messageLayout->count() > 0) {
        QLayoutItem* item = messageLayout->takeAt(messageLayout->count() - 1);
        delete item;
    }

    // 添加新消息
    MessageItem* messageItem = new MessageItem(sender, message, time, type, messageContainer);
    messageLayout->addWidget(messageItem);

    // 重新添加拉伸项，使消息保持在顶部
    messageLayout->addStretch();

    // 调用辅助函数滚动到底部
    scrollToBottom();
}

void GroupChatWindow::clearMessages() {
    // 移除所有消息项
    while (messageLayout->count() > 0) {
        QLayoutItem* item = messageLayout->takeAt(0);
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }

    // 添加拉伸项，使消息保持在顶部
    messageLayout->addStretch();
}

void GroupChatWindow::scrollToBottom() {
    qDebug() << "尝试滚动到底部";

    QTimer::singleShot(0, [this]() {
        QTimer::singleShot(0, [this]() {
            QScrollBar* scrollBar = messageDisplay->verticalScrollBar();
            if (scrollBar) {
                qDebug() << "滚动前: 最大值 =" << scrollBar->maximum()
                         << ", 当前值 =" << scrollBar->value();
                scrollBar->setValue(scrollBar->maximum());
                qDebug() << "滚动后: 当前值 =" << scrollBar->value();
            }
        });
    });
}
