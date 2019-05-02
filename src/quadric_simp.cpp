// adapted to white_dune
/****************************************************************************
 * MeshLab                                                           o o     *
 * A versatile mesh processing toolbox                             o     o   *
 *                                                                _   O  _   *
 * Copyright(C) 2005                                                \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
 * All rights reserved.                                                      *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 2 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 *                                                                           *
 ****************************************************************************/
#include "config.h"
#ifdef HAVE_LIBVCG

#include "quadric_simp.h"
#include "swt.h"

using namespace vcg;
using namespace std;

void QuadricSimplification(CMeshO &m,int targetFaceNum, bool selected, 
                           tri::TriEdgeCollapseQuadricParameter &pp, 
                           CallBackPos *cb)
{
    math::Quadric<double> QZero;
    QZero.SetZero();
    tri::QuadricTemp TD(m.vert,QZero);
    tri::QHelper::TDp()=&TD;

    if (selected) {
        // simplify only inside selected faces
        // select only the vertices having ALL incident faces selected
        tri::UpdateSelection<CMeshO>::VertexFromFaceStrict(m);
  
        // Mark not writable un-selected vertices
        CMeshO::VertexIterator  vi;
        for(vi=m.vert.begin();vi!=m.vert.end();++vi) if(!(*vi).IsD()) {
            if(!(*vi).IsS()) (*vi).ClearW();
                        else (*vi).SetW();
        }
    }
  
    if (pp.PreserveBoundary && !selected) {
        pp.FastPreserveBoundary = true;
        pp.PreserveBoundary = false;
    }

    if (pp.NormalCheck) 
        pp.NormalThrRad = M_PI/4.0;

    m.vert.EnableVFAdjacency();
    m.face.EnableVFAdjacency();
    m.vert.EnableMark();
    m.face.EnableMark();
  
    vcg::LocalOptimization<CMeshO> DeciSession(m,&pp);
    cb(1,"Initializing simplification");
    DeciSession.Init<tri::MyTriEdgeCollapse >();
  
    if (selected)
      targetFaceNum= m.fn - (m.sfn - targetFaceNum);
    DeciSession.SetTargetSimplices(targetFaceNum);
    //  if (TargetError < numeric_limits<double>::max() ) 
    //     DeciSession.SetTargetMetric(TargetError);
    DeciSession.SetTargetMetric(numeric_limits<double>::max());
    int faceToDel = m.fn-targetFaceNum;
    double start = swGetCurrentTime();
    while (DeciSession.DoOptimization() && m.fn > targetFaceNum ) {
        cb(100 - 100 * (m.fn - targetFaceNum) / (faceToDel), "Simplifying...");
        if (swGetCurrentTime() - start > 10.0)
            break;
    };
  
    DeciSession.Finalize<tri::MyTriEdgeCollapse >();
    
    if (selected) {
        // Clear Writable flags
        CMeshO::VertexIterator vi;
        for (vi = m.vert.begin(); vi != m.vert.end(); ++vi) {
            if (!(*vi).IsD()) (*vi).SetW();
            if ((*vi).IsS()) (*vi).ClearS();
        }
    }
}
#endif
