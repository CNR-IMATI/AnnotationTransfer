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

#include "annotation.h"
#include "annotationutilities.h"
#include "nanoflann.hpp"
#include "nanoflannhelper.h"

using namespace std;
using namespace IMATI_STL;

Annotation::Annotation() {
    color[0] = 0;
    color[1] = 0;
    color[2] = 0;
    tag = "";
}

Annotation* Annotation::transfer(ExtendedTrimesh* targetMesh, short metric){

    Annotation* otherAnnotation = new Annotation(); //The transferred annotation
    Vertex* v, *initialVertex;                      //Some support variable

    NanoflannHelper h(targetMesh);
    sphere_ray = targetMesh->bboxLongestDiagonal() / BBOX_SPHERE_RATIO;

    for(vector<vector<Vertex*> >::iterator oit = outlines.begin(); oit != outlines.end(); oit++){

        std::vector<Vertex*> otherOutline;              //The outline of the transferred annotation
        vector<Vertex*> outline = static_cast<vector<Vertex*> >(*oit);
        std::vector<Vertex*>::iterator vit = outline.begin();
        Vertex* v1, *v2;
        do{
            v = static_cast<Vertex*>(*vit);
            vector<Vertex*> neighbors = h.getNeighboursInSphere(*v, sphere_ray);
            vector<Triangle*> toCheckTriangles;
            Utilities::findFaces(toCheckTriangles, neighbors);
            v1 = Utilities::findCorrespondingVertex(v, toCheckTriangles);
            vit++;
        }while(v1 == nullptr && vit != outline.end());
        initialVertex = v1;
        otherOutline.push_back(v1);

        for(; vit != outline.end(); vit++){
            v = static_cast<Vertex*>(*vit);
            vector<Vertex*> neighbors = h.getNeighboursInSphere(*v, sphere_ray);
            vector<Triangle*> toCheckTriangles;
            Utilities::findFaces(toCheckTriangles, neighbors);
            v2 = Utilities::findCorrespondingVertex(v, toCheckTriangles);
            if(v2 != nullptr){
                std::vector<Vertex*> path = Utilities::dijkstra(v1, v2, metric);
                otherOutline.insert(otherOutline.end(), path.begin(), path.end());
                v1 = v2;
            }
        }

        v2 = initialVertex;
        vector<Vertex*> path = Utilities::dijkstra(v1, v2, metric);
        otherOutline.insert(otherOutline.end(), path.begin(), path.end());

        while((otherOutline[0] == otherOutline[otherOutline.size() - 1]) && (otherOutline[1] == otherOutline[otherOutline.size() - 2])){
            otherOutline.erase(otherOutline.begin());
            otherOutline.erase(otherOutline.begin() + static_cast<long>(otherOutline.size()) - 1);
        }

        v = otherOutline[0];
        otherOutline.erase(otherOutline.begin());
        std::vector<Vertex*> crossedVertices;

        for(vit = otherOutline.begin(); vit != otherOutline.end(); vit++){
            v1 = static_cast<Vertex*>(*vit);
            if(std::find(crossedVertices.begin(), crossedVertices.end(), *vit) == crossedVertices.end())
                crossedVertices.push_back(v1);
            else{
                std::vector<Vertex*>::iterator vit1 = vit;
                for(; vit1 != otherOutline.end(); vit1--){
                    v2 = *vit1;
                    if(v2 == v1)
                        break;
                }
                if(v1 == v2)
                    otherOutline.erase(vit,vit1);

            }
        }

        otherOutline.insert(otherOutline.begin(), v);

        otherAnnotation->addOutline(otherOutline);  //The new annotation outline is computed
    }

    //The Outline and inner vertex have been found, the tag and color are the same, so the process ends.
    otherAnnotation->setTag(this->tag);
    otherAnnotation->setColor(this->color);



    return otherAnnotation;

}

