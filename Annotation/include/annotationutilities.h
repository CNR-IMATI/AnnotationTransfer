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

#ifndef ANNOTATION_UTILITIES_H
#define ANNOTATION_UTILITIES_H

#include <vector>
#include <queue>
#include <set>
#include <map>
#include <math.h>
#include <iostream>
#include <cstdio>
#include <ctime>
#include <algorithm>
#include <extendedtrimesh.h>

class ExtendedTrimesh;
namespace Utilities {

    const double EPSILON_BC = 0.00005;
    const double EPSILON_D = 0.01;
    const short int SEGMENT_DISTANCE = 1;
    const short int EUCLIDEAN_DISTANCE = 2;
    const short int COMBINED_DISTANCE = 3;

    void findFaces(std::vector<IMATI_STL::Triangle*> &faces, std::vector<IMATI_STL::Vertex*> vertices);
    void findPoints(std::vector<IMATI_STL::Vertex*> &vertices, std::vector<IMATI_STL::Triangle*> triangles, std::vector<std::vector<IMATI_STL::Vertex*> > outlines);
    void checkOutlineOrder(std::vector<int> innerVerticesIndices, std::vector<IMATI_STL::Vertex *> &outline, ExtendedTrimesh* model);
    bool isPointInsideTriangle(IMATI_STL::Point checkPoint, IMATI_STL::Point trianglePoint1, IMATI_STL::Point trianglePoint2, IMATI_STL::Point trianglePoint3);
    std::vector<IMATI_STL::Vertex*> dijkstra(IMATI_STL::Vertex* v1, IMATI_STL::Vertex* v2, const short int metric);
    std::vector<IMATI_STL::Triangle*> regionGrowing(std::vector<std::vector<IMATI_STL::Vertex*> > contours);
    IMATI_STL::Vertex* findCorrespondingVertex(IMATI_STL::Vertex* v, std::vector<IMATI_STL::Triangle*> otherMesh);
    IMATI_STL::Triangle* findCorrespondingTriangle(IMATI_STL::Vertex* v, std::vector<IMATI_STL::Triangle*> otherMesh);
    std::vector<std::vector<IMATI_STL::Vertex *> > getOutlines(std::vector<IMATI_STL::Triangle *> set);
}

#endif
