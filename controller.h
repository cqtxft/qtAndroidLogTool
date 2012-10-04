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
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QLabel>
#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QSortFilterProxyModel>


class Controller : public QWidget
{
    Q_OBJECT
protected:
    QHBoxLayout* mLayout;

public:
    explicit Controller(QWidget *parent = 0, QHBoxLayout* layout = 0);
    virtual  ~Controller();

public slots:
    virtual void show() = 0;
    virtual void hide() = 0;
signals:
     void setFilter(int col, const QString& pattern);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
class DateControl : public Controller {
    Q_OBJECT
public:
    explicit DateControl(QWidget *parent = 0, QHBoxLayout* layout = 0);
    virtual  ~DateControl();
public slots:
    virtual void show();
    virtual void hide();
    virtual void filter();
private:
    QLabel    *mDateLabel;
    QLineEdit *mDateEdit;
};

class TimeControl : public Controller {
    Q_OBJECT
public:
    explicit TimeControl(QWidget *parent = 0, QHBoxLayout* layout = 0);
    virtual  ~TimeControl();
public slots:
    virtual void show();
    virtual void hide();
    virtual void filter();
private:
    QLabel    *mStartTimeLabel;
    QLineEdit *mStartTimeEdit;
    QLabel    *mEndTimeLabel;
    QLineEdit *mEndTimeEdit;
};


class PidsControl : public Controller {
    Q_OBJECT
public:

    explicit PidsControl(QWidget *parent = 0, QHBoxLayout* layout = 0);
    virtual  ~PidsControl();
public slots:
    virtual void show();
    virtual void hide();
    virtual void filter();
private:
    QLabel    *mPidsLabel;
    QLineEdit *mPidsEdit;
    QPushButton* mSelectPidsButton;
};

class TidsControl : public Controller {
    Q_OBJECT
public:
    explicit TidsControl(QWidget *parent = 0, QHBoxLayout* layout = 0);
    virtual  ~TidsControl();
public slots:
    virtual void show();
    virtual void hide();
    virtual void filter();
private:
    QLabel    *mTidsLabel;
    QLineEdit *mTidsEdit;
    QPushButton* mSelectTidsButton;
};

class PriControl : public Controller {
    Q_OBJECT
public:
    explicit PriControl(QWidget *parent = 0, QHBoxLayout* layout = 0);
    virtual  ~PriControl();
signals:
    void setFilter(int col, const QString& pattern);
public slots:
    virtual void show();
    virtual void hide();
    virtual void filter();
public slots:
    void priIndexChanged(int index);
private:
    QLabel    *mPriLabel;
    QLineEdit *mPriEdit;
    QComboBox *mPriCombox;
};

class TagControl : public Controller {
    Q_OBJECT
public:
    explicit TagControl(QWidget *parent = 0, QHBoxLayout* layout = 0);
    virtual  ~TagControl();
    void setTags(const QStringList&);
public slots:
    virtual void show();
    virtual void hide();
    virtual void filter();
public slots:
    void TagIndexChanged(int index);
private:
    QLabel    *mTagLabel;
    QComboBox *mTagCombox;
    QLabel    *mTagsLabel;
    QLineEdit *mTagsEdit;
};

class LogControl : public Controller {
     Q_OBJECT
public:
    explicit LogControl(QWidget *parent = 0, QHBoxLayout* layout = 0);
    virtual  ~LogControl();
public slots:
    virtual void show();
    virtual void hide();
    virtual void filter();
private:
    QLabel    *mLogLabel;
    QLineEdit *mLogEdit;
};


#endif // CONTROLLER_H
