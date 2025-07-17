// groupchatwindow.h
#ifndef GROUPCHATWINDOW_H
#define GROUPCHATWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QStringList>
#include "database/chatdatabase.h"
#include "groupmemberdialog.h" // 添加对话框头文件
#include "messagebubble/messagebubble.h"

class GroupChatWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit GroupChatWindow(QWidget *parent = nullptr);

private slots:
    void sendMessage();
    void onGroupSelected(QListWidgetItem* item);
    void addGroup();
    void deleteGroup();

private:
    QListWidget* groupListWidget;
    QScrollArea* messageDisplay;
    QWidget* messageContainer;
    QVBoxLayout* messageLayout;
    QTextEdit* messageInput;
    QPushButton* sendButton;
    QPushButton* addGroupButton;
    QPushButton* deleteGroupButton;
    QString currentGroup;
    QStringList users;
    QStringList replies;
    ChatDatabase* chatDatabase;

    void initUsers();
    void createGroupListPanel(QHBoxLayout* mainLayout);
    void createChatArea(QHBoxLayout* mainLayout);
    void addMessage(const QString& sender, const QString& message,
                    const QDateTime& time, MessageBubble::BubbleType type);
    void clearMessages();
    void loadGroupMessages(const QString& groupName);
    void loadGroupsFromDatabase();
    void scrollToBottom();
};

#endif // GROUPCHATWINDOW_H
