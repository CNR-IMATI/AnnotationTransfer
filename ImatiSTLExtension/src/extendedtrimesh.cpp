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

#include "extendedtrimesh.h"

using namespace IMATI_STL;
using namespace std;

ExtendedTrimesh::ExtendedTrimesh(){}

int ExtendedTrimesh::load(string filename){
    int l_result = TriMesh::load(filename.c_str());

    if(l_result == 0){
        unsigned long i = 0;
        verticesId.clear();
        trianglesId.clear();
        for(Node* n = V.head(); n != nullptr; n = n->next()){
            Vertex* v = static_cast<Vertex*>(n->data);
            idVertices[i] = v;
            verticesId[v] = i++;
        }

        i = 0;

        for(Node* n = T.head(); n != nullptr; n = n->next()){
            Triangle* t = static_cast<Triangle*>(n->data);
            idTriangles[i] = t;
            trianglesId[t] = i++;
        }

    }

    return l_result;
}

IMATI_STL::Vertex* ExtendedTrimesh::getPoint(unsigned long pointID) const{
    return idVertices.at(pointID);
}

Triangle* ExtendedTrimesh::getTriangle(unsigned long triangleID) const{
    return idTriangles.at(triangleID);
}

unsigned long ExtendedTrimesh::getPointId(IMATI_STL::Vertex* v) const{
    return verticesId.at(v);
}

unsigned long ExtendedTrimesh::getTriangleId(Triangle* t) const{
    return trianglesId.at(t);
}

vector<Annotation *> ExtendedTrimesh::getAnnotations() const{
    return this->annotations;
}

void ExtendedTrimesh::setAnnotations(const vector<Annotation *> &value){
    this->annotations = value;
}


void ExtendedTrimesh::addAnnotation(Annotation* annotation){
    this->annotations.push_back(annotation);
}

void ExtendedTrimesh::removeAnnotation(Annotation* annotation){
    std::vector<Annotation*>::iterator ait = std::find(this->annotations.begin(), this->annotations.end(), annotation);
    this->annotations.erase(ait);
}
