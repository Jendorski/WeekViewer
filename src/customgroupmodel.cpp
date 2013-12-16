#include "customgroupmodel.hpp"

#include <QObject>
#include <bb/cascades/GroupDataModel>
#include <bb/pim/calendar/CalendarEvent>
#include <bb/pim/calendar/CalendarFolder>
#include <bb/pim/calendar/EventSearchParameters>
#include <bb/pim/calendar/CalendarService>
#include <sstream>
#include <QDateTime>

using namespace bb::cascades;
using namespace bb::pim::calendar;
using namespace bb::cascades;

const QString TIME_FORMAT = "hh:mm";

CustomGroupModel::CustomGroupModel(QObject* parent)
	: bb::cascades::GroupDataModel()
{
    setParent(parent);
}

CustomGroupModel::~CustomGroupModel()
{
}


QVariant CustomGroupModel::data(const QVariantList& indexPath) {
	// if it is a header
	if (this->hasChildren(indexPath)){
//		qDebug() << "FMI ######### header";
//		QDate headerResult = GroupDataModel::data(indexPath).toDate();
//		QString headerFormatted = headerResult.toString("dd.MM.yy");
//		return QVariant(headerFormatted);
		QVariant mData = GroupDataModel::data(indexPath);
		return mData;
	}
	else{
		QVariantMap mData = GroupDataModel::data(indexPath).toMap();
		QDateTime startTime = mData["startTime"].toDateTime();
		QDateTime endTime = mData["endTime"].toDateTime();
		int diff = startTime.secsTo(endTime);
//		qDebug() << "FMI ######### item " << mData["myType"].toString() << " subject" << mData["subject"].toString() << " time:" << startTime.toString() << " " << startTime.time().toString() << " " << diff;// << " " << endTime;

		if ((mData["myType"].toString().compare("Heute") == 0) || (mData["myType"].toString().compare("Morgen") == 0))
		{
			if (diff == 86400)
				mData["timeString"] = QString::fromUtf8("Ganztägig");
			else
				mData["timeString"] = startTime.time().toString(TIME_FORMAT) + " - " + endTime.time().toString(TIME_FORMAT);
		}
		else {
			if (diff == 86400)
				mData["timeString"] = startTime.date().toString("ddd") + " " + startTime.date().toString(Qt::DefaultLocaleShortDate) + QString::fromUtf8("    Ganztägig");
			else
				mData["timeString"] = startTime.date().toString("ddd") + " " + startTime.date().toString(Qt::DefaultLocaleShortDate) + "    " + startTime.time().toString(TIME_FORMAT) + " - " + endTime.time().toString(TIME_FORMAT);
		}
		return mData;
	}
}

