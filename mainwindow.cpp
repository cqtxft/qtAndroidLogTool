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
#include <QtGui>
#include <assert.h>
#include "mainwindow.h"
#include "loginfo.h"
#include "datamodel.h"
#include "controller.h"


MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent), mCurrentRow(0), mcnt(0),mDataModel(0)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *SettingLayout = new QHBoxLayout();
    QLabel *fileLabel = new QLabel(
    QApplication::translate("AndroidLogTool", "File Name:"), this);
    mFileEdit = new QLineEdit(this);
    QPushButton * fileButton = new QPushButton("Select", this);

    QLabel *formatLabel = new QLabel(
                QApplication::translate("AndroidLogTool", "Log Format:"), this);

    mformatCombox = new QComboBox(this);
    QStringList format;
    format << "threadtime" << "brief" << "process" << "tag" << "thread" << "raw" << "time" << "long";
    mformatCombox->addItems(format);
    mformatCombox->setEditable(false);

    connect(fileButton, SIGNAL(clicked()), this , SLOT(selectFile()));

    SettingLayout->addWidget(fileLabel);
    SettingLayout->addWidget(mFileEdit);
    SettingLayout->addWidget(fileButton);
    SettingLayout->addWidget(formatLabel);
    SettingLayout->addWidget(mformatCombox);

    QHBoxLayout *queryLayout = new QHBoxLayout();

    QLabel *queryLabel = new QLabel(
    QApplication::translate("AndroidLogTool", "Query:"), this);

    mQueryEdit = new QLineEdit(this);
    QPushButton *findButton = new QPushButton("Find", this);
    QPushButton *nextButton = new QPushButton("Next", this);
    QPushButton *prevButton = new QPushButton("Prev", this);

    queryLayout->addWidget(queryLabel);
    queryLayout->addWidget(mQueryEdit);
    queryLayout->addWidget(findButton);
    queryLayout->addWidget(nextButton);
    queryLayout->addWidget(prevButton);

    connect(findButton, SIGNAL(clicked()), this , SLOT(find()));
    connect(nextButton, SIGNAL(clicked()), this , SLOT(next()));
    connect(prevButton, SIGNAL(clicked()), this , SLOT(prev()));


    mSortCombox = new QComboBox(this);
    sortLayout = new QHBoxLayout();
    sortLayout->addWidget(mSortCombox);


    mSortButton = new QPushButton("filter", this);
    QHBoxLayout *sortButtonLayout = new QHBoxLayout();
    sortButtonLayout->addWidget(mSortButton);

    mainLayout->addLayout(SettingLayout);
    mainLayout->addLayout(queryLayout);
    mainLayout->addLayout(sortLayout);
    mainLayout->addLayout(sortButtonLayout);

    resultView = new QTableView(this);
    mainLayout->addWidget(resultView);

    connect(mSortCombox, SIGNAL(currentIndexChanged(int)), this, SLOT(currentIndexChanged(int)));
    setWindowTitle(
        QApplication::translate("AndroidLogTool", "Android Log Tool"));
    setLayout(mainLayout);

    // Set up the model and configure the view..
    mProxyModel = new ProxyModule(this);
}

class Factory {
public:
        static Controller* createController(int fieldType, QWidget* parent, QHBoxLayout* layout)
        {
            switch (fieldType) {
            case TYPE_DATE:
                return new DateControl(parent, layout);
            case TYPE_TIME:
                return new TimeControl(parent, layout);
            case TYPE_DATETIME:
                return NULL;//DateTimeControl();
            case TYPE_PID:
                return new PidsControl(parent, layout);
            case TYPE_TID:
                return new TidsControl(parent, layout);
            case TYPE_PRI:
                return new PriControl(parent, layout);
            case TYPE_TAG:
                return new TagControl(parent, layout);
            case TYPE_MESSAGE:
                return new LogControl(parent, layout);
            default:
                return NULL;

            }
        }
};

void MainWindow::currentIndexChanged(int index)
{
    // get type of the header col
    // draw the widget per the return type
    // user press button, then filter the data.
    int fieldType = mFieldType;
    mFieldType = mDataModel->getFieldType(mSortCombox->itemText(index));
    if (fieldType == mFieldType)
        return;

    Controller *cnt = mcnt;
    mcnt = Factory::createController(mFieldType, this, sortLayout);

    assert(mcnt != NULL);

    if(TYPE_TAG == mFieldType)
        dynamic_cast<TagControl*> (mcnt)->setTags(mDataModel->tags());

    connect(mSortButton, SIGNAL(clicked()), mcnt, SLOT(filter()));
    connect(mcnt, SIGNAL(setFilter(int, const QString&)), mProxyModel, SLOT(setNewFilter(int, const QString&)));

    if (cnt){
        cnt->hide();
        delete cnt;
        cnt = NULL;
    }

    mcnt->show();

}

void MainWindow::next()
{
    int row = mProxyModel->next(resultView->currentIndex().row(),
                                resultView->currentIndex().column(),
                                mQueryEdit->text());
    if (row < 0)
        return;
    scrollto(row);
}

void MainWindow::prev()
{

    int row = mProxyModel->prev(resultView->currentIndex().row(),
                                resultView->currentIndex().column(),
                                mQueryEdit->text());
    if (row < 0)
        return;
    scrollto(row);
}

void MainWindow::scrollto(int row)
{
    resultView->scrollTo(mProxyModel->index(row, 0));
    resultView->setCurrentIndex((mProxyModel->index(row, 0)));
    QVariant tmp(1);
    //mProxyModel->setData(mProxyModel->index(row, resultView->currentIndex().column()), tmp, DataModel::QueryRole);
    //TODO, the col will be got from proxymodule.
   // int col = resultView->currentIndex().column() >= 0 ? resultView->currentIndex().column() : 0;
    mProxyModel->setData(mProxyModel->index(row, 0), tmp, DataModel::QueryRole);
    for (int i=0; i<mProxyModel->columnCount(); i++) {
        resultView->update(mProxyModel->index(row, i));
    }
}

void MainWindow::find() {


    int row = mProxyModel->next(0,
                            resultView->currentIndex().column(),
                            mQueryEdit->text());
    if (row < 0)
        return;
    scrollto(row);
}

void MainWindow::parseLog(const QString& filename)
{
    if (mDataModel)
        delete mDataModel;

    int type = FORMAT_THREADTIME;
    mDataModel = new DataModel(type);
    // need to start a thread to do this.
    mDataModel->parse(filename, mformatCombox->currentText());
    mProxyModel->setSourceModel(mDataModel);

    QStringList items = mDataModel->headers();
    mSortCombox->clear();
    mSortCombox->addItems(items);//addItems must be call after create ProxyModule, need to find a way to
    //solve it.

    resultView->setModel(mProxyModel);
    resultView->verticalHeader()->hide();
    resultView->horizontalHeader()->setStretchLastSection(true);
}

void MainWindow::selectFile()
{
    connect(this, SIGNAL(sigFileSelected(const QString&)), this, SLOT(parseLog(const QString&)));

    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::AnyFile);
    if(dialog.exec()) {
        QStringList files = dialog.selectedFiles();
        if (files.count() > 0) {
            mFileEdit->setText(files.at(0));//only handle one file.
            emit sigFileSelected(files.at(0));
        }
    }
}

