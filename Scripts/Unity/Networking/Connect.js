var remoteIP = "127.0.0.1";
var remotePort = 25000;
var listenPort = 25000;
var useNAT = false;
var yourIP = "";
var yourPort = "";

function OnGUI() {
	if (Network.peerType == NetworkPeerType.Disconnected) {
		
		//GUILayout.Label("Not Connected");
		
		if (GUI.Button(Rect(10,10,100,30), "Connect")) {
			Network.Connect(remoteIP, remotePort);
		}
		
		if (GUI.Button(Rect(10,50,100,30), "Start Server")) {
			Network.InitializeServer(4, listenPort, useNAT);
			for (var go : GameObject in FindObjectsOfType(GameObject)) {
				go.SendMessage("OnNetworkLoadedLevel", SendMessageOptions.DontRequireReceiver);
			}
		}
		
		remoteIP = GUI.TextField(Rect(120,10,100,20), remoteIP);
		remotePort = parseInt( GUI.TextField(Rect(230,10,50,20), remotePort.ToString()) );
	}
	else if (Network.peerType == NetworkPeerType.Connecting) {
		GUILayout.Label("Connecting");
	}
	else  {
		//GUILayout.Label("Network started");
		var ipAddress = Network.player.ipAddress;
		var port = Network.player.port.ToString();
		GUI.Label(Rect(140,20,250,40), "IP Address: + " + ipAddress + ":" + port);
		
		if (GUI.Button(Rect(10,10,100,50), "Diconnect")) {
			Network.Disconnect(200);
		}
	}
}

function OnConnectedToServer() {
	for (var go : GameObject in FindObjectsOfType(GameObject)) {
		go.SendMessage("OnNetworkLoadedLevel", SendMessageOptions.DontRequireReceiver);
	}
}
