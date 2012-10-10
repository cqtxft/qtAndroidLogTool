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
#include "controller.h"
#include "loginfo.h"
#include "datamodel.h"
#include <assert.h>
const char* Separator = ";";

Controller::Controller(QWidget* parent, QHBoxLayout* layout) :
    QWidget(parent), mLayout(layout)
{
    assert(layout != NULL);
}

Controller::~Controller()
{
}

////////////////////////////////////////////////////////////////////////////////////
DateControl::DateControl(QWidget* parent, QHBoxLayout* layout) :
    Controller(parent, layout)
{
    mDateLabel = new QLabel("Date", this);
    mDateEdit  = new QLineEdit(this);
}

DateControl::~DateControl() {
}

void DateControl::show() {

    mLayout->addWidget(mDateLabel);
    mLayout->addWidget(mDateEdit);
    mDateLabel->show();
    mDateEdit->show();
}

void DateControl::hide() {

    mLayout->removeWidget(mDateLabel);
    mLayout->removeWidget(mDateEdit);
    mDateLabel->hide();
    mDateEdit->hide();
}

void DateControl::filter() {

}

/////////////////////////////////////////////////////////////////////////////////////
TimeControl::TimeControl(QWidget* parent, QHBoxLayout* layout) :
    Controller(parent, layout)
{
    mStartTimeLabel = new QLabel("start time", this);
    mStartTimeEdit  = new QLineEdit(this);
    mEndTimeLabel = new QLabel("end time", this);
    mEndTimeEdit  = new QLineEdit(this);
}

TimeControl::~TimeControl() {
}

void TimeControl::show() {

    mLayout->addWidget(mStartTimeLabel);
    mLayout->addWidget(mStartTimeEdit);
    mLayout->addWidget(mEndTimeLabel);
    mLayout->addWidget(mEndTimeEdit);
    mStartTimeLabel->show();
    mStartTimeEdit->show();
    mEndTimeLabel->show();
    mEndTimeEdit->show();
}

void TimeControl::hide() {
    mLayout->removeWidget(mStartTimeLabel);
    mLayout->removeWidget(mStartTimeEdit);
    mLayout->removeWidget(mEndTimeLabel);
    mLayout->removeWidget(mEndTimeEdit);
    mStartTimeLabel->hide();
    mStartTimeEdit->hide();
    mEndTimeLabel->hide();
    mEndTimeEdit->hide();
}

void TimeControl::filter() {

}
///////////////////////////////////////////////////////////////////////////////////////////

PidsControl::PidsControl(QWidget* parent, QHBoxLayout* layout) :
    Controller(parent, layout)
{
    mPidsLabel = new QLabel("Pids", this);;
    mPidsEdit  = new QLineEdit(this);
    mSelectPidsButton = new QPushButton("Select", this);
}

PidsControl::~PidsControl()
{
}

void PidsControl::show() {
    mLayout->addWidget(mPidsLabel);
    mLayout->addWidget(mPidsEdit);
    mLayout->addWidget(mSelectPidsButton);
    mPidsLabel->show();
    mPidsEdit->show();
    mSelectPidsButton->show();
}

void PidsControl::hide() {
    mLayout->removeWidget(mPidsLabel);
    mLayout->removeWidget(mPidsEdit);
    mLayout->removeWidget(mSelectPidsButton);
    mPidsLabel->hide();
    mPidsEdit->hide();
    mSelectPidsButton->hide();
}

