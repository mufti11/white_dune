#include <stdio.h>
#include "X3dSceneGraph.h"

/* 
   matching white_dune commandline usage:

	dune -prefix Pre_ -c file1.x3dv 
	dune -prefix Pre2_ -c_ file2.x3dv 

*/

void myTextDoWithData(Pre_Node *self, void *data) {
    struct Pre_Text* text = (struct Pre_Text *) self;
    if (text)
        if (text->string_length > 0)
            printf("%s\n", text->string[0]);
}

void myTextDoWithData2(Pre2_Node *self, void *data) {
    struct Pre2_Text* text = (struct Pre2_Text *) self;
    if (text)
        if (text->string_length > 0)
            printf("%s\n", text->string[0]);
}

void myAnchorTreeDoWithData(Pre_Node *self, void *data) {
    struct Pre_Anchor* anchor = (struct Pre_Anchor *) self;
    if (anchor)
        printf("Anchor node has %d children\n", anchor->children_length);
}

int main(int argc, char** argv) {
    struct Pre_SceneGraph sceneGraph;
    struct Pre2_SceneGraph appendedSceneGraph;

    Pre_TextDoWithDataCallback = &myTextDoWithData;
    Pre_AnchorTreeDoWithDataCallback = &myAnchorTreeDoWithData;

    Pre_SceneGraphInit(&sceneGraph);
    Pre_GroupTreeDoWithData(&sceneGraph.root, NULL);

    Pre2_TextDoWithDataCallback = &myTextDoWithData2;

    Pre2_SceneGraphInit(&appendedSceneGraph);
    Pre2_GroupTreeDoWithData(&appendedSceneGraph.root, NULL);
    return 0;
}
