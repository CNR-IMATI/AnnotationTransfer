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

#include "annotationutilities.h"

using namespace IMATI_STL;
using namespace std;

namespace Utilities{

void findFaces(vector<Triangle*> &faces, vector<Vertex*> vertices){

    //For each vertex, we find the triangle ring and we insert in the array all the faces that aren't in it
    for(unsigned long i = 0; i < vertices.size(); i++){

        for(IMATI_STL::Node* n = vertices[i]->VT()->head(); n != nullptr; n = n->next()){
            IMATI_STL::Triangle* t = static_cast<IMATI_STL::Triangle*>(n->data);

            vector<Triangle*>::iterator it = find(faces.begin(), faces.end(), t);
            if(it == faces.end())
                faces.push_back(t);
        }
    }
}

    void findPoints(vector<Vertex*> &vertices, vector<Triangle*> triangles, vector<vector<Vertex*> > outlines){
        unsigned int BOUNDARY_EDGE = 1;
        unsigned int ALREADY_USED = 9;

        for(vector<vector<Vertex*> >::iterator oit = outlines.begin(); oit != outlines.end(); oit++){
            vector<Vertex*> outline = static_cast<vector<Vertex*> >(*oit);
            for(vector<Vertex*>::iterator vit = outline.begin(); vit != outline.end(); vit++){

                Vertex* v = static_cast<Vertex*>(*vit);
                v->info = &BOUNDARY_EDGE;

            }
        }

        for(unsigned int i = 0; i < triangles.size(); i++){
            Vertex* v = triangles[i]->v1();
            for(unsigned int j = 0; j < 3; j++){
                v = triangles[i]->nextVertex(v);
                if(v->info == nullptr || (*static_cast<unsigned int*>(v->info) != ALREADY_USED && *static_cast<unsigned int*>(v->info) != BOUNDARY_EDGE)){
                    vertices.push_back(v);
                    v->info = &ALREADY_USED;
                }
            }
        }

        for(unsigned int i = 0; i < triangles.size(); i++){
            Vertex* v = triangles[i]->v1();
            for(int j = 0; j < 3; j++){
                v = triangles[i]->nextVertex(v);
                v->info = nullptr;
            }
        }
    }

    void checkOutlineOrder(vector<int> innerVerticesIndices, vector<Vertex *> &outline, ExtendedTrimesh* model){
        int IS_POINTER = 19;
        int IS_POINTED = 20;
        vector<Triangle*> pointedTriangles;
        vector<Vertex*> innerVertices;

        for(vector<int>::iterator vit = innerVerticesIndices.begin(); vit != innerVerticesIndices.end(); vit++)
            innerVertices.push_back(model->getPoint(static_cast<int>(*vit)));

        for(vector<Vertex*>::iterator vit = innerVertices.begin(); vit != innerVertices.end(); vit++)
            (*vit)->info = &IS_POINTER;

        for(vector<Vertex*>::iterator vit = outline.begin(); vit != outline.end(); vit++)
            (*vit)->info = &IS_POINTER;

        for(vector<Vertex*>::iterator vit = innerVertices.begin(); vit != innerVertices.end(); vit++){
            Vertex* v = static_cast<Vertex*>(*vit);
            for(Node* n = v->VT()->head(); n != nullptr; n = n->next()){
                Triangle* t = static_cast<Triangle*>(n->data);
                if( t->v1()->info != nullptr && *static_cast<int*>(t->v1()->info) == IS_POINTER &&
                    t->v2()->info != nullptr && *static_cast<int*>(t->v2()->info) == IS_POINTER &&
                    t->v3()->info != nullptr && *static_cast<int*>(t->v3()->info) == IS_POINTER &&
                    (t->info == nullptr || *static_cast<int*>(t->info) != IS_POINTED)){
                    t->info = &IS_POINTED;
                    pointedTriangles.push_back(t);
                }
            }
        }

        Triangle* as = outline[3]->getEdge(outline[4])->leftTriangle(outline[3]);
        if(as->info == nullptr || *static_cast<int*>(as->info) != IS_POINTED)
            std::reverse(outline.begin(), outline.end());

        for(vector<Vertex*>::iterator vit = innerVertices.begin(); vit != innerVertices.end(); vit++)
            (*vit)->info = nullptr;
        for(vector<Vertex*>::iterator vit = outline.begin(); vit != outline.end(); vit++)
            (*vit)->info = nullptr;
        for(vector<Triangle*>::iterator tit = pointedTriangles.begin(); tit != pointedTriangles.end(); tit++)
            (*tit)->info = nullptr;

    }

