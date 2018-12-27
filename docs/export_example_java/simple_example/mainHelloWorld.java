/* 
   matching white_dune commandline usage:

        dune -prefix Pre_ -java file1.x3dv 
        dune -prefix Pre2_ -java file2.x3dv 

*/

class MyTextCallback extends Pre_TextDoWithDataCallback {
    public void doWithData(Pre_Node node) {
        Pre_Text text = (Pre_Text) node; 
        if ((text.string != null) && (text.string.length > 0))
            System.out.println(text.string[0]);
         if (text.getType() == Pre_TextType.type)
            System.out.println("this is a Text node");
    }   
}

class myAnchorTreeCallback extends Pre_AnchorTreeDoWithDataCallback {
    public void treeDoWithData(Pre_Node node) {
        Pre_Anchor anchor = (Pre_Anchor ) node;
        if (anchor.children != null)
            System.out.println("Anchor node has " + anchor.children.length + " children");
    }
}

class MyTextCallback2 extends Pre2_TextDoWithDataCallback {
    public void doWithData(Pre2_Node node) {
        Pre2_Text text = (Pre2_Text) node; 
        if ((text.string != null) && (text.string.length > 0))
            System.out.println(text.string[0]);
    }   
}


class helloWorld {

    public static void main(String args[]) {
        Pre_Text.setPre_TextDoWithDataCallback(new MyTextCallback());
        Pre_Anchor.setPre_AnchorTreeDoWithDataCallback(new myAnchorTreeCallback());

        Pre_SceneGraph sceneGraph = new Pre_SceneGraph();
        sceneGraph.doWithData();

        Pre2_Text.setPre2_TextDoWithDataCallback(new MyTextCallback2());

        Pre2_SceneGraph sceneGraph2 = new Pre2_SceneGraph();
        sceneGraph2.doWithData();
    }
};

