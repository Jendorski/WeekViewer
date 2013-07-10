#ifndef CUSTOMDATAMODEL_HPP
#define CUSTOMDATAMODEL_HPP

#include <QObject>
#include <QVariant>
#include <bb/cascades/GroupDataModel>


class CustomGroupModel : public bb::cascades::GroupDataModel
{
	Q_OBJECT

public:
	CustomGroupModel(QObject* parent = 0);
	virtual ~CustomGroupModel();

	QVariant data(const QVariantList& indexPath);
	void loadEvents();

};

#endif
