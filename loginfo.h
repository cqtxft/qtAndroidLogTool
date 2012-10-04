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
#ifndef LOGINFO_H
#define LOGINFO_H

#include <QString>
#include <QVector>
#include <QObject>
#include <QStandardItem>
#include "customizeitem.h"

typedef enum {
    FORMAT_OFF = 0,
    FORMAT_BRIEF,
    FORMAT_PROCESS,
    FORMAT_TAG,
    FORMAT_THREAD,
    FORMAT_RAW,
    FORMAT_TIME,
    FORMAT_THREADTIME,
    FORMAT_LONG,
} AndroidLogPrintFormat;

typedef enum {
    TYPE_OFF = 0,
    TYPE_DATE,
    TYPE_TIME,
    TYPE_DATETIME,
    TYPE_PID,
    TYPE_TID,
    TYPE_PRI,
    TYPE_TAG,
    TYPE_MESSAGE,
    TYPE_MAX = 1000
} AndroidLogFieldType;

typedef struct AndroidLogEntry_t {
    long tv_nsec;
    int pid;
    int tid;
    char priChar;
    char tag[20];
    char message[1000];
} AndroidLogEntry;

class LogInfo : public QObject {
   // Q_OBJECT
public:

    /*! only implement threadtime parser now.
    */
    static LogInfo* createLogInfo(const QString&);
    LogInfo(const QString&);
    virtual ~LogInfo(){}
    /*!
     *@deprecated
     * This function is used to get log format
     * by parse logcat command line like:
     * logcat -v threadtime -f file.txt
     * Now the log format is input by user from UI.
     */
    void parseCmdline(const QString&);
    void setFormatFromString(const QString&);
    AndroidLogPrintFormat logFormat(void);
    QString fieldName(const int)const;
    const QVector<int>& fields() const;
    int fieldsCount() const;
    int fieldPos(int) const;
    virtual void parse(QList<CustomizeItem *>&, const QString&);
    const QStringList& tags() { return mTags; };
    const QStringList& headers() { return mHeaders; };
protected:
    QString mCmd;
    QVector<int> mFields;
    AndroidLogPrintFormat mFormat;
    int mTagPos;
    QStringList mTags;
    QStringList mHeaders;
};

class ThreadTime : public LogInfo
{
//    Q_OBJECT
public:
    ThreadTime(const QString&);
    virtual ~ThreadTime(){}
    void parse(QList<CustomizeItem *>&, const QString&);

};

#endif // LOGINFO_H
