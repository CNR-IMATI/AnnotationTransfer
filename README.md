ANNOTATION TRANSFER TOOL

1 GENERAL INFORMATION

1.1 Release notes
• Release 1.0 has been developed within the GRAVITATE project, funded by the European Union’s Horizon 2020 research and innovation programme under grant agreement 665155.
• IP and usage rights are described in the related section of this document.

1.2 System Overview
The “Annotation Transfer” tool contains a routine for transferring the annotations’ shape between the different resolutions of the same 3D virtual replica. 
This tool contains algorithms for projection of the annotations’ boundary onto the different 3D models (with different resolutions) representing the same artefact.
Beyond GRAVITATE, where the tool is used for obtaining the faceting of the greater than 50K vertices resolutions models, this software component can be extended for the transfer of any kind of annotation (it requires the development of the appropriate parser).
Include the link for the 
• User Manual download (this document)
• https://gitlab.it-innovation.soton.ac.uk/ascalas/facettransfer.git
	
1.3 Authorized Use Permission
The software related to this manual is licensed under the GNU GPL v3.
IN NO EVENT SHALL THE AUTHORS OR DISTRIBUTORS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION, OR ANY DERIVATIVES THEREOF, EVEN IF THE AUTHORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE AUTHORS AND DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.  THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, AND THE AUTHORS AND DISTRIBUTORS HAVE NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
The AnnotationTransfer module and the information contained in AnnotationTransfer including, but not limited to, the ideas, concepts and know-how are proprietary, confidential and trade secret to:
	
IMATI-GE / CNR
Consiglio Nazionale delle Ricerche
Istituto di Matematica Applicata e Tecnologie Informatiche “E. Magenes”, Sezione di Genova
Via de Marini 6, Torre di Francia, 16149, Genova - ITALY

1.4 Points of Contact and Support
The AnnotationTransfer tool has been developed by IMATI-GE.

IMATI-GE / CNR
Consiglio Nazionale delle Ricerche
Istituto di Matematica Applicata e Tecnologie Informatiche “E. Magenes”, Sezione di Genova
Via de Marini 6, Torre di Francia, 16149, Genova – ITALY

Author of the software is: Andreas Scalas
Contact email: andreas.scalas@ge.imati.cnr.it
Type of support: Best effort

1.5 Organization of the Manual
The present manual is divided in three parts:
    • Getting started (Section 3)
    • Deployment instructions (Section 4)
    • System usage (Section 5)

2 Getting Started

2.1 System prerequisites and software download
The following instructions need to be followed to prepare your system for installing the GRAVITATE software.

2.1.1 Hardware requisites 
n/a

2.1.2 3rd party software necessary to install the module
• ImatiSTL 4.2 or higher: https://sourceforge.net/projects/imatistl/ (GNU GPL v3)
• nanoflann: https://github.com/jlblancoc/nanoflann.git (BSD)

2.1.3 Binary Distributions
n/a

2.1.4 Code distribution
https://gitlab.it-innovation.soton.ac.uk/ascalas/facettransfer.git

2.2 Credential and software setup
Credentials are not needed.
The only requirement for system setup is to have C++11 or higher installed in the system.

3 Deployment Instruction
To build the source code the user needs C++11 or higher and cmake to be installed in the system. After the download of ImatiSTL and nanoflann, the user has to unzip the archives in the main folder of the AnnotationTranfer tool.
Then, the user has to perform the following calls in the system shell:

~AnnotationTransfer_path/cmake> cmake CMakeLists.txt
~AnnotationTransfer_path/cmake> make

4 System Usage

4.1 AnnotationTranfer 
This library is designed to be applied to a collection of triangle meshes. The routine assumes that the triangle meshes have a single connected component and are manifold. 

4.1.1 Data Flows
• Input: a faceting in FCT file format file, the corresponding source triangle mesh and a target triangle mesh. Triangle meshes must be in one of the following file formats:
	◦ IV
	◦ VRML 1.0
	◦ VRML 2.0
	◦ OFF
	◦ EFF
	◦ PLY
	◦ VER-TRI
	◦ OBJ
	◦ STL
• Output: a faceting in FCT file format. 

4.1.2 Annotations properties
The FCT files should contain the exact name of the source mesh file and the source mesh file should be in the same folder of both the FCT file and the target mesh file. 
As it is, the tool assumes that annotations are composed of:
• An informative part (text, tag, etc.)
• A geometric selector
This tool ignores the informative part of the annotations and requires that the selector is defined as follows:
1. The selected geometry is a collection of connected components defined as sets of triangles
2. Each set of triangles have a single outer boundary and 0 or more inner boundaries (holes)
The target mesh needs to be registered to the source mesh (same position/orientation).

4.1.3 Usage
The tool can be launched with the following system shell command:

~AnnotationTransfer_path/build_whatsoever/> ./AnnotationTransfer source_faceting.fct target_mesh.ext

where “faceting_path” is the path to the source faceting and “target” that of the target mesh. Notice that meshformat should be one of the following:
• .iv
• .wrl
• .off
• .eff
• .ply
• .tri
• .obj
• .stl
This call requires that the input source mesh is in the same folder of “faceting_path.fct”.

4.1.4 Running the Annotation Transfer executable on a dataset.
This is an example of bash script. We assume that the faceting files and the corresponding source and target meshes are contained in the subfolder of the calling folder, namely /starting_path. So, the bash script expects the folder to be structured as:
/starting_path
• .
• ..
• object_id_1/
• object_id_2/
• ⁝
• object_id_n/
• AnnotationTransfer
• script.sh

The following is the corresponding bash code:

#!/bin/bash
IFS=$'\n'
a=$(ls -d */ )
for b in $a
do
	cd $b
	c=$(ls -I "*50K*" -I "*.fct")
	d=$(ls *50K.fct 2>/dev/null)
	echo -n ${d%"_50K.fct"}
	if [ "$d" != "" ]; then
		echo -n ", "
	fi
	for e in $c
	do
		if [ "$d" != "" ]; then
			./../AnnotationTransfer $d $e 
			echo -n ", "
		fi
	done
	if [ "$d" != "" ]; then
		echo ""
	fi
	cd ..
done