    bool isPointInsideTriangle(Point checkPoint, Point trianglePoint1, Point trianglePoint2, Point trianglePoint3){
        Point AB = trianglePoint2 - trianglePoint1;
        Point AC = trianglePoint3 - trianglePoint1;
        Point PA = trianglePoint1 - checkPoint;
        Point PB = trianglePoint2 - checkPoint;
        Point PC = trianglePoint3 - checkPoint;
        double areaABC = (AB & AC).length() / 2.0;
        long double alfa = static_cast<long double>((PB & PC).length() / (2.0 * areaABC));
        long double beta = static_cast<long double>((PC & PA).length() / (2.0 * areaABC));
        long double gamma = static_cast<long double>((PA & PB).length() / (2.0 * areaABC));
        long double aboutOne = static_cast<long double>(1.0 + EPSILON_BC);
        if((alfa >= 0 && alfa <= aboutOne) && (beta >= 0 && beta <= aboutOne) &&
           (gamma >= 0 && gamma <= aboutOne) && (alfa + beta + gamma <= aboutOne))
            return true;
        else
            return false;
    }

    Vertex* extractNearestVertex(vector<Vertex*> &frontier, map<Vertex*, double> distances){

        double minDist = DBL_MAX;
        int minPos = -1;
        for(unsigned int i = 0; i < frontier.size(); i++){
            if(distances[frontier[i]] < minDist){
                minPos = i;
                minDist = distances[frontier[i]];
            }
        }

        Vertex* nearest = frontier[minPos];
        frontier.erase(frontier.begin() + minPos);
        return nearest;

    }

    vector<Vertex*> dijkstra(Vertex* v1, Vertex* v2, const short int metric, const bool avoidUsed){

        vector<Vertex*> frontier;
        map<Vertex*, double> distances = {{v1, 0}};
        map<Vertex*, Vertex*> predecessors = {{v1, nullptr}};
        set<Vertex*> v21RingNeighbors;
        vector<Vertex*> shortestPath;
        Vertex* v;
        bool v2visited = false;

        if(((*v1) - (*v2)).length() == 0.0){
            return shortestPath;
        }

        frontier.push_back(v1);

        do{
            v = extractNearestVertex(frontier, distances);

            List* neighbors = v->VV();
            for(Node* n = neighbors->head(); n != nullptr; n = n->next()){
                Vertex* x = static_cast<Vertex*>(n->data);

                if(x == v2 && v21RingNeighbors.find(v) == v21RingNeighbors.end())
                    v21RingNeighbors.insert(v);

                map<Vertex*, Vertex*>::iterator pit = predecessors.find(x);
                double distanceVX;
                if(!avoidUsed || x->info == nullptr || !(*static_cast<bool*>(x->info))){
                    switch(metric){
                        case SEGMENT_DISTANCE:
                            distanceVX = distances[v] + x->distanceFromEdge(v1,v2);
                            break;
                        case COMBINED_DISTANCE:
                            distanceVX = distances[v] + (*x-*v).length() + x->distanceFromEdge(v1,v2);
                            break;
                        default:
                            distanceVX = distances[v] + (*x-*v).length();
                    }
                }else
                    distanceVX = DBL_MAX;

                if(pit != predecessors.end()){
                    if(distances[x] > distanceVX){
                        distances[x] = distanceVX;
                        predecessors[x] = v;
                    }
                } else {
                    distances.insert(std::make_pair(x, distanceVX));
                    predecessors.insert(std::make_pair(x, v));
                    frontier.push_back(x);
                }
            }
            if(v == v2)
                v2visited = true;

        } while(!v2visited);

        shortestPath.push_back(v2);
        v = predecessors[v2];
        while(v != v1){
            shortestPath.insert(shortestPath.begin(), v);
            v = predecessors[v];
        }

        return shortestPath;
    }

