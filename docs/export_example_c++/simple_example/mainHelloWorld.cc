// This file is free software; the Free Software Foundation
// gives unlimited permission to copy, distribute and modify it.

#include <stdio.h>
#include "X3dSceneGraph.h"

/* 
   matching white_dune commandline usage:

	dune -prefix Pre_ -c++ file1.x3dv
	dune -prefix Pre2_ -c++ file2.x3dv

*/

void myViewpointDoWithData(Pre_Node *self, void *data) {
    Pre_Viewpoint* viewpoint = (Pre_Viewpoint *) self;
    if (viewpoint)
        printf("%s\n", viewpoint->description);
}  

void myAnchorTreeDoWithData(Pre_Node *self, void *data) {
    Pre_Anchor* anchor = (Pre_Anchor *) self;
    if (anchor)
        printf("Anchor node has %d children\n", anchor->children_length);
}

void myViewpointDoWithData(Pre2_Node *self, void *data) {
    Pre2_Viewpoint* viewpoint = (Pre2_Viewpoint *) self;
    if (viewpoint)
        printf("%s\n", viewpoint->description);
}  

int main(int argc, char** argv) {
    Pre_Viewpoint dummy;
    dummy.doWithDataCallback = &myViewpointDoWithData;
    Pre_Anchor dummy2;
    dummy2.doWithDataCallback = &myAnchorTreeDoWithData;

    Pre_SceneGraph sceneGraph;
    sceneGraph.doWithData(NULL);

    Pre2_SceneGraph sceneGraph2;

    Pre2_Viewpoint dummy3;
    dummy3.doWithDataCallback = &myViewpointDoWithData;

    sceneGraph2.doWithData(NULL);
    return 0;
}
