#include "customgroupmodel.hpp"

#include <QObject>
#include <bb/cascades/GroupDataModel>
#include <bb/pim/calendar/CalendarEvent>
#include <bb/pim/calendar/EventSearchParameters>
#include <bb/pim/calendar/CalendarService>
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

		if (mData["myType"].toString().compare("Heute") == 0)
		{
			if (diff == 86400)
				mData["timeString"] = QString::fromUtf8("Ganztägig");
			else
				mData["timeString"] = startTime.time().toString(TIME_FORMAT) + " - " + endTime.time().toString(TIME_FORMAT);
		}
		else {
			if (diff == 86400)
				mData["timeString"] = mData["timeString"] = startTime.date().toString(Qt::DefaultLocaleShortDate) + QString::fromUtf8("    Ganztägig");
			else
				mData["timeString"] = startTime.date().toString(Qt::DefaultLocaleShortDate) + "    " + startTime.time().toString(TIME_FORMAT) + " - " + endTime.time().toString(TIME_FORMAT);
		}
		return mData;
	}
}

void CustomGroupModel::loadEvents()
{
	const QDate today = QDate::currentDate();
	const QTime midnight(0, 0, 0);

	QDateTime searchStartTime = QDateTime(today, midnight);
	QDateTime searchEndTime = QDateTime(today.addDays(1), midnight);

	// Setup the search parameters with time range as specified by filter criterion
	EventSearchParameters searchParameters;
	searchParameters.setStart(searchStartTime);
	searchParameters.setEnd(searchEndTime);
	searchParameters.setDetails(DetailLevel::Weekly);

	bb::pim::calendar::CalendarService* calendarService = new CalendarService();
	const QList<CalendarEvent> events = calendarService->events(searchParameters);

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
		entry["subject"] = event.subject();
		entry["order"] = order++;
		entry["startTime"] = event.startTime();
		entry["endTime"] = event.endTime();
		entry["timeString"] = "";
		entry["account"] = event.accountId();

		qDebug() << "FMI ######### id:" << event.id() << " subject" << event.subject() << " startTime:" << event.startTime().toString(Qt::DefaultLocaleShortDate);

		entries.append(entry);
	}
//    QVariantMap empty;
//    empty["myType"] = QVariant("Heute");
//    entries.append(empty);

	// Setup the search parameters with time range as specified by filter criterion
	searchStartTime = QDateTime(today.addDays(1), QTime(0,0,1));
	searchEndTime = QDateTime(today.addDays(8), midnight);
	searchParameters.setStart(searchStartTime);
	searchParameters.setEnd(searchEndTime);
	searchParameters.setDetails(DetailLevel::Weekly);

	const QList<CalendarEvent> events2 = calendarService->events(searchParameters);

	// Iterate over the list of events
	foreach (const CalendarEvent &event, events2) {
		// Copy the data into a model entry
		QVariantMap entry;
		entry["myType"] = QVariant("Woche");
		entry["eventId"] = event.id();
		entry["accountId"] = event.accountId();
		entry["subject"] = event.subject();
		entry["order"] = order++;
		entry["startTime"] = event.startTime();
		entry["endTime"] = event.endTime();
		entry["timeString"] = "";
		entry["account"] = event.accountId();

		qDebug() << "FMI ######### id:" << event.id() << " subject" << event.subject() << " startTime:" << event.startTime().toString(Qt::DefaultLocaleShortDate);

		entries.append(entry);
	}

	setSortingKeys(QStringList() << "myType" << "startTime");
	setGrouping(ItemGrouping::ByFullValue);
	insertList(entries);
}
