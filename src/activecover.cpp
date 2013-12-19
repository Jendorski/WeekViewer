/* Copyright (c) 2013 Florian Miess sinnix.de.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "activecover.hpp"

#include <bb/cascades/SceneCover>
#include <bb/cascades/Container>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>

using namespace bb::cascades;

ActiveCover::ActiveCover(QString colorsThumbnailForeground, QString colorsThumbnailBackground, QObject *parent)
	: SceneCover(parent),
	  m_colorsThumbnailForeground(colorsThumbnailForeground), m_colorsThumbnailBackground(colorsThumbnailBackground)
{
    QmlDocument *qml = QmlDocument::create("asset:///AppCover.qml").parent(parent);
    Container *mainContainer = qml->createRootObject<Container>();

    Color c2 = Color::fromARGB(m_colorsThumbnailForeground.toUInt());

    mainContainer->background(c2);
    setContent(mainContainer);

    // Retrieves the label from QML that we want to update
    m_coverLabel = mainContainer->findChild<Label*>("TheLabel");
    m_coverLabel->setParent(mainContainer);
    m_coverLabel->setProperty("forgroundcolor", "#FFFFFF");


    QObject::connect(Application::instance(), SIGNAL(thumbnail()), this, SLOT(update()));

}

void ActiveCover::update() {
	qDebug() << "FMI ############## current date: " << QDate::currentDate().toString(Qt::DefaultLocaleShortDate);

	if (QLocale().name().compare("de_DE") == 0 )
		m_coverLabel->setText(QDate::currentDate().toString("dd.MM.yyyy"));
	else
		m_coverLabel->setText(QDate::currentDate().toString(Qt::DefaultLocaleShortDate));
}


