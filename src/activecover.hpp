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

#ifndef ACTIVECOVER_HPP_
#define ACTIVECOVER_HPP_

#include <QObject>
#include <bb/cascades/Label>
#include <bb/cascades/SceneCover>

using namespace ::bb::cascades;

class ActiveCover : public SceneCover
{
	Q_OBJECT

public:
	ActiveCover(QString m_colorsThumbnailForeground, QString m_colorsThumbnailBackground, QObject *parent = 0);


public slots:
    Q_INVOKABLE void update();

private:
    bb::cascades::Label *m_coverLabel;

    QString m_colorsThumbnailForeground;
    QString m_colorsThumbnailBackground;
};

#endif /* ACTIVECOVER_HPP_ */
