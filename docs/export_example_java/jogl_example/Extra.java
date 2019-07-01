class Extra {
    public static void reInitSensor(X3dNode node) {
        if (node.getType() == X3dTouchSensorType.type) {
            X3dTouchSensor touchSensor = (X3dTouchSensor)node;
            touchSensor.isOver = false;
        }
    }
}



