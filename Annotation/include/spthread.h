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

#ifndef SPTHREAD_H
#define SPTHREAD_H

#include <vector>
#include <thread>
#include <mainthread.h>
#include <annotationutilities.h>
#include <iostream>
#include <imatistl.h>


class SPThread : public MainThread{

    private:

        IMATI_STL::Vertex* v1, *v2;        //Vertices between which the path will be computed
        std::vector<IMATI_STL::Vertex*>* path;  //The computed path
        short int metric; //The metric to be used

        /**
         * @brief Method that allows the execution of the main task of the thread
         */
        void executeTask();

    public:

        /**
         * @brief Main constructor of the class. To succesfully start, a thread need a set of information
         * @param v1 First vertex of the path
         * @param v2 Last vertex of the path
         * @param path The computed path
         * @param metric The metric used for weighting the arcs of paths
         */
        SPThread(IMATI_STL::Vertex* v1, IMATI_STL::Vertex* v2, std::vector<IMATI_STL::Vertex*>* path, const short int metric);

        /**
         * @brief method that allows the starting of the thread execution
         */
        void startThread();

        /**
         * @brief Method that allows the request for waiting the end of the thread execution
         */
        void waitThread();

};

#endif // SPTHREAD_H
