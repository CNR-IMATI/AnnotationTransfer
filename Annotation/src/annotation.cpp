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
    sphereRay = targetMesh->bboxLongestDiagonal() / BBOX_SPHERE_RATIO;
    for(Node* n = targetMesh->V.head(); n != nullptr; n=n->next())
        static_cast<Vertex*>(n->data)->info = nullptr;

    for(vector<vector<Vertex*> >::iterator oit = outlines.begin(); oit != outlines.end(); oit++){

        std::vector<Vertex*> otherOutline;              //The outline of the transferred annotation
        bool alreadyUsed = true;
        vector<Vertex*> outline = static_cast<vector<Vertex*> >(*oit);
        std::vector<Vertex*>::iterator vit = outline.begin();
        Vertex* v1, *v2;

        outline.pop_back();

        do{
            v = static_cast<Vertex*>(*vit);
            vector<Vertex*> neighbors = h.getNeighboursInSphere(*v, sphereRay);
            vector<Triangle*> toCheckTriangles;
            Utilities::findFaces(toCheckTriangles, neighbors);
            v1 = Utilities::findCorrespondingVertex(v, toCheckTriangles);
            if(v1 == nullptr)
                sphereRay *= 2;
        }while(v1 == nullptr);
        initialVertex = v1;
        v1->info = &alreadyUsed;

        for(; vit != outline.end(); vit++){
            do{
                v = static_cast<Vertex*>(*vit);
                vector<Vertex*> neighbors = h.getNeighboursInSphere(*v, sphereRay);
                vector<Triangle*> toCheckTriangles;
                Utilities::findFaces(toCheckTriangles, neighbors);
                v2 = Utilities::findCorrespondingVertex(v, toCheckTriangles);
                if(v2 == nullptr)
                    sphereRay *= 2;
            }while(v2 == nullptr);

            if(v2->info == nullptr || !(*static_cast<bool*>(v2->info))){
                vector<Vertex*> path = Utilities::dijkstra(v1, v2, metric, !POST_PROCESSING);
                for (vector<Vertex*>::iterator pit = path.begin(); pit != path.end(); pit++)
                    (*pit)->info = &alreadyUsed;

                otherOutline.insert(otherOutline.end(), path.begin(), path.end());
                v1 = v2;
            }
        }

        v2 = initialVertex;
        initialVertex->info = nullptr;
        vector<Vertex*> path = Utilities::dijkstra(v1, v2, metric, false);
        otherOutline.insert(otherOutline.end(), path.begin(), path.end());
        for(unsigned int i = 0; i < otherOutline.size(); i++)
            otherOutline[i]->info = nullptr;

        while((otherOutline[0] == otherOutline[otherOutline.size() - 1]) && (otherOutline[1] == otherOutline[otherOutline.size() - 2])){
            otherOutline.erase(otherOutline.begin());
            otherOutline.erase(otherOutline.begin() + static_cast<long>(otherOutline.size()) - 1);
        }


        if(POST_PROCESSING){
            v = otherOutline[0];
            otherOutline.erase(otherOutline.begin());
            std::vector<Vertex*> crossedVertices;

            for(vector<Vertex*>::iterator vit1 = otherOutline.begin(); vit1 != otherOutline.end(); vit1++){
                v1 = static_cast<Vertex*>(*vit1);
                if(std::find(crossedVertices.begin(), crossedVertices.end(), v1) == crossedVertices.end())
                    crossedVertices.push_back(v1);
                else
                    for(vector<Vertex*>::iterator vit2 = vit1 - 1; vit2 != otherOutline.begin(); vit2--){
                        v2 = static_cast<Vertex*>(*vit2);

                        if(v2 == v1){
                            otherOutline.erase(vit2, vit1);
                            vit1 = vit2;
                            break;
                        }
                    }
            }

            otherOutline.insert(otherOutline.begin(), v);
        }

        outline.push_back(*outline.begin());
        otherOutline.push_back(*otherOutline.begin());

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
    sphereRay = targetMesh->bboxLongestDiagonal() / BBOX_SPHERE_RATIO;

    for(vector<vector<Vertex*> >::iterator oit = outlines.begin(); oit != outlines.end(); oit++){
        std::vector<Vertex*> otherOutline;              //The outline of the transferred annotation
        bool alreadyUsed = true;
        vector<Vertex*> outline = static_cast<vector<Vertex*> >(*oit);
        std::vector<Vertex*>::iterator vit = outline.begin();
        Vertex* v1, *v2;

        outline.pop_back();

        do{
            v = static_cast<Vertex*>(*vit);
            vector<Vertex*> neighbors = h.getNeighboursInSphere(*v, sphereRay);
            vector<Triangle*> toCheckTriangles;
            Utilities::findFaces(toCheckTriangles, neighbors);
            v1 = Utilities::findCorrespondingVertex(v, toCheckTriangles);
            if(v1 == nullptr)
                sphereRay *= 2;
        }while(v1 == nullptr);
        initialVertex = v1;
        v1->info = &alreadyUsed;

        vector<SPThread*> spTasks;
        vector<vector<Vertex*> *> paths;


        for(; vit != outline.end(); vit++){
            v = static_cast<Vertex*>(*vit);
            vector<Vertex*> neighbors = h.getNeighboursInSphere(*v, sphereRay);
            vector<Triangle*> toCheckTriangles;
            Utilities::findFaces(toCheckTriangles, neighbors);
            v2 = Utilities::findCorrespondingVertex(v, toCheckTriangles);
            if(v2 != nullptr && v2->info == nullptr){
                if(spTasks.size() >= NUM_OF_THREADS){
                    for(uint i = 0; i < NUM_OF_THREADS; i++){
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
        for(uint i = 0; i < spTasks.size(); i++){
            spTasks[i]->waitThread();
            otherOutline.insert(otherOutline.end(), paths[i]->begin(), paths[i]->end());
        }
        v2 = initialVertex;
        vector<Vertex*> path = Utilities::dijkstra(v1, v2, metric, false);
        otherOutline.insert(otherOutline.end(), path.begin(), path.end());

        while((otherOutline[0] == otherOutline[otherOutline.size() - 1]) && (otherOutline[1] == otherOutline[otherOutline.size() - 2])){
            otherOutline.erase(otherOutline.begin());
            otherOutline.erase(otherOutline.begin() + static_cast<long>(otherOutline.size()) - 1);
        }

        v = otherOutline[0];
        otherOutline.erase(otherOutline.begin());
        std::vector<Vertex*> crossedVertices;

        for(vector<Vertex*>::iterator vit1 = otherOutline.begin(); vit1 != otherOutline.end(); vit1++){
            v1 = static_cast<Vertex*>(*vit1);
            if(std::find(crossedVertices.begin(), crossedVertices.end(), v1) == crossedVertices.end())
                crossedVertices.push_back(v1);
            else
                for(vector<Vertex*>::iterator vit2 = vit1 - 1; vit2 != otherOutline.begin(); vit2--){
                    v2 = static_cast<Vertex*>(*vit2);

                    if(v2 == v1){
                        otherOutline.erase(vit2, vit1);
                        vit1 = vit2;
                        break;
                    }
                }
        }

        otherOutline.insert(otherOutline.begin(), v);
        otherOutline.push_back(*otherOutline.begin());
        outline.push_back(*outline.begin());

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
