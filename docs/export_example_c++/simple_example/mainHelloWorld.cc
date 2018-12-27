// This file is free software; the Free Software Foundation
// gives unlimited permission to copy, distribute and modify it.

#include <stdio.h>
#include "X3dSceneGraph.h"

/* 
   matching white_dune commandline usage:

	dune -prefix Pre_ -c++ file1.x3dv
	dune -prefix Pre2_ -c++ file2.x3dv

*/

void myTextDoWithData(Pre_Node *self, void *data) {
    Pre_Text* text = (Pre_Text *) self;
    if (text)
        if (text->string_length > 0)
            printf("%s\n", text->string[0]);
}  

void myAnchorTreeDoWithData(Pre_Node *self, void *data) {
    Pre_Anchor* anchor = (Pre_Anchor *) self;
    if (anchor)
        printf("Anchor node has %d children\n", anchor->children_length);
}

void myTextDoWithData(Pre2_Node *self, void *data) {
    Pre2_Text* text = (Pre2_Text *) self;
    if (text)
        if (text->string_length > 0)
            printf("%s\n", text->string[0]);
}  

int main(int argc, char** argv) {
    Pre_Text dummy;
    dummy.doWithDataCallback = &myTextDoWithData;
    Pre_Anchor dummy2;
    dummy2.doWithDataCallback = &myAnchorTreeDoWithData;

    Pre_SceneGraph sceneGraph;
    sceneGraph.doWithData(NULL);

    Pre2_SceneGraph sceneGraph2;

    Pre2_Text dummy3;
    dummy3.doWithDataCallback = &myTextDoWithData;

    sceneGraph2.doWithData(NULL);
    return 0;
}
