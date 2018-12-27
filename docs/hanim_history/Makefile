# This file is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

all: gehmess.x3dv
	echo "#X3D V3.0 utf8" > gehmess_proto.x3dv
	echo PROFILE Immersive >> gehmess_proto.x3dv
	echo >> gehmess_proto.x3dv
	echo "# human animation x3dv PROTO based on gehmess.wrl" >> gehmess_proto.x3dv
	echo "#    copyright (c) 1999 by Guido Kramann" >> gehmess_proto.x3dv
	echo "#    This program was developed by Guido Kramann in the DYNAMICUS-project of" >> gehmess_proto.x3dv
	echo "#    the ICA2 at University of Stuttgart in 1999" >> gehmess_proto.x3dv
	echo "#" >> gehmess_proto.x3dv
	echo "#    see also:" >> gehmess_proto.x3dv
	echo "#    http://129.69.35.12/vrml/vrml_dynamikus.html" >> gehmess_proto.x3dv
	echo "#    http://www.csv.ica.uni-stuttgart.de/projects/rsyst/project-dynamikus.html" >> gehmess_proto.x3dv
	echo "#    \"Simulation und Animation natuerlicher Fortbewegungsarten des Menschen unter Verwendung der inversen Dynamik\"" >> gehmess_proto.x3dv
	echo "#    ISSN 0941-4665 Oktober 2000 RUS-48" >> gehmess_proto.x3dv
	echo "#" >> gehmess_proto.x3dv
	echo "#    This program is free software; you can redistribute it and/or modify" >> gehmess_proto.x3dv
	echo "#    it under the terms of the GNU General Public License as published by" >> gehmess_proto.x3dv
	echo "#    the Free Software Foundation; either version 2 of the License, or" >> gehmess_proto.x3dv
	echo "#    (at your option) any later version." >> gehmess_proto.x3dv
	echo "#    This program is distributed in the hope that it will be useful," >> gehmess_proto.x3dv
	echo "#    but WITHOUT ANY WARRANTY; without even the implied warranty of" >> gehmess_proto.x3dv
	echo "#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the" >> gehmess_proto.x3dv
	echo "#    GNU General Public License for more details." >> gehmess_proto.x3dv
	echo "#" >> gehmess_proto.x3dv
	echo "#    You should have received a copy of the GNU General Public License" >> gehmess_proto.x3dv
	echo "#    along with this program; if not, write to the Free Software" >> gehmess_proto.x3dv
	echo "#    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA" >> gehmess_proto.x3dv
	echo "#" >> gehmess_proto.x3dv
	echo "PROTO GEH_MESS [" >> gehmess_proto.x3dv
	echo "  outputOnly SFFloat fraction_changed" >> gehmess_proto.x3dv
	echo "  outputOnly SFRotation vl5_rotation_changed" >> gehmess_proto.x3dv
	echo "  outputOnly SFRotation l_hip_rotation_changed" >> gehmess_proto.x3dv
	echo "  outputOnly SFRotation r_hip_rotation_changed" >> gehmess_proto.x3dv
	echo "  outputOnly SFRotation l_knee_rotation_changed" >> gehmess_proto.x3dv
	echo "  outputOnly SFRotation r_knee_rotation_changed" >> gehmess_proto.x3dv
	echo "  outputOnly SFRotation l_ankle_rotation_changed" >> gehmess_proto.x3dv
	echo "  outputOnly SFRotation r_ankle_rotation_changed" >> gehmess_proto.x3dv
	echo "  outputOnly SFRotation thorax_rotation_changed" >> gehmess_proto.x3dv
	echo "  outputOnly SFRotation l_shoulder_rotation_changed" >> gehmess_proto.x3dv
	echo "  outputOnly SFRotation r_shoulder_rotation_changed" >> gehmess_proto.x3dv
	echo "  outputOnly SFRotation l_elbow_rotation_changed" >> gehmess_proto.x3dv
	echo "  outputOnly SFRotation r_elbow_rotation_changed" >> gehmess_proto.x3dv
	echo "]" >> gehmess_proto.x3dv
	echo "{" >> gehmess_proto.x3dv
	echo "  DEF TIMER TimeSensor {" >> gehmess_proto.x3dv
	echo "     loop TRUE" >> gehmess_proto.x3dv
	echo "     cycleInterval 8" >> gehmess_proto.x3dv
	echo "     fraction_changed IS fraction_changed ">> gehmess_proto.x3dv
	echo "  }" >> gehmess_proto.x3dv
	awk -f build.awk -v name="HUEFTE._R" -v is="vl5_rotation_changed" gehmess.x3dv >> gehmess_proto.x3dv 
	echo >> gehmess_proto.x3dv
	echo "ROUTE TIMER.fraction_changed TO HUEFTE_R.set_fraction" >> gehmess_proto.x3dv
	echo >> gehmess_proto.x3dv
	awk -f build.awk -v name="FEML._R" -v is="l_hip_rotation_changed" gehmess.x3dv >> gehmess_proto.x3dv 
	echo >> gehmess_proto.x3dv
	echo "ROUTE TIMER.fraction_changed TO FEML_R.set_fraction" >> gehmess_proto.x3dv
	echo >> gehmess_proto.x3dv
	awk -f build.awk -v name="FEMR._R" -v is="r_hip_rotation_changed" gehmess.x3dv >> gehmess_proto.x3dv 
	echo >> gehmess_proto.x3dv
	echo "ROUTE TIMER.fraction_changed TO FEMR_R.set_fraction" >> gehmess_proto.x3dv
	echo >> gehmess_proto.x3dv
	awk -f build.awk -v name="CRUL_R" -v is="l_knee_rotation_changed" gehmess.x3dv >> gehmess_proto.x3dv 
	echo >> gehmess_proto.x3dv
	echo "ROUTE TIMER.fraction_changed TO CRUL_R.set_fraction" >> gehmess_proto.x3dv
	echo >> gehmess_proto.x3dv
	awk -f build.awk -v name="CRUR_R" -v is="r_knee_rotation_changed" gehmess.x3dv >> gehmess_proto.x3dv 
	echo >> gehmess_proto.x3dv
	echo "ROUTE TIMER.fraction_changed TO CRUR_R.set_fraction" >> gehmess_proto.x3dv
	echo >> gehmess_proto.x3dv
	awk -f build.awk -v name="PEDR_R" -v is="r_ankle_rotation_changed" gehmess.x3dv >> gehmess_proto.x3dv 
	echo >> gehmess_proto.x3dv
	echo "ROUTE TIMER.fraction_changed TO PEDR_R.set_fraction" >> gehmess_proto.x3dv
	echo >> gehmess_proto.x3dv
	awk -f build.awk -v name="PEDL_R" -v is="l_ankle_rotation_changed" gehmess.x3dv >> gehmess_proto.x3dv 
	echo >> gehmess_proto.x3dv
	echo "ROUTE TIMER.fraction_changed TO PEDL_R.set_fraction" >> gehmess_proto.x3dv
	echo >> gehmess_proto.x3dv
	awk -f build.awk -v name="THO._R" -v is="thorax_rotation_changed" gehmess.x3dv >> gehmess_proto.x3dv 
	echo >> gehmess_proto.x3dv
	echo "ROUTE TIMER.fraction_changed TO THO_R.set_fraction" >> gehmess_proto.x3dv
	echo >> gehmess_proto.x3dv
	awk -f build.awk -v name="BRAL._R" -v is="l_shoulder_rotation_changed" gehmess.x3dv >> gehmess_proto.x3dv 
	echo >> gehmess_proto.x3dv
	echo "ROUTE TIMER.fraction_changed TO BRAL_R.set_fraction" >> gehmess_proto.x3dv
	echo >> gehmess_proto.x3dv
	awk -f build.awk -v name="BRAR._R" -v is="r_shoulder_rotation_changed" gehmess.x3dv >> gehmess_proto.x3dv 
	echo >> gehmess_proto.x3dv
	echo "ROUTE TIMER.fraction_changed TO BRAR_R.set_fraction" >> gehmess_proto.x3dv
	echo >> gehmess_proto.x3dv
	awk -f build.awk -v name="ANTL_R" -v is="l_elbow_rotation_changed" gehmess.x3dv >> gehmess_proto.x3dv 
	echo >> gehmess_proto.x3dv
	echo "ROUTE TIMER.fraction_changed TO ANTL_R.set_fraction" >> gehmess_proto.x3dv
	echo >> gehmess_proto.x3dv
	awk -f build.awk -v name="ANTR_R" -v is="r_elbow_rotation_changed" gehmess.x3dv >> gehmess_proto.x3dv 
	echo >> gehmess_proto.x3dv
	echo "ROUTE TIMER.fraction_changed TO ANTL_R.set_fraction" >> gehmess_proto.x3dv
	echo >> gehmess_proto.x3dv
	echo "}" >> gehmess_proto.x3dv


