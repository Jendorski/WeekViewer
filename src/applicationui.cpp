// List with context menu project template
#include "applicationui.hpp"
#include "activecover.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/system/InvokeRequest>
#include <bb/system/InvokeReply>
#include <bb/system/InvokeTarget>
#include <bb/system/InvokeTargetReply>
#include <bb/PpsObject>
#include <bb/cascades/Container>
#include <bb/cascades/SceneCover>
#include <bb/system/SystemDialog>
#include <bb/data/JsonDataAccess>

using namespace bb::cascades;
using namespace bb::system;


ApplicationUI::ApplicationUI(QObject *parent)
	: QObject(parent)
	, m_invokeManager(new InvokeManager(this))
	, m_model(new CustomGroupModel())
{
	qmlRegisterType<CustomGroupModel>("bb.mymodel", 1, 0, "CustomGroupModel");
	qmlRegisterType<bb::system::SystemDialog>("bb.system.SystemDialog", 1, 0, "SystemDialog");

	readJsonEntries();

	QString locale_string = QLocale().name();
	QString file_name = QString("WeekViewer_%1").arg(locale_string);
	if (m_pTranslator.load(file_name, "app/native/qm")) {
		QCoreApplication::instance()->installTranslator(&m_pTranslator);
	}

    // create scene document from main.qml asset
    // set parent to created document to ensure it exists for the whole application lifetime
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
	qml->setContextProperty("_app", this);

	ActiveCover* scene = new ActiveCover(m_colorsThumbnailForeground, m_colorsThumbnailBackground);
	Application::instance()->setCover(scene);
	qml->setContextProperty("activeFrame", scene);

    // create root object for the UI
	bb::cascades::AbstractPane *root = qml->createRootObject<AbstractPane>();
    // set created root object as a scene
	Application::instance()->setScene(root);
}

void ApplicationUI::openCalendar()
{
	InvokeRequest invokeRequest;
	invokeRequest.setAction("bb.calendar.OPEN");
	invokeRequest.setMimeType("text/calendar");
	QVariantMap data;
	data.insert("view", "agenda");
	invokeRequest.setData(bb::PpsObject::encode(data, NULL));

	// Start the invocation
	m_invokeManager->invoke(invokeRequest);
}

void ApplicationUI::loadEvent(int id, int account, QDateTime start)
{
	QString startString = start.toString("yyyy-MM-dd hh:mm:ss");
	qDebug() << "FMI ######### received event " << id << " account " << account << " beginning " << startString;

	InvokeRequest invokeRequest;
	invokeRequest.setAction("bb.calendar.OPEN");
	invokeRequest.setTarget("sys.pim.calendar.viewer.ics");
	invokeRequest.setMimeType("text/calendar");
	QVariantMap data;
	data.insert("accountId", account);
	data.insert("eventId", id);
	data.insert("type", "event");
	data.insert("start", startString);
//	data.insert("start", "2013-12-07 11:00:00");

	//invokeRequest.setData(bb::PpsObject::encode(data, NULL));
	bool ok;
	QByteArray encData = bb::PpsObject::encode(data, &ok);
	if (ok) {
		invokeRequest.setData(encData);
		// Start the invocation
		const InvokeReply *reply = m_invokeManager->invoke(invokeRequest);
	//	reply->setParent(this);

		connect(reply, SIGNAL(finished()), this, SLOT(processInvokeReply()));

		connectResult = connect(m_invokeManager, SIGNAL(childCardDone(const bb::system::CardDoneMessage&)), this, SLOT(childCardDone(const bb::system::CardDoneMessage&)));
	}
}

void ApplicationUI::childCardDone(const bb::system::CardDoneMessage &message)
{
	qDebug() << "FMI ######### childCardDone";
	loadEvents();
}

void ApplicationUI::processInvokeReply()
{
	qDebug() << "FMI ######### processInvokeReply";
	InvokeReply *reply = qobject_cast<InvokeReply*>(sender());

	// Check for errors during invocation

	switch (reply->error()) {
	case InvokeReplyError::BadRequest:
		qDebug() << "FMI ##### fehler? "<< tr("[ErrorBadRequest] Invoke Failed!");
		break;
	case InvokeReplyError::Internal:
		qDebug() << "FMI ##### fehler? "<< tr("[ErrorInternal] Invoke Failed!");
		break;
	case InvokeReplyError::NoTarget:
		qDebug() << "FMI ##### fehler? "<< tr("[ErrorNoTarget] Invoke Failed!");
		break;
	case InvokeReplyError::TargetNotOwned:
		qDebug() << "FMI ##### fehler? "<< tr("[ErrorTargetNotOwned] Invoke Failed.");
		break;
	default:
		qDebug() << "FMI ##### kein fehler ";
		break;
	}

	// Delete the reply later on
	reply->deleteLater();
}

void ApplicationUI::loadEvents()
{
	m_model->loadEvents();
	emit modelChanged();
}

CustomGroupModel* ApplicationUI::model()
{
	qDebug() << "FMI ######## load events!";
	return m_model;
}

void ApplicationUI::readJsonEntries()
{
	// Load the JSON data
	bb::data::JsonDataAccess jda;
	QVariant list = jda.load("app/native/assets/models/settings.json");
	QVariantMap currentEntriesMap = list.value<QVariantList>().first().value<QVariantMap>();

	m_colorsThumbnailForeground = currentEntriesMap["colors-thumbnail-foreground"].toString();
	m_colorsThumbnailBackground = currentEntriesMap["colors-thumbnail-background"].toString();
	m_colorsFullscreenForeground = currentEntriesMap["colors-fullscreen-foreground"].toString();

//	qDebug() <<  "#########4 " << m_colorsFullscreenForeground;
}

QString ApplicationUI::colorsThumbnailForeground()
{
	return m_colorsThumbnailForeground;
}

QString ApplicationUI::colorsThumbnailBackground()
{
	return m_colorsThumbnailBackground;
}
QString ApplicationUI::colorsFullscreenForeground()
{
	return m_colorsFullscreenForeground;
}
