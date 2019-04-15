/* 
   matching white_dune commandline usage:

        dune -prefix Pre_ -java file1.x3dv 
        dune -prefix Pre2_ -java file2.x3dv 

*/

class MyViewpointCallback extends Pre_ViewpointDoWithDataCallback {
    public void doWithData(Pre_Node node) {
        Pre_Viewpoint viewpoint = (Pre_Viewpoint) node; 
        if (viewpoint.description != null)
            System.out.println(viewpoint.description);
         if (viewpoint.getType() == Pre_ViewpointType.type)
            System.out.println("this is a Viewpoint node");
    }   
}

class myAnchorTreeCallback extends Pre_AnchorTreeDoWithDataCallback {
    public void treeDoWithData(Pre_Node node) {
        Pre_Anchor anchor = (Pre_Anchor ) node;
        if (anchor.children != null)
            System.out.println("Anchor node has " + anchor.children.length + " children");
    }
}

class MyViewpointCallback2 extends Pre2_ViewpointDoWithDataCallback {
    public void doWithData(Pre2_Node node) {
        Pre2_Viewpoint viewpoint = (Pre2_Viewpoint) node; 
        if (viewpoint.description != null)
            System.out.println(viewpoint.description);
    }   
}

class helloWorld {

    public static void main(String args[]) {
        Pre_Anchor.setPre_AnchorTreeDoWithDataCallback(new myAnchorTreeCallback());
        Pre_Viewpoint.setPre_ViewpointDoWithDataCallback(new MyViewpointCallback());

        Pre_SceneGraph sceneGraph = new Pre_SceneGraph();
        sceneGraph.doWithData();

        Pre2_Viewpoint.setPre2_ViewpointDoWithDataCallback(new MyViewpointCallback2());

        Pre2_SceneGraph sceneGraph2 = new Pre2_SceneGraph();
        sceneGraph2.doWithData();
    }
};

