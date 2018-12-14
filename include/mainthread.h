/****************************************************************************
*   AnnotationTransfer                                                      *
*   Authors: Andreas Scalas                                                 *
*   Copyright (C) 2018  IMATI-GE / CNR                                      *
*   All rights reserved.                                                    *
*                                                                           *
*   This program is free software: you can redistribute it and/or modify    *
*   it under the terms of the GNU General Public License as published by    *
*   the Free Software Foundation, either version 3 of the License, or       *
*   (at your option) any later version.                                     *
*                                                                           *
*   This program is distributed in the hope that it will be useful,         *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of          *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
*   GNU General Public License for more details.                            *
*                                                                           *
*   You should have received a copy of the GNU General Public License       *
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
****************************************************************************/

#ifndef MAINTHREAD_H
#define MAINTHREAD_H

#include <stdlib.h>
#include <thread>

/**
 * @brief The MainThread class abstract class to define an interface for different types of threads
 */
class MainThread{

    public:
    virtual ~MainThread(){}
        virtual void startThread() = 0;
        virtual void waitThread() = 0;

    protected:
        std::thread* tid;                           //Thread ID
        virtual void executeTask() = 0;
        static void executeTaskHelper(void* context){
            (static_cast<MainThread*>(context))->executeTask();
        }

};


#endif // MAINTHREAD_H
