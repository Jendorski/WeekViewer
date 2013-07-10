// List with context menu project template
#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include "customgroupmodel.hpp"

#include <QObject>
#include <bb/cascades/GroupDataModel>
#include <bb/system/InvokeManager>
#include <QDateTime>
#include <bb/cascades/AbstractPane>


class ApplicationUI : public QObject
{
    Q_OBJECT

    Q_PROPERTY(CustomGroupModel* model READ model NOTIFY modelChanged)

public:
    ApplicationUI(QObject *parent = 0);
    virtual ~ApplicationUI() {}

    Q_INVOKABLE void loadEvent(int id, int account, QDateTime start);
    Q_INVOKABLE void openCalendar();
    Q_INVOKABLE void loadEvents();

    // If any Q_ASSERT statement(s) indicate that the slot failed to connect to
	// the signal, make sure you know exactly why this has happened. This is not
	// normal, and will cause your app to stop working!!
	bool connectResult;

private Q_SLOTS:
	// This slot handles the result of an invocation
	void processInvokeReply();

	void childCardDone(const bb::system::CardDoneMessage &message);

Q_SIGNALS:
	void modelChanged();

private:
	CustomGroupModel* model();
	CustomGroupModel* m_model;
    bb::system::InvokeManager* m_invokeManager;

};

#endif /* ApplicationUI_HPP_ */
