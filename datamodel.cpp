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
#include <QVector>
#include <QFile>
#include <QStringList>
#include <QApplication>
#include "datamodel.h"
#include "customizeitem.h"
#include <stdio.h>

QString DataModel::fieldName(const int type) const {
    switch (type) {
    case TYPE_DATE:
        return "DATE";
    case TYPE_TIME:
        return "TIME";
    case TYPE_PID:
        return "PID";
    case TYPE_TID:
        return "TID";
    case TYPE_PRI:
        return "PRI";
    case TYPE_TAG:
        return "TAG";
    case TYPE_MESSAGE:
        return "LOG";
    default:
        return "EMPTY";
    }
    return "EMPTY";
}

DataModel::DataModel(int format):
    mLogInfo(NULL) {
}

void DataModel::setFilename(const QString& name) {
    mFilename = name;
}

void DataModel::parse(const QString& name, const QString& cmdline) {

    setFilename(name);

    if (NULL != mLogInfo) {
        delete mLogInfo;
        mLogInfo = NULL;
    }

    mLogInfo = LogInfo::createLogInfo(cmdline);

    CustomizeItem* item;
    if (!mItems.isEmpty()) {
        foreach (item, mItems){
            delete item;
        }
        mItems.clear();
    }

    mLogInfo->parse(mItems, name);
    setHorizontalHeaderLabels(mLogInfo->headers());

    QList<QStandardItem *> items;
    int i = 0;
    foreach (item, mItems) {
        if(i++ < mLogInfo->fieldsCount()) {
            items.append(item);
            continue;
        }
        appendRow(items);
        items.clear();
        items.append(item);
        i = 1;
    }
}

#if 0
/*
void DataModel::parse(const QString& name) {

    setFilename(name);

    if (NULL != mLogInfo) {
        delete mLogInfo;
        mLogInfo = NULL;
    }

    mLogInfo = new LogInfo(name);
    setHorizontalHeaderLabels(mHeaders);
    mLogInfo->parse(mItems, name);
    //headers = mLogInfo->getHeader()

    char buf[1024];
    switch(mFormat) {
        case FORMAT_THREADTIME:
        {
        //int i =0;
        QFile file(mFilename);
        if (file.open(QFile::ReadOnly)) {

            QList<QStringList> rows = QList<QStringList>();

            while (!file.atEnd()) {

                qint64 lineLength = file.readLine(buf, sizeof(buf));
                char timeBuf[32] = {0};
                char c;//remove the "."
                AndroidLogEntry* entry = new AndroidLogEntry;
                // sscanf's usage is different in qt & android, so I can not directly use the
                // format in android log module.
                if (lineLength != -1) {
                    sscanf(buf, "%[^.] %c %03ld %5d %5d %c %s: ", timeBuf, &c , &(entry->tv_nsec),
                                                       &(entry->pid), &(entry->tid), &(entry->priChar), (entry->tag));




                if (lineLength != -1) {
                    QString line(buf);
                    QStringList tmp = line.split(' ', QString::SkipEmptyParts);
                    if (tmp.count() < mFields.count()) {//not a effective log line. output it
                        continue;
                    }
                    else {
                        //model.setHorizontalHeaderLabels(Headers());
                        int last = mFields.count() - 1;
                        QString s;
                        while (last < tmp.count() ) {
                            s += tmp.takeAt(last);
                            s += " ";
                        }

                        tmp.append(s);
                        rows.append(tmp);
                    }
                }
                */
            }

            // extract all tags 
            foreach (QStringList row, rows) {
                QList<QStandardItem *> items;
                int i =0;
                foreach (QString text, row) {
                    CustomizeItem* item = new CustomizeItem(text);
                    if(i++ == mTagPos){
                        int index = tags.indexOf(text);
                        if (-1 == index) {
                            tags.append(text);
                            index = tags.count() - 1;
                        }
                        item->setTagIndex(index);
                    }
                    items.append(item);
                }
                appendRow(items);

            }

        }

       }
    default:
        break;
    }
}
#endif
int DataModel::getFieldType(const QString& type) {

    if(type == "DATE")
        return TYPE_DATE;
    else if (type == "TIME")
        return TYPE_TIME;
    else if (type == "PID")
        return TYPE_PID;
    else if(type == "TID")
        return TYPE_TID;
    else if(type == "PRI")
        return TYPE_PRI;
    else if(type == "TAG")
        return TYPE_TAG;
    else if (type == "LOG")
        return TYPE_MESSAGE;
    else
        return TYPE_OFF;
}

QVariant DataModel::data(const QModelIndex &index, int role) const
{
    switch(role){
    case Qt::DisplayRole:
        return item(index.row(),index.column())->text();
    case Qt::FontRole:
        break;
    case Qt::BackgroundRole:
        for (int i = 0; i < columnCount(); i++) {
           if (dynamic_cast<CustomizeItem *>(item(index.row(), i))->isFound() == 1) {
                QBrush redBackground(Qt::red);
                return redBackground;
           }
        }
        break;
    case Qt::EditRole:
         return item(index.row(),index.column())->text();
         break;
    case Qt::TextAlignmentRole:
        break;
    case Qt::CheckStateRole:
        break;
    default:
        break;
    }
    return QVariant();
}

bool DataModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (role == QueryRole)
    {
        dynamic_cast<CustomizeItem *>(item(index.row(), index.column()))->setFound(1);
        //m_gridData[index.row()][index.column()] = value.toString();
    }
    return true;
}

int DataModel::column(int type)
{
    return mLogInfo->fieldPos(type);
}

const QStringList DataModel::headers()
{
    return mLogInfo->headers();
}

QStringList DataModel::tags()
{
    return mLogInfo->tags();
}

int DataModel::queryCol()
{
    return mLogInfo->fieldPos(TYPE_MESSAGE);
}
