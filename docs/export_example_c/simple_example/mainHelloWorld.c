#include <stdio.h>
#include "X3dSceneGraph.h"

/* 
   matching white_dune commandline usage:

	dune -prefix Pre_ -c file1.x3dv 
	dune -prefix Pre2_ -c_ file2.x3dv 

*/

void myViewpointDoWithData(Pre_Node *self, void *data) {
    struct Pre_Viewpoint* viewpoint = (struct Pre_Viewpoint *) self;
    if (viewpoint)
        printf("%s\n", viewpoint->description);
}

void myViewpointDoWithData2(Pre2_Node *self, void *data) {
    struct Pre2_Viewpoint* viewpoint = (struct Pre2_Viewpoint *) self;
    if (viewpoint)
        printf("%s\n", viewpoint->description);
}

void myAnchorTreeDoWithData(Pre_Node *self, void *data) {
    struct Pre_Anchor* anchor = (struct Pre_Anchor *) self;
    if (anchor)
        printf("Anchor node has %d children\n", anchor->children_length);
}

int main(int argc, char** argv) {
    struct Pre_SceneGraph sceneGraph;
    struct Pre2_SceneGraph appendedSceneGraph;

    Pre_ViewpointDoWithDataCallback = &myViewpointDoWithData;
    Pre_AnchorTreeDoWithDataCallback = &myAnchorTreeDoWithData;

    Pre_SceneGraphInit(&sceneGraph);
    Pre_GroupTreeDoWithData(&sceneGraph.root, NULL);

    Pre2_ViewpointDoWithDataCallback = &myViewpointDoWithData2;

    Pre2_SceneGraphInit(&appendedSceneGraph);
    Pre2_GroupTreeDoWithData(&appendedSceneGraph.root, NULL);
    return 0;
}
