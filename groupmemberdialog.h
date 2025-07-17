// groupmemberdialog.h
#ifndef GROUPMEMBERDIALOG_H
#define GROUPMEMBERDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

class GroupMemberDialog : public QDialog
{
    Q_OBJECT
public:
    explicit GroupMemberDialog(QWidget *parent = nullptr);

    QString getGroupName() const;
    QStringList getSelectedMembers() const;
    QString getMyRole() const;

private slots:
    void onOkButtonClicked();

private:
    QLineEdit* groupNameEdit;
    QListWidget* memberListWidget;
    QComboBox* myRoleComboBox;
    QPushButton* okButton;
    QPushButton* cancelButton;
};

#endif // GROUPMEMBERDIALOG_H
