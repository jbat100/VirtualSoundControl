using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;

public class OSCStart : MonoBehaviour {

	// Use this for initialization
	void Start () {
		Debug.Log("Calling OSCHandler.Instance.Init");
		OSCHandler.Instance.Init();
	}
	
	// Update is called once per frame
	void Update () {
		//Debug.Log("Calling OSCHandler.Instance.UpdateLogs");
		OSCHandler.Instance.UpdateLogs();

		//Dictionary<string, string> servers = new Dictionary<string, string>();

		Dictionary<string, ServerLog> servers = new Dictionary<string, ServerLog>();
		servers = OSCHandler.Instance.Servers;

		foreach(KeyValuePair<string, ServerLog> item in servers)
		{
			// If we have received at least one packet,
			// show the last received from the log in the Debug console
			if(item.Value.log.Count > 0) 
			{
				int lastPacketIndex = item.Value.packets.Count - 1;

				/* UnityEngine.Debug.Log(String.Format("COUNT: {0} SERVER: {1} ADDRESS: {2} VALUE 0: {3}", 
				                                    item.Value.log.Count.ToString(),
				                                    item.Key, // Server name
				                                    item.Value.packets[lastPacketIndex].Address, // OSC address
				                                    item.Value.packets[lastPacketIndex].Data[0].ToString())); //First data value */
				Debug.Log("LAST LOG" + item.Value.log[lastPacketIndex]);
			}
		}
	}
}
