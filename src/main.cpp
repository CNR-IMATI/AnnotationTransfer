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

#include <iostream>
#include <vector>
#include <imatistl.h>
#include <facet.h>
#include <annotation.h>
#include <extendedtrimesh.h>
#include <string>
#include <string.h>

using namespace std;
using namespace IMATI_STL;

vector<Annotation*> readFacets(string filename, ExtendedTrimesh* source);
bool saveFacets(string filename, ExtendedTrimesh* target, vector<Annotation*> annotations);

int main(int argc, char *argv[]){

    ImatiSTL::init();
    ExtendedTrimesh* source = new ExtendedTrimesh();
    ExtendedTrimesh* target = new ExtendedTrimesh();

    if(argc < 2){
        std::cout<<"Missing source filename"<<endl<<flush;
        exit(1);
    }

    if(argc < 3) {
        std::cout<<"Missing target filename"<<endl<<flush;
        exit(3);
    }


    string sourceFilename(argv[1]);
    string targetFilename(argv[2]);
    vector<Annotation*> sourceAnnotations = readFacets(sourceFilename, source);
    vector<Annotation*> targetAnnotations;
    
    target->load(targetFilename);

    for(short m = 2; m <= 2; m++){

        for(vector<Annotation*>::iterator it = sourceAnnotations.begin(); it != sourceAnnotations.end(); it++){
            Annotation* a = static_cast<Annotation*>(*it);
            long int duration;
            auto start = std::chrono::high_resolution_clock::now();
            if(source->V.numels() < target->V.numels() / 5)
                targetAnnotations.push_back(a->parallelTransfer(target));
            else
                targetAnnotations.push_back(a->transfer(target));
            /*double oldArea = a->getArea();
            double newArea = targetAnnotations.back()->getArea();
            double oldPerimeter = a->getPerimeter();
            double newPerimeter = targetAnnotations.back()->getPerimeter();
            double oldP_A = oldPerimeter/oldArea;
            double newP_A = newPerimeter/newArea;
            double oldA_P = oldArea/oldPerimeter;
            double newA_P = newArea/newPerimeter;
            double oldCircularity = pow(oldPerimeter, 2)/oldArea;
            double newCircularity = pow(newPerimeter, 2)/newArea;
            //cout<<"Old area: "<<oldArea<<" New area: "<<newArea<<endl;*/
            auto end = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();



            if(strcmp(argv[argc - 1], "-t") == 0){
                int secs = 0;

                if(duration > 1000){
                    secs = static_cast<int>(duration / 1000);
                    duration = duration % 1000;
                }

                cout << "The trasfer of annotation \""<<a->getTag()<<"\" took " << secs << "." << duration << " seconds" << endl << flush;
            }

            /*cout<<abs((1 - oldArea/newArea)*100)<<"%, ";
            cout<<abs((1 - oldPerimeter/newPerimeter)*100)<<"%, ";
            cout<<abs((1 - oldP_A/newP_A)*100)<<"%, ";
            cout<<abs((1 - oldA_P/newA_P)*100)<<"%, ";
            cout<<abs((1 - oldCircularity/newCircularity)*100)<<"%";
            if(m != 3)
                cout<<", ";*/
            /*if(oldArea < newArea)
                cout<<"The new annotation is "<<(1 - oldArea/newArea)*100 <<"% larger than the original one\n" ;
            else
                cout<<"The new annotation is "<<(1 - newArea/oldArea)*100 <<"% smaller than the original one\n" ;*/
        }

    if(argc > 3 && strcmp(argv[3], "-t") != 0)
        saveFacets(argv[3], target, targetAnnotations);
    else
        saveFacets(targetFilename, target, targetAnnotations);
    }

}


