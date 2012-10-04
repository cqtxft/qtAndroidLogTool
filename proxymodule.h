/*
 * Copyright (C) 2012  cqtxft@gmail.com
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef PROXYMODULE_H
#define PROXYMODULE_H

#include "datamodel.h"
#include <QStandardItemModel>
#include <QSortFilterProxyModel>

/*! \brief This class handle the request of filter and query.
 *
 * This class provides a slot setNewFilter to hand request for filter data
 * and this class also provides three functions to find a row which contains the appointed text.
 * next & prev search the row from different direction. find function search the text from the top
 * of the table to the bottom.
 *
 * @todo turn three functions -- find, prev, next  into slot, and a signal scrollto should be sent to mainwindow
 *       for updating resultView if the text is found.
 */
class ProxyModule : public QSortFilterProxyModel {
    Q_OBJECT

public:
    ProxyModule(QObject * parent = 0) :
        QSortFilterProxyModel(parent){
    }

    ~ProxyModule() {
    }

    int next(int row, int col, const QString& text);
    int prev(int row, int col, const QString& text);
    int find(int start, int end, int col, const QString& text);

public slots:
    void setNewFilter(int type, const QString& pattern);
};


#endif // PROXYMODULE_H
