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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QObject>
#include <QStandardItemModel>

#include "controller.h"
#include "proxymodule.h"

class QLabel;
class QLineEdit;
class QSortFilterProxyModel;
class QHBoxLayout;
class QPushButton;
class QComboBox;
class QTableView;


class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
signals:
    void sigFileSelected(const QString& filename);
    void setFilter(int index, const QString& pattern);
public slots:
    void currentIndexChanged(int);
    void selectFile();
    void parseLog(const QString& filename);
    void next();
    void prev();
    void find();
private:
    int mFieldType;
    int mCurrentRow;
    QLineEdit*   mQueryEdit;
    QLineEdit*   mFileEdit;
    QComboBox*   mSortCombox;
    QTableView*  resultView;
    QHBoxLayout* sortLayout;
    QPushButton* mSortButton;
    Controller*  mcnt;
    ProxyModule* mProxyModel;
    DataModel*   mDataModel;
    QComboBox*   mformatCombox;
    QPushButton* mNextButton;
    QPushButton* mPrevButton;
    void scrollto(int row);
};

#endif // MAINWINDOW_H