vector<Annotation*> readFacets(string filename, ExtendedTrimesh* source){

    FacetReader fr;
    vector<Annotation*> annotations;
    if(fr.readFct(filename)){
        vector<Facet> facets = fr.getFacets();
        string sourceFilename = fr.getFilename();
        int l_result = source->load(sourceFilename);

        if(l_result == 0){
            for(std::vector<Facet>::iterator it = facets.begin(); it != facets.end(); it++){
                vector<vector<int> > boundaryVertices = it->boundaries;
                vector<int> innerVerticesIndices = it->innerVertices;

                Annotation* a = new Annotation();
                for(vector<vector<int> >::iterator oit = boundaryVertices.begin(); oit != boundaryVertices.end(); oit++){
                    vector<int> facetOutline = static_cast<vector<int> >(*oit);
                    vector<Vertex*> outline;
                    for(unsigned int i = 0; i < facetOutline.size(); i++){
                        Vertex* v = source->getPoint(static_cast<unsigned long>(facetOutline[i]));
                        outline.push_back(v);
                    }
                    Utilities::checkOutlineOrder(innerVerticesIndices, outline, source);
                    a->addOutline(outline);
                }
                u_char color[3];
                string tag;
                switch (it->type) {
                case Facet::FacetType::INTERNAL:
                    color[0] = 255;
                    color[1] = 0;
                    color[2] = 0;
                    tag = "Internal";
                    break;
                case Facet::FacetType::EXTERNAL:
                    color[0] = 0;
                    color[1] = 255;
                    color[2] = 0;
                    tag = "External";
                    break;
                case Facet::FacetType::FRACTURE:
                    color[0] = 0;
                    color[1] = 0;
                    color[2] = 255;
                    tag = "Fracture";
                    break;
                default:
                    color[0] = 0;
                    color[1] = 0;
                    color[2] = 0;
                    tag = "Unknown";
                    break;
                }
                a->setColor(color);
                a->setTag(tag);
                /*a->updateArea();
                a->updatePerimeter();*/
                annotations.push_back(a);
            }

        }

    }

    return annotations;
}

bool saveFacets(string filename, ExtendedTrimesh* target, vector<Annotation*> annotations){

    FacetWriter fw;
    vector<Facet> facets;
    fw.setFilename(filename);
    string facetFilename(filename);
    int pointPos = facetFilename.find_last_of('.');
    facetFilename = facetFilename.substr(0, pointPos);
    facetFilename.append(".fct");
    char id = 'A';
    for(vector<Annotation*>::iterator it = annotations.begin(); it != annotations.end(); it++){
        Facet f;
        Annotation* a = static_cast<Annotation*>( *it);
        vector<vector<Vertex*> > outlines = a->getOutlines();
        if (a->getTag().compare("Internal") == 0)
            f.type = Facet::FacetType::INTERNAL;
        else if(a->getTag().compare("External") == 0)
            f.type = Facet::FacetType::EXTERNAL;
        else if(a->getTag().compare("Fracture") == 0)
            f.type = Facet::FacetType::FRACTURE;
        else
            f.type = Facet::FacetType::UNKNOWN;

        for(vector<vector<Vertex*> >::iterator oit = outlines.begin(); oit != outlines.end(); oit++){
            vector<Vertex*> outline = static_cast<vector<Vertex*> >(*oit);
            vector<int> boundaryVertices;
            for(vector<Vertex*>::iterator bit = outline.begin(); bit != outline.end(); bit++)
                boundaryVertices.push_back(target->getPointId(static_cast<Vertex*>(*bit)));
            f.boundaries.push_back(boundaryVertices);
        }

        vector<Triangle*> innerTriangles = Utilities::regionGrowing(outlines);
        vector<Vertex*> innerVertices;
        Utilities::findPoints(innerVertices, innerTriangles, outlines);
        for(vector<Vertex*>::iterator vit = innerVertices.begin(); vit != innerVertices.end(); vit++)
            f.innerVertices.push_back(target->getPointId(static_cast<Vertex*>(*vit)));
        f.label.push_back(id++);
        facets.push_back(f);
    }
    fw.setFacets(facets);
    return fw.writeFct(facetFilename);

}
