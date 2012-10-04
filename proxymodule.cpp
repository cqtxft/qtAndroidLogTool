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
#include "proxymodule.h"

void ProxyModule::setNewFilter(int type, const QString& pattern)
{
    int col = dynamic_cast<DataModel*>(sourceModel())->column(type);
    setFilterKeyColumn(col);
    setFilterRegExp(pattern);
}

int ProxyModule::find(int start, int end, int col, const QString& text)
{
    int step = start <= end? 1:-1;
    int i = start;
    while (i != end) {
        QString content = data(index(i, col), Qt::DisplayRole).toString();
        if (content.indexOf(text, 0, Qt::CaseInsensitive)>=0)
            break;
        i += step;
    }
    // check i == end condition
    QString content = data(index(i, col), Qt::DisplayRole).toString();
    if (content.indexOf(text, 0, Qt::CaseInsensitive)>=0)
        return i;
    else //not find
        return -1;
}

int ProxyModule::next(int row, int col, const QString& text)
{
    DataModel* model = dynamic_cast<DataModel*>(sourceModel());
    if (model == NULL)
        return -1;

    int count = rowCount();
    int nextRow = -1;

    if (count < row)
      return nextRow;
    // temporary only check the fixed message field.
    col = model->queryCol();
    //col = col > -1 ? col : dynamic_cast<DataModel*>(sourceModel())->queryCol();
    if (row < count - 1)//current row is the last row
        nextRow = find(row + 1, count - 1, col, text);

    if (nextRow >= 0) {
        return nextRow;
    }

    nextRow = find(0, row, col, text);

    return nextRow;
}

int ProxyModule::prev(int row, int col, const QString& text)
{
    DataModel* model = dynamic_cast<DataModel*>(sourceModel());
    int count = rowCount();
    int prevRow = -1;

    if (model == NULL)
        return -1;
    if (count < row)
      return prevRow;

    // temporary only check the fixed message field.
    col = model->queryCol();
    //col = col > -1 ? col : dynamic_cast<DataModel*>(sourceModel())->queryCol();

    if (row > 0) //current row is not the first row
        prevRow = find(row - 1, 0, col, text);

    if (prevRow >= 0) {
        return prevRow;
    }

    prevRow = find(count - 1, row + 1, col, text);

    return prevRow;
}