    vector<Triangle*> regionGrowing(vector<vector<Vertex*> > contours){

        queue<Triangle*> neighbors;
        vector<Triangle*> internalTriangles;
        unsigned int BOUNDARY_EDGE = 1;
        unsigned int ALREADY_USED = 9;

        for(vector<vector<Vertex*> >::iterator oit = contours.begin(); oit != contours.end(); oit++){
            vector<Vertex*> contour = static_cast<vector<Vertex*> >(*oit);
            neighbors.push(contour[0]->getEdge(contour[1])->leftTriangle(contour[0]));
            for(unsigned int i = 1; i <= contour.size(); i++){

                Vertex* v1, * v2;
                if(i < contour.size()){
                    v1 = contour[i - 1];
                    v2 = contour[i];
                } else {
                    v1 = contour[i - 1];
                    v2 = contour[0];
                }

                Edge* e = v1->getEdge(v2);
                if(e != nullptr)
                    e->info = static_cast<void*>(&BOUNDARY_EDGE);
            }
        }

        while(neighbors.size() > 0){
            Triangle* t = neighbors.front();
            neighbors.pop();
            Edge* e = t->e1;
            for(int i = 0; i < 3; i++){
                if(!(e->info != nullptr && *static_cast<unsigned int*>(e->info) == BOUNDARY_EDGE)){
                    Triangle* t_ = e->oppositeTriangle(t);
                    if(t_->info == nullptr ||  *static_cast<unsigned int*>(t_->info) != ALREADY_USED){
                        internalTriangles.push_back(t_);
                        t_->info = static_cast<void*>(&ALREADY_USED);
                        neighbors.push(t_);
                    }
                }
                e = t->nextEdge(e);
            }
        }

        for(vector<vector<Vertex*> >::iterator oit = contours.begin(); oit != contours.end(); oit++){
            vector<Vertex*> contour = static_cast<vector<Vertex*> >(*oit);
            for(unsigned int i = 1; i <= contour.size(); i++){

                Vertex* v1, *v2;
                if(i < contour.size()){
                    v1 = contour[i - 1];
                    v2 = contour[i];
                }else{
                    v1 = contour[i - 1];
                    v2 = contour[0];
                }

                Edge* e = v1->getEdge(v2);
                if(e != nullptr)
                    e->info = nullptr;
            }
        }

        for(std::vector<Triangle*>::iterator it = internalTriangles.begin(); it != internalTriangles.end(); it++)
            (*it)->info = nullptr;


        return internalTriangles;

    }

    Vertex* findCorrespondingVertex(Vertex* v, vector<Triangle*> neighbors){

        double bestDistance = DBL_MAX;
        Vertex* r = nullptr;
        Triangle* t = findCorrespondingTriangle(v, neighbors);
        if(t != nullptr){
            Vertex* tv1 = t->v1(), *tv2 = t->v2(), *tv3 = t->v3();
            Point p = Point::linePlaneIntersection(*v, (*v) + v->getNormal(), *tv1, *tv2, *tv3);
            Vertex* v_ = t->v1();

            for(int i = 0; i < 3; i++){
                double actualDistance = ((p)-(*v_)).length();
                if(actualDistance < bestDistance){
                    bestDistance = actualDistance;
                    r = v_;
                }
                v_ = t->nextVertex(v_);
            }

        }
        return r;

    }

