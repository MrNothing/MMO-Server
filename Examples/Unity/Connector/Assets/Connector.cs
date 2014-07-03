using UnityEngine;
using System.Collections;
using CSharpClient;
public class Connector : MonoBehaviour {
	Core core;
	// Use this for initialization
	void Start () {
		core = new Core ();
		core.OnConnected += new Core.ConnectedEvent(OnConnectionSucces);
		core.OnDisconnected += new Core.DisconnectedEvent (OnConnectionLost);
		core.Connect("127.0.0.1", 6600);
	}
	
	void OnConnectionSucces()
	{
		Debug.Log ("Connected!");
	}

	void OnConnectionLost(string reason)
	{
		Debug.Log ("Disconnected, reason: "+reason);
	}
}
