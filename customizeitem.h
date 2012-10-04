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
#ifndef CUSTOMIZEITEM_H
#define CUSTOMIZEITEM_H

#include <QStandardItem>

class CustomizeItem : public QStandardItem
{
public:
    CustomizeItem (const QString &text);
    void setTagIndex(int index) { mIndex = index;}
    void setFound(int value) { found = value;}
    int  isFound() {return found;}
signals:
    
public slots:
private:
    int mIndex;
    int found;
    
};

#endif // CUSTOMIZEITEM_H
