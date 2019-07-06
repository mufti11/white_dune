class Extra {
    public static void reInitSensor(X3dNode node) {
        if (node.getType() == X3dTouchSensorType.type) {
            X3dTouchSensor touchSensor = (X3dTouchSensor)node;
            touchSensor.isOver = false;
        }
        if (node.getType() == X3dPlaneSensorType.type) {
            X3dPlaneSensor planeSensor = (X3dPlaneSensor)node;
            planeSensor.isOver = false;
        }
        if (node.getType() == X3dCylinderSensorType.type) {
            X3dCylinderSensor cylinderSensor = (X3dCylinderSensor)node;
            cylinderSensor.isOver = false;
        }
        if (node.getType() == X3dSphereSensorType.type) {
            X3dSphereSensor sphereSensor = (X3dSphereSensor)node;
            sphereSensor.isOver = false;
        }
    }
}
    
    
    
    
