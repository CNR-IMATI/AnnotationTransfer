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

#ifndef NANOFLANNHELPER_H
#define NANOFLANNHELPER_H

#include <vector>
#include <extendedtrimesh.h>
#include <KDTreeVectorOfVectorsAdaptor.h>
#include <nanoflann.hpp>

using namespace IMATI_STL;
using namespace std;

class NanoflannHelper{

public:
    NanoflannHelper(ExtendedTrimesh* mesh);
    vector<Vertex*> getNeighboursInSphere(Point queryPt, double radius);

protected:
    typedef vector<vector<double> > my_vector_of_vectors_t;
    typedef KDTreeVectorOfVectorsAdaptor< my_vector_of_vectors_t, double > my_kd_tree_t;
    typedef double num_t;

    my_vector_of_vectors_t points;
    my_kd_tree_t* mat_index;
    ExtendedTrimesh* mesh;

};

#endif // NANOFLANNHELPER_H
