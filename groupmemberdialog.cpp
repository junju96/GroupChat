// groupmemberdialog.cpp
#include "groupmemberdialog.h"
#include <QLineEdit>
#include <QMessageBox>

GroupMemberDialog::GroupMemberDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("创建新群组");
    setMinimumWidth(400);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // 群组名称
    QHBoxLayout* nameLayout = new QHBoxLayout;
    QLabel* nameLabel = new QLabel("群组名称:", this);
    groupNameEdit = new QLineEdit(this);
    nameLayout->addWidget(nameLabel);
    nameLayout->addWidget(groupNameEdit);
    mainLayout->addLayout(nameLayout);

    // 可选成员列表
    QLabel* memberLabel = new QLabel("选择群组成员:", this);
    mainLayout->addWidget(memberLabel);

    memberListWidget = new QListWidget(this);
    memberListWidget->setSelectionMode(QAbstractItemView::MultiSelection);

    QStringList availableMembers = {"协同席", "一号操控席", "二号操控席", "三号操控席", "四号操控席"};
    foreach (const QString& member, availableMembers) {
        QListWidgetItem* item = new QListWidgetItem(member, memberListWidget);
        item->setCheckState(Qt::Checked);
    }

    mainLayout->addWidget(memberListWidget);

    // 我的角色
    QHBoxLayout* roleLayout = new QHBoxLayout;
    QLabel* roleLabel = new QLabel("我的角色:", this);
    myRoleComboBox = new QComboBox(this);
    myRoleComboBox->addItems(availableMembers);
    roleLayout->addWidget(roleLabel);
    roleLayout->addWidget(myRoleComboBox);
    mainLayout->addLayout(roleLayout);

    // 按钮
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    okButton = new QPushButton("创建", this);
    cancelButton = new QPushButton("取消", this);

    connect(okButton, &QPushButton::clicked, this, &GroupMemberDialog::onOkButtonClicked);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    mainLayout->addLayout(buttonLayout);
}

QString GroupMemberDialog::getGroupName() const
{
    return groupNameEdit->text().trimmed();
}

QStringList GroupMemberDialog::getSelectedMembers() const
{
    QStringList members;
    for (int i = 0; i < memberListWidget->count(); ++i) {
        QListWidgetItem* item = memberListWidget->item(i);
        if (item->checkState() == Qt::Checked) {
            members.append(item->text());
        }
    }
    return members;
}

QString GroupMemberDialog::getMyRole() const
{
    return myRoleComboBox->currentText();
}

void GroupMemberDialog::onOkButtonClicked()
{
    if (getGroupName().isEmpty()) {
        QMessageBox::warning(this, "警告", "群组名称不能为空！");
        return;
    }

    if (getSelectedMembers().isEmpty()) {
        QMessageBox::warning(this, "警告", "至少选择一个群组成员！");
        return;
    }

    accept();
}
