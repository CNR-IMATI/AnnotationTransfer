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

#ifndef ANNOTATION_H
#define ANNOTATION_H

#include <vector>
#include <queue>
#include <fstream>
#include <string>
#include <sstream>
#include <string>
#include <chrono>
#include <extendedtrimesh.h>
#include <spthread.h>

class ExtendedTrimesh;
/**
 * @brief The Annotation class (for now) manages the area annotations, allowing their storage and
 * their transfer between different representations of the same object.
 */
class Annotation {

    public:
        Annotation();
        /**
         * @brief transfer This method takes the annotations of an object defined on
         * a model with a certain resolution and transfers it to a model with another resolution
         * @param otherMesh The model with lower resolution
         * @param metric the metric to be used for the shortest path
         * @return The annotation defined on the other model.
         */
        Annotation* transfer(ExtendedTrimesh* otherMesh, short metric = 2);

        /**
         * @brief parallelTransfer This method takes the annotations of an object defined on
         * a model with a certain resolution and transfers it to a model with another resolution using threads
         * @param otherMesh The model with lower resolution
         * @param metric the metric to be used for the shortest path
         * @return The annotation defined on the other model.
         */
        Annotation* parallelTransfer(ExtendedTrimesh* otherMesh, short metric = 2);

        //Getter and setter methods
        std::vector<std::vector<IMATI_STL::Vertex *> > getOutlines() const;
        void setOutlines(const std::vector<std::vector<IMATI_STL::Vertex *> > value);
        void addOutline(const std::vector<IMATI_STL::Vertex*> value);
        std::string getTag() const;
        void setTag(std::string value);
        unsigned char* getColor();
        void setColor(unsigned char value[3]);
        std::vector<IMATI_STL::Triangle*> getTriangles();

    private:
        std::vector<std::vector<IMATI_STL::Vertex*> > outlines;      //The outline of the annotated region
        unsigned char color[3];                 //The color associated to the annotation
        std::string tag;                        //The tag of the annotation
        double sphereRay;                       //The radius of the sphere for the neighborhood search
        const short BBOX_SPHERE_RATIO = 1000;   //Divisive coefficient between the BBox longest diagonal and neighborhood sphere radius
        const unsigned short NUM_OF_THREADS = 8;//Number of threads used for the transfer procedure
        const bool ORDER = true;                //Order of the outline: if TRUE then it is counterclockwise, otherwise is clockwise

};

#endif // ANNOTATION_H
