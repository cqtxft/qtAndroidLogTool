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
#ifndef DataModel_H
#define DataModel_H
#include <QVector>
#include <QStringList>
#include <QStandardItemModel>
#include "loginfo.h"
#include "customizeitem.h"



class DataModel : public QStandardItemModel
{
public:
    enum {
        QueryRole = Qt::UserRole + 1,
    };
    DataModel(int format);
    int getFieldType(const QString& type);
    int column(int type);
    bool setData(const QModelIndex & index, const QVariant & value, int role);
    void setFilename(const QString&);
    QString fieldName(const int type) const;
    void parse(const QString& name, const QString&);
    const QStringList headers();
    QStringList tags();
    int queryCol();
private:
    int mFormat;
    QString mFilename;
    QVector<int> mFields;   
    QStringList mHeaders;
    QStringList mTags;
    QList<CustomizeItem *> mItems;
    LogInfo *mLogInfo;
    QVariant data(const QModelIndex &index, int role) const;
   // QString m_gridData[10000][10];  //holds text entered into QTableView
};
#endif // DataModel_H
