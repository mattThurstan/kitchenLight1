/*----------------------------comms---------------------------*/
void setupMesh() 
{
  mesh.setDebugMsgTypes( ERROR | STARTUP );       // set before init() so that you can see startup messages

  mesh.init(MESH_NAME, MESH_PASSWORD, MESH_PORT, WIFI_AP_STA, _stationChannel, MESH_NODE_HIDDEN, MESH_NODE_MAX_CONN); // hidden
  
  mesh.setContainsRoot(true);
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
}

void changeChannel(uint8_t channel) {
  _stationChannel = channel;
  saveSettings();
  //deviceRestart();
  doRestart(3);
}

void turnOffComms() 
{
  publishDeviceOffline();
  DEBUG_COMMS = false;
  turnOffMesh();
  turnOffWifi();
  DEBUG_GEN = false;
  turnOffSerial();
}
