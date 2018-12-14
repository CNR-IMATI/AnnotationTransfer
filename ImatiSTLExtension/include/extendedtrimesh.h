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

#ifndef EXTENDEDTRIMESH_H
#define EXTENDEDTRIMESH_H

#include <string>
#include <map>
#include <vector>
#include <imatistl.h>
#include <annotation.h>

class Annotation;
class ExtendedTrimesh : public IMATI_STL::TriMesh{

    public:
        ExtendedTrimesh();

        int load(std::string filename);
        IMATI_STL::Vertex* getPoint(unsigned long triangleID) const;
        IMATI_STL::Triangle* getTriangle(unsigned long triangleID) const;
        unsigned long getPointId(IMATI_STL::Vertex* v) const;
        unsigned long getTriangleId(IMATI_STL::Triangle* t) const;
        /**
         * @brief addAnnotation Adds an annotation to the mesh
         * @param annotation the annotation to add
         */
        void addAnnotation(Annotation* annotation);

        /**
         * @brief removeAnnotation Removes an annotation from the mesh
         * @param annotation The annotation to remove
         */
        void removeAnnotation(Annotation* annotation);

        std::vector<Annotation *> getAnnotations() const;

        void setAnnotations(const std::vector<Annotation *> &value);

    protected:

        std::map<IMATI_STL::Vertex*, unsigned long> verticesId;
        std::map<IMATI_STL::Triangle*, unsigned long> trianglesId;
        std::map<unsigned long, IMATI_STL::Vertex*> idVertices;
        std::map<unsigned long, IMATI_STL::Triangle*> idTriangles;
        std::vector<Annotation*> annotations;                       //List of annotations defined on the mesh
};

#endif // EXTENDEDTRIMESH_H
