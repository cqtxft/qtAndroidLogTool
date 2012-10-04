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
#include <QString>
#include <unistd.h>
#include <QStringList>
#include <QFile>
#include "loginfo.h"
#include "customizeitem.h"

LogInfo* LogInfo::createLogInfo(const QString& optarg)
{
    if ("brief" == optarg)  return  NULL;
    else if ("process" == optarg) return NULL;
    else if ("tag" == optarg) return NULL;
    else if ("thread" == optarg) return NULL;
    else if ("raw" == optarg) return NULL;
    else if ("time" == optarg) return NULL;
    else if ("threadtime" == optarg) return (LogInfo*)new ThreadTime("threadtime");
    else if ("long" == optarg) return NULL;
    else return NULL;
}


const QVector<int>& LogInfo::fields() const
{
    return mFields;
}

int LogInfo::fieldsCount() const
{
    return mFields.count();
}

int LogInfo::fieldPos(int type) const
{
    return mFields.indexOf(type);
}

void LogInfo::setFormatFromString(const QString& optarg)
{
    if ("brief" == optarg) mFormat = FORMAT_BRIEF;
    else if ("process" == optarg) mFormat = FORMAT_PROCESS;
    else if ("tag" == optarg) mFormat = FORMAT_TAG;
    else if ("thread" == optarg) mFormat = FORMAT_THREAD;
    else if ("raw" == optarg) mFormat = FORMAT_RAW;
    else if ("time" == optarg) mFormat = FORMAT_TIME;
    else if ("threadtime" == optarg) mFormat = FORMAT_THREADTIME;
    else if ("long" == optarg) mFormat = FORMAT_LONG;
    else mFormat = FORMAT_OFF;
    return;
}

AndroidLogPrintFormat LogInfo::logFormat() {
    return mFormat;
}

void LogInfo::parseCmdline(const QString& cmdline){

    char* argv[10] = {0};
    QStringList argvList = cmdline.split(' ');
    int argc = argvList.count();
    for(int i=0; i<argc; i++){
        argv[i] =  new char[argvList.at(i).length()];
        strcpy(argv[i], argvList.at(i).toAscii().data());
        QString s = argv[i];
    }

    mFormat = FORMAT_OFF;
    for (;;) {
        int ret;
        ret = getopt(argc, argv, "cdt:gsQf:r::n:v:b:B");
        if (ret < 0)
            break;

        switch(ret) {
            case 'v': {
                setFormatFromString(optarg);
                break;
            }
            default:
                break;
        }
    }
    return;
}

QString LogInfo::fieldName(const int type) const {
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

LogInfo::LogInfo(const QString& format){

    //parseCmdline(cmd);
    setFormatFromString(format);
}

ThreadTime::ThreadTime(const QString& format) : LogInfo(format) {

    mFields.clear();
    mFields.append(TYPE_DATE);
    mFields.append(TYPE_TIME);
    mFields.append(TYPE_PID);
    mFields.append(TYPE_TID);
    mFields.append(TYPE_PRI);
    mFields.append(TYPE_TAG);
    mFields.append(TYPE_MESSAGE);

    mHeaders.clear();
    for (int i=0; i<mFields.count(); i++) {
        mHeaders.append(fieldName(mFields.at(i)));
    }
}

/*
LogInfo::LogInfo(const QString& format){

    //parseCmdline(cmd);
    setFormatFromString(format);

    switch(mFormat) {
        case FORMAT_TIME:
            mFields.clear();
            //mFields.append();
            break;
        case FORMAT_THREADTIME:
        {
            mFields.clear();
            mFields.append(TYPE_DATE);
            mFields.append(TYPE_TIME);
            mFields.append(TYPE_PID);
            mFields.append(TYPE_TID);
            mFields.append(TYPE_PRI);
            mFields.append(TYPE_TAG);
            mFields.append(TYPE_MESSAGE);


            mHeaders.clear();
            for (int i=0; i<mFields.count(); i++) {
                mHeaders.append(fieldName(mFields.at(i)));
            }

            break;
        }
        default:
            break;
    }
}*/

void ThreadTime::parse(QList<CustomizeItem *>& items, const QString& filename) {
    char buf[1024];
    switch(mFormat) {
        case FORMAT_THREADTIME:
        {
            QFile file(filename);
            if (file.open(QFile::ReadOnly)) {

                QList<QStringList> rows = QList<QStringList>();
                AndroidLogEntry* entry = new AndroidLogEntry;
                while (!file.atEnd()) {

                    qint64 lineLength = file.readLine(buf, sizeof(buf));
                    if (lineLength != -1) {

                        char timeBuf[32] = {0};
                        char c;//remove the "."
                        // sscanf's usage is different between qt & android, so I can not directly use the
                        // format in android log module.
                        int ret = 0;
                        if (lineLength != -1)
                           ret = sscanf(buf, "%[^.] %c %03ld %5d %5d %c %[^:]", timeBuf, &c , &(entry->tv_nsec),
                                                               &(entry->pid), &(entry->tid), &(entry->priChar), &(entry->tag));
                        if ((ret != 0) && (ret != 7))
                            continue;

                        QStringList tmp = QString(timeBuf).split(' ', QString::SkipEmptyParts);//get date & time;

                        //tmp.at(1) += QString("%1").arg(entry->tv_nsec);
                        tmp.append(QString("%1").arg(entry->pid));
                        tmp.append(QString("%1").arg(entry->tid));
                        tmp.append(QString(entry->priChar));
                        tmp.append(QString(entry->tag));

                        // the true log message
                        QString line(buf);
                        int pos = line.indexOf(entry->tag) + strlen(entry->tag) + 1;
                        tmp.append(line.mid(pos));
                        rows.append(tmp);
                    }
                }

            // extract all tags
                int tagPos = fieldPos(TYPE_TAG);
                foreach (QStringList row, rows) {
                    int i = 0;
                    foreach (QString text, row) {
                        CustomizeItem* item = new CustomizeItem(text);
                        if(i++ == tagPos){
                            int index = mTags.indexOf(text);
                            if (-1 == index) {
                                mTags.append(text);
                                index = mTags.count() - 1;
                            }
                            item->setTagIndex(index);
                        }
                        items.append(item);
                    }


                }
            }
        }
        default:
            break;
    }
}

/*
 this method is another attempt to parse the file if we do not use the sscanf in qt.
 */
void LogInfo::parse(QList<CustomizeItem *>& items, const QString& filename) {
    char buf[1024];
    switch(mFormat) {
        case FORMAT_THREADTIME:
        {
            QFile file(filename);
            if (file.open(QFile::ReadOnly)) {

                QList<QStringList> rows = QList<QStringList>();

                while (!file.atEnd()) {

                    qint64 lineLength = file.readLine(buf, sizeof(buf));
                    if (lineLength != -1) {
                        QString line(buf);
                        QStringList tmp = line.split(' ', QString::SkipEmptyParts);
                        if (tmp.count() < mFields.count()) {//not a effective log line. output it
                            continue;
                        }
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

            // extract all tags
                foreach (QStringList row, rows) {
                    int i = 0;
                    foreach (QString text, row) {
                        CustomizeItem* item = new CustomizeItem(text);
                        if(i++ == mTagPos){
                            int index = mTags.indexOf(text);
                            if (-1 == index) {
                                mTags.append(text);
                                index = mTags.count() - 1;
                            }
                            item->setTagIndex(index);
                        }
                        items.append(item);
                    }

                }
            }
        }
        default:
            break;
    }
}

