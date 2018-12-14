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

#include "nanoflannhelper.h"

NanoflannHelper::NanoflannHelper(ExtendedTrimesh* mesh){

    this->mesh = mesh;
    for(Node* n = mesh->V.head(); n != NULL; n = n->next()){
        Vertex* v = (Vertex*) n->data;
        vector<double> point = {v->x, v->y, v->z};
        this->points.push_back(point);
    }

    mat_index = new my_kd_tree_t(3, this->points, 10 /* max leaf */ );
    mat_index->index->buildIndex();
}

vector<Vertex*> NanoflannHelper::getNeighboursInSphere(Point queryPt, double radius){
    double point[3] = {queryPt.x, queryPt.y, queryPt.z};
    const num_t search_radius = static_cast<num_t>(radius);
    std::vector<pair<size_t,num_t> > neighbors_distances;
    vector<Vertex*> neighbors;
    nanoflann::SearchParams params;
    mat_index->index->radiusSearch(point, search_radius, neighbors_distances, params);
    for(vector<pair<size_t,num_t> >::iterator it = neighbors_distances.begin(); it != neighbors_distances.end(); it++){
        pair<size_t,num_t> p = (pair<long int, double>) *it;
        neighbors.push_back(mesh->getPoint(p.first));
    }

    return neighbors;
}