Annotation* Annotation::parallelTransfer(ExtendedTrimesh* targetMesh, short metric){

    Annotation* otherAnnotation = new Annotation(); //The transferred annotation
    Vertex* v, *initialVertex;                      //Some support variable

    NanoflannHelper h(targetMesh);
    sphere_ray = targetMesh->bboxLongestDiagonal() / BBOX_SPHERE_RATIO;

    for(vector<vector<Vertex*> >::iterator oit = outlines.begin(); oit != outlines.end(); oit++){
        std::vector<Vertex*> otherOutline;              //The outline of the transferred annotation
        vector<Vertex*> outline = static_cast<vector<Vertex*> >(*oit);
        std::vector<Vertex*>::iterator vit = outline.begin();
        Vertex* v1, *v2;
        do{
            v = static_cast<Vertex*>(*vit);
            vector<Vertex*> neighbors = h.getNeighboursInSphere(*v, sphere_ray);
            vector<Triangle*> toCheckTriangles;
            Utilities::findFaces(toCheckTriangles, neighbors);
            v1 = Utilities::findCorrespondingVertex(v, toCheckTriangles);
            vit++;
        }while(v1 == nullptr && vit != outline.end());
        initialVertex = v1;
        otherOutline.push_back(v1);

        vector<SPThread*> spTasks;
        vector<vector<Vertex*> *> paths;


        unsigned int counter = 0;
        for(; vit != outline.end(); vit++){
            v = static_cast<Vertex*>(*vit);
            counter++;
            cout<<counter * 100 / outline.size()<<"%"<<endl<<flush;
            vector<Vertex*> neighbors = h.getNeighboursInSphere(*v, sphere_ray);
            vector<Triangle*> toCheckTriangles;
            Utilities::findFaces(toCheckTriangles, neighbors);
            v2 = Utilities::findCorrespondingVertex(v, toCheckTriangles);
            if(v2 != nullptr){
                if(spTasks.size() >= NUM_OF_THREADS){
                    for(unsigned int i = 0; i < NUM_OF_THREADS; i++){
                        spTasks[i]->waitThread();
                        otherOutline.insert(otherOutline.end(), paths[i]->begin(), paths[i]->end());
                    }
                    paths.clear();
                    spTasks.clear();
                }
                paths.push_back(new vector<Vertex*>());
                spTasks.push_back(new SPThread(v1, v2, paths[paths.size() - 1], metric));
                spTasks[spTasks.size() - 1]->startThread();
                v1 = v2;
            }
        }
        for(unsigned int i = 0; i < spTasks.size(); i++){
            spTasks[i]->waitThread();
            otherOutline.insert(otherOutline.end(), paths[i]->begin(), paths[i]->end());
        }
        v2 = initialVertex;
        vector<Vertex*> path = Utilities::dijkstra(v1, v2, metric);
        otherOutline.insert(otherOutline.end(), path.begin(), path.end());

        while((otherOutline[0] == otherOutline[otherOutline.size() - 1]) && (otherOutline[1] == otherOutline[otherOutline.size() - 2])){
            otherOutline.erase(otherOutline.begin());
            otherOutline.erase(otherOutline.begin() + static_cast<long>(otherOutline.size()) - 1);
        }

        v = otherOutline[0];
        otherOutline.erase(otherOutline.begin());
        std::vector<Vertex*> crossedVertices;
        for(vit = otherOutline.begin(); vit != otherOutline.end(); vit++){
            v1 = static_cast<Vertex*>(*vit);
            if(std::find(crossedVertices.begin(), crossedVertices.end(), *vit) == crossedVertices.end())
                crossedVertices.push_back(v1);
            else{
                std::vector<Vertex*>::iterator vit1 = vit;
                vit1++;
                for(; vit1 != otherOutline.end(); vit1--){
                    v2 = *vit1;
                    if(v2 == v1)
                        break;
                }
                if(v1 == v2)
                    otherOutline.erase(vit,vit1);

            }
        }
        otherOutline.insert(otherOutline.begin(), v);

        otherAnnotation->addOutline(otherOutline);  //The new annotation outline is computed
    }
    //The Outline and inner vertex have been found, the tag and color are the same, so the process ends.
    otherAnnotation->setTag(this->tag);
    otherAnnotation->setColor(this->color);


    return otherAnnotation;

}

vector<vector<Vertex *> > Annotation::getOutlines() const{
    return outlines;
}

void Annotation::setOutlines(const vector<vector<Vertex *> > value){
    outlines.clear();
    outlines.insert(outlines.end(), value.begin(), value.end());
}

void Annotation::addOutline(const vector<Vertex *> value){
    outlines.push_back(value);
}

unsigned char* Annotation::getColor(){
    return this->color;
}

void Annotation::setColor(unsigned char value[3]){
    this->color[0] = value[0];
    this->color[1] = value[1];
    this->color[2] = value[2];
}

vector<Triangle *> Annotation::getTriangles(){
    vector<Triangle*> annotationTriangles = Utilities::regionGrowing(outlines);

    return annotationTriangles;
}

string Annotation::getTag() const{
    return tag;
}

void Annotation::setTag(string value){
    tag = value;
}