void CustomGroupModel::loadEvents()
{
	const QDate today = QDate::currentDate();
	const QTime midnight(0, 0, 0);
	const QTime startday(0, 0, 1);
	const QTime endday(23, 59, 59);

	QDateTime searchStartTime = QDateTime(today, startday);
	QDateTime searchEndTime = QDateTime(today, endday);

	// Setup the search parameters with time range as specified by filter criterion
	EventSearchParameters searchParameters;
	searchParameters.setStart(searchStartTime);
	searchParameters.setEnd(searchEndTime);
	searchParameters.setDetails(DetailLevel::Weekly);


	bb::pim::calendar::CalendarService* calendarService = new CalendarService();
	const QList<CalendarEvent> events = calendarService->events(searchParameters);

	// get list of all calendars
	QList<CalendarFolder> allFolders = calendarService->folders();
	CalendarFolder cf = allFolders.at(0);
	QMap<std::string, uint > accountColor;
	for (int i = 0; i < allFolders.size(); i++) {

		std::stringstream keyStream;
		keyStream << allFolders.at(i).accountId() << allFolders.at(i).id();
		std::string key = keyStream.str();

		accountColor.insert(key, allFolders.at(i).color());
//		qDebug() << "############## folder:" << allFolders.at(i).name() << ", key:" <<  QString::fromStdString(key) << "=" << QString::fromStdString(value);
//		accountColor.insert(as.append(fs), allFolders.at(i).color());
	}

	// Clear the old events information from the model
	clear();


	// Iterate over the list of events
	QVariantList entries;
	int order = 0;
	foreach (const CalendarEvent &event, events) {
		// Copy the data into a model entry
		QVariantMap entry;
		entry["myType"] = QVariant("Heute");
		entry["eventId"] = event.id();
		entry["accountId"] = event.accountId();
		entry["subject"] = event.subject().replace('&', "&amp;").replace('<', "&lt;").replace('>', "&gt;").replace('"', "&quot;");
		entry["order"] = order++;
		entry["startTime"] = event.startTime();
		entry["endTime"] = event.endTime();
		entry["timeString"] = "";
		entry["account"] = event.accountId();

		std::stringstream keyStream;
		keyStream << event.accountId() << event.folderId();
		std::string key = keyStream.str();
		entry["color24"] = QString::number(accountColor[key], 16);

		qDebug() << "FMI ######### key:" <<  QString::fromStdString(key) << "=" << accountColor[key];
		qDebug() << "FMI #########    id:" << event.id() << " subject" << event.subject() << " startTime:" << event.startTime().toString(Qt::DefaultLocaleShortDate);

		entries.append(entry);
	}


	// Setup the search parameters with time range as specified by filter criterion
	searchStartTime = QDateTime(today.addDays(1), startday);
	searchEndTime = QDateTime(today.addDays(1), endday);
	searchParameters.setStart(searchStartTime);
	searchParameters.setEnd(searchEndTime);
	searchParameters.setDetails(DetailLevel::Weekly);

	const QList<CalendarEvent> events2 = calendarService->events(searchParameters);

	// Iterate over the list of events
	foreach (const CalendarEvent &event, events2) {
		// Copy the data into a model entry
		QVariantMap entry;
		entry["myType"] = QVariant("Morgen");
		entry["eventId"] = event.id();
		entry["accountId"] = event.accountId();
		entry["subject"] = event.subject().replace('&', "&amp;").replace('<', "&lt;").replace('>', "&gt;").replace('"', "&quot;");
		entry["order"] = order++;
		entry["startTime"] = event.startTime();
		entry["endTime"] = event.endTime();
		entry["timeString"] = "";
		entry["account"] = event.accountId();

		std::stringstream keyStream;
		keyStream << event.accountId() << event.folderId();
		std::string key = keyStream.str();
		entry["color24"] = QString::number(accountColor[key], 16);

		qDebug() << "FMI ######### key:" <<  QString::fromStdString(key) << "=" << accountColor[key];
		qDebug() << "FMI #########    id:" << event.id() << " subject" << event.subject() << " startTime:" << event.startTime().toString(Qt::DefaultLocaleShortDate);
		entries.append(entry);
	}

	// Setup the search parameters with time range as specified by filter criterion
	searchStartTime = QDateTime(today.addDays(2), startday);
	searchEndTime = QDateTime(today.addDays(7), endday);
	searchParameters.setStart(searchStartTime);
	searchParameters.setEnd(searchEndTime);
	searchParameters.setDetails(DetailLevel::Weekly);

	const QList<CalendarEvent> events3 = calendarService->events(searchParameters);

	// Iterate over the list of events
	foreach (const CalendarEvent &event, events3) {
		// Copy the data into a model entry
		QVariantMap entry;
		entry["myType"] = QVariant("Woche");
		entry["eventId"] = event.id();
		entry["accountId"] = event.accountId();
		entry["subject"] = event.subject().replace('&', "&amp;").replace('<', "&lt;").replace('>', "&gt;").replace('"', "&quot;");
		entry["order"] = order++;
		entry["startTime"] = event.startTime();
		entry["endTime"] = event.endTime();
		entry["timeString"] = "";
		entry["account"] = event.accountId();

		std::stringstream keyStream;
		keyStream << event.accountId() << event.folderId();
		std::string key = keyStream.str();
		entry["color24"] = QString::number(accountColor[key], 16);

		qDebug() << "FMI ######### key:" <<  QString::fromStdString(key) << "=" << accountColor[key];
		qDebug() << "FMI #########    id:" << event.id() << " subject" << event.subject() << " startTime:" << event.startTime().toString(Qt::DefaultLocaleShortDate);
		entries.append(entry);
	}

	setSortingKeys(QStringList() << "myType" << "startTime");
	setGrouping(ItemGrouping::ByFullValue);
	insertList(entries);
}