void PidsControl::filter() {
    QString pids = mPidsEdit->text();
    QStringList pidsList = pids.split(Separator);
    QString pattern;
    foreach(QString pid, pidsList){
        if (!pattern.isEmpty())
            pattern = pattern + "|" + pid;
        else
            pattern = pid;
    }
    emit setFilter(TYPE_PID, pattern);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
TidsControl::TidsControl(QWidget* parent, QHBoxLayout* layout) :
    Controller(parent, layout)
{
    mTidsLabel = new QLabel("Tids", this);
    mTidsEdit  = new QLineEdit(this);
    mSelectTidsButton = new QPushButton("Select", this);
}

TidsControl::~TidsControl()
{
}

void TidsControl::show() {

    mLayout->addWidget(mTidsLabel);
    mLayout->addWidget(mTidsEdit);
    mLayout->addWidget(mSelectTidsButton);
    mTidsLabel->show();
    mTidsEdit->show();
    mSelectTidsButton->show();
}

void TidsControl::hide(){
    mLayout->removeWidget(mTidsLabel);
    mLayout->removeWidget(mTidsEdit);
    mLayout->removeWidget(mSelectTidsButton);
    mTidsLabel->hide();
    mTidsEdit->hide();
    mSelectTidsButton->hide();
}

void TidsControl::filter(){
    QString tids = mTidsEdit->text();
    QStringList tidsList = tids.split(Separator);
    QString pattern;
    foreach(QString tid, tidsList){
        if (!pattern.isEmpty())
            pattern = pattern + "|" + tid;
        else
            pattern = tid;
    }
    emit setFilter(TYPE_TID, pattern);
}

///////////////////////////////////////////////////////////////////////////////////////
PriControl::PriControl(QWidget* parent, QHBoxLayout* layout) :
    Controller(parent, layout)
{
    mPriLabel  = new QLabel("Priority", this);
    mPriEdit   = new QLineEdit(this);
    mPriCombox = new QComboBox(this);
    QStringList pris;
    pris << "E" << "W" << "I" << "D" << "V";
    mPriCombox->addItems(pris);
    connect(mPriCombox, SIGNAL(currentIndexChanged(int)), this, SLOT(priIndexChanged(int)));
}

PriControl::~PriControl()
{
}

void PriControl::show() {
    mLayout->addWidget(mPriLabel);
    mLayout->addWidget(mPriEdit);
    mLayout->addWidget(mPriCombox);

    mPriLabel->show();
    mPriEdit->show();
    mPriCombox->show();
}

void PriControl::hide() {
    mLayout->removeWidget(mPriLabel);
    mLayout->removeWidget(mPriEdit);
    mLayout->removeWidget(mPriCombox);

    mPriLabel->hide();
    mPriEdit->hide();
    mPriCombox->hide();
}
void PriControl::filter(){
    QString pris = mPriEdit->text();
    QStringList prisList = pris.split(Separator);
    QString pattern;
    foreach(QString pri, prisList){
        if (!pattern.isEmpty())
            pattern = pattern + "|" + pri;
        else
            pattern = pri;
    }
    emit setFilter(TYPE_PRI, pattern);
}

void PriControl::priIndexChanged(int index) {
    mPriEdit->setText(mPriEdit->text() + Separator + mPriCombox->itemText(index));
}


//////////////////////////////////////////////////////////////////////////////////
TagControl::TagControl(QWidget* parent, QHBoxLayout* layout) :
    Controller(parent, layout)
{
    mTagLabel  = new QLabel("TAG", this);
    mTagCombox = new QComboBox(this);
    mTagsLabel = new QLabel("TAG:", this);
    mTagsEdit  = new QLineEdit(this);
    connect(mTagCombox, SIGNAL(currentIndexChanged(int)), this, SLOT(TagIndexChanged(int)));
}

TagControl::~TagControl()
{
}

void TagControl::show() {

    mLayout->addWidget(mTagLabel);
    mLayout->addWidget(mTagCombox);
    mLayout->addWidget(mTagsLabel);
    mLayout->addWidget(mTagsEdit);

    mTagLabel->show();
    mTagsEdit->show();
    mTagCombox->show();
    mTagsLabel->show();
}

void TagControl::hide() {

    mLayout->removeWidget(mTagLabel);
    mLayout->removeWidget(mTagCombox);
    mLayout->removeWidget(mTagsEdit);
    mLayout->removeWidget(mTagsLabel);

    mTagLabel->hide();
    mTagsEdit->hide();
    mTagsLabel->hide();
    mTagCombox->hide();
}

void TagControl::setTags(const QStringList& tags) {
    mTagCombox->addItems(tags);
}

void TagControl::filter() {

    QString tags = mTagsEdit->text();
    QStringList tagsList = tags.split(Separator);
    QString pattern;
    foreach(QString tag, tagsList){
        if (!pattern.isEmpty())
            pattern = pattern + "|" + tag;
        else
            pattern = tag;
    }
    emit setFilter(TYPE_TAG, pattern);
}

void TagControl::TagIndexChanged(int index) {
    mTagsEdit->setText(mTagsEdit->text() + Separator + mTagCombox->itemText(index));
}

//////////////////////////////////////////////////////////////////////////////////
LogControl::LogControl(QWidget* parent, QHBoxLayout* layout) :
    Controller(parent, layout)
{
    mLogLabel  = new QLabel("Log", this);
    mLogEdit  = new QLineEdit(this);
}

LogControl::~LogControl()
{
}

void LogControl::show() {
    mLayout->addWidget(mLogLabel);
    mLayout->addWidget(mLogEdit);
    mLogLabel->show();
    mLogEdit->show();
}

void LogControl::hide() {
    mLayout->removeWidget(mLogLabel);
    mLayout->removeWidget(mLogEdit);
    mLogLabel->hide();
    mLogEdit->hide();
}

void LogControl::filter() {

}

