using System;
using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using CSharpClient;
public class Chat : MonoBehaviour {
	Core core;
	// Use this for initialization
	void Start () {
		core = new Core ();
		core.OnConnected += new Core.ConnectedEvent(OnConnectionSucces);
		core.OnDisconnected += new Core.DisconnectedEvent (OnConnectionLost);
		core.OnLoggedIn += new Core.LoggedInEvent (OnLoginSuccess);

		core.OnJoinedChannel += new Core.JoinedChannelEvent (OnJoinedChannel);
		core.OnLeftChannel += new Core.LeftChannelEvent (OnLeftChannel);

		core.OnChannelListRecieved += new Core.ChannelListRecievedEvent (GotChannels);

		core.OnChatMessageRecieved += new Core.ChatMessageRecievedEvent (OnChat);

		core.OnErrorMessage += new Core.ErrorMessageEvent (OnError);

		core.OnParsingError += new Core.ParsingErrorEvent(OnParseError);

		core.Connect("127.0.0.1", 6600);
	}

	bool loggedIn = false;
	string chatMessages="";
	string inputText="";
	string userName="Username";
	void OnGUI()
	{
		if(!loggedIn)
		{
			GUILayout.BeginArea (new Rect(Screen.width/2-100, Screen.height/2, 200, 50));
			userName = GUILayout.TextField (userName);
			if(GUILayout.Button("Login"))
			{
				core.Login(userName, "");
				inputText = "";
			}
			GUILayout.EndArea ();
		}
		else
		{
			GUILayout.BeginArea (new Rect(Screen.width/2-100, Screen.height/2, 300, 200));
			GUILayout.Label (chatMessages);
			GUILayout.BeginHorizontal();
			inputText = GUILayout.TextField (inputText);

			if(GUILayout.Button("Send"))
			{
				core.SendChat(inputText);
				inputText = "";
			}

			GUILayout.EndHorizontal();
			GUILayout.EndArea ();
		}


	}
	
	void OnConnectionSucces()
	{
		Debug.Log ("Connected!");
		chatMessages+="Connected!"+"\n";
	}

	void OnConnectionLost(string reason)
	{
		Debug.Log ("Disconnected, reason: "+reason);
		chatMessages+="Disconnected."+"\n";
	}

	void OnLoginSuccess(Player player)
	{
		loggedIn = true;

		Debug.Log ("Logged in as " + player.getName ());
		chatMessages += "Logged in as " + player.getName ()+"\n";

		core.RequestPublicChannels ();
	}

	void OnJoinedChannel(Player player, Channel channel)
	{
		Debug.Log (player.getName ()+" has joined the channel: " + channel.getName ());
		chatMessages += player.getName ()+" has joined the channel: " + channel.getName ()+"\n";
	}

	void OnLeftChannel(Player player, Channel channel)
	{
		chatMessages += player.getName ()+" has left the channel: " + channel.getName ()+"\n";
	}

	void GotChannels(Dictionary<string, Channel> channels)
	{
		Debug.Log ("Channels revieved!");
		foreach (string s in channels.Keys) 
		{
			//join the first channel in the list...
			core.JoinChannel (channels[s].getName());
			break;
		}
	}

	void OnChat (string message, Player author, Channel room)
	{
		Debug.Log ("[" + author.getName ()+"] says: " +message);
		chatMessages += "[" + author.getName ()+"] says: " +message+"\n";
	}

	void OnError(ErrorMessage error)
	{
		Debug.Log (error.Id+" "+error.Message);
		chatMessages += error.Message+"\n";
	}


	//If this is event is triggered, I made a mistake somewhere...
	void OnParseError(Exception e, Hashtable data)
	{
		Debug.Log (core.location);
	}
}