    Triangle* findCorrespondingTriangle(Vertex* v, vector<Triangle*> neighbors){

        bool found = false;
        double bestDistance = DBL_MAX;
        Triangle* t = nullptr;
        for(vector<Triangle*>::iterator it = neighbors.begin(); it != neighbors.end(); it++){
            Triangle* t_ = static_cast<Triangle*>(*it) ;
            double normalProduct = t_->getNormal() * v->getNormal();
            if(normalProduct >= 0){
                Vertex* tv1 = t_->v1(), *tv2 = t_->v2(), *tv3 = t_->v3();
                Point p = Point::linePlaneIntersection(*v, (*v) + v->getNormal(), *tv1, *tv2, *tv3);
                if(isPointInsideTriangle(p, *tv1, *tv2, *tv3)){
                    double actualDistance = t_->pointTriangleDistance(v);
                    if(!found){
                        t = t_;
                        bestDistance = actualDistance;
                        found = true;
                    } else if(actualDistance < bestDistance){
                        t = t_;
                        bestDistance = actualDistance;
                    }
                }
            }
        }

        return t;
    }

    vector<vector<Vertex *> > getOutlines(vector<Triangle *> set){

        vector<std::pair<Vertex*, Vertex*> > setOutlineEdges;
        vector<vector<Vertex*> > outlines;
        Vertex* v, *v_;
        int IS_INSIDE = 738;

        for(std::vector<Triangle*>::iterator tit = set.begin(); tit != set.end(); tit++)
            (*tit)->info = &IS_INSIDE;

        for(std::vector<Triangle*>::iterator tit = set.begin(); tit != set.end(); tit++){
            Triangle* t = static_cast<Triangle*>(*tit);
            Edge* e = t->e1;
            for(int i = 0; i < 3; i++){
                Triangle* t_ = e->oppositeTriangle(t);
                if(t_->info == nullptr || *static_cast<int*>(t_->info) != IS_INSIDE){
                    Edge* e_ = t->prevEdge(e);
                    v = e_->commonVertex(e);
                    setOutlineEdges.push_back(std::make_pair(v, e->oppositeVertex(v)));
                }
                e = t->nextEdge(e);
            }
        }

        while(setOutlineEdges.size() != 0){

            vector<Vertex*> outline;
            v = setOutlineEdges[0].first;
            Vertex *initialVertex = v;

            std::pair<Vertex*, Vertex*> pPrev = std::make_pair(nullptr, nullptr);
            do{
                outline.push_back(v);
                for(Node *n = v->VV()->head(); n != nullptr; n = n->next()){
                    v_ = static_cast<Vertex*>(n->data);
                    std::pair<Vertex*, Vertex*> p = std::make_pair(v, v_);
                    for(vector<pair<Vertex*, Vertex*> >::iterator pit = setOutlineEdges.begin(); pit != setOutlineEdges.end(); pit++){
                        pair<Vertex*, Vertex*> tmp = static_cast<pair<Vertex*, Vertex*> >(*pit);
                        if(p != pPrev &&
                           ((p.first == tmp.second && p.second == tmp.first) ||
                           (p.first == tmp.second && p.second == tmp.first)) ){
                            v = v_;
                            pPrev = p;
                            setOutlineEdges.erase(pit);
                            break;
                        }
                    }

                    if(v == v_)
                        break;
                }
            }while(v != initialVertex);
            outline.push_back(outline[0]);
            outlines.push_back(outline);
        }

        for(vector<vector<Vertex*> >::iterator oit = outlines.begin(); oit != outlines.end(); oit++){
            vector<Vertex*> outline = static_cast<vector<Vertex*> >(*oit);
            Vertex* v1 = outline[0], *v2 = outline[1];
            Triangle* t = v1->getEdge(v2)->leftTriangle(v1);
            if(std::find(set.begin(), set.end(), t) == set.end())
                std::reverse(oit->begin(), oit->end());
        }

        for(std::vector<Triangle*>::iterator tit = set.begin(); tit != set.end(); tit++){
            (*tit)->info = nullptr;
        }

        return outlines;

    }



}
