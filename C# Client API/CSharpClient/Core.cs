using System;
using System.IO;
using System.Net.Sockets;
using System.Text;
using System.Collections;
using System.Collections.Generic;
using Newtonsoft.Json;

namespace CSharpClient
{
    public class Core
    {
        const int READ_BUFFER_SIZE = 255;
        private byte[] readBuffer = new byte[READ_BUFFER_SIZE];

        public delegate void DataEvent(Hashtable message);
        
        public delegate void ConnectedEvent();
        public delegate void DisconnectedEvent(string reason);
        
        public delegate void LoggedInEvent(Player player);
        public delegate void LoggedOutEvent(string reason);
        
        public delegate void JoinedChannelEvent(Player player, Channel channel);
        public delegate void LeftChannelEvent(Player player, Channel channel);
        
        public delegate void UserListRecievedEvent(Dictionary<string, Player> list);
        public delegate void ChannelListRecievedEvent(Dictionary<string, Channel> list);

        public delegate void ChatMessageRecievedEvent(string message, Player author, Channel room);
        public delegate void PrivateMessageRecievedEvent(string message, Player author);
       
        public delegate void PersistentDataRecievedEvent(string name, Object data);
        public delegate void AllPersistentDataRecievedEvent(Object data);
        public delegate void PulicDataRecievedEvent(string name, Object data, Player author);
        public delegate void AllPulicDataRecievedEvent(Object data, Player author);

        public delegate void ErrorMessageEvent(ErrorMessage error);
        public delegate void ParsingErrorEvent(Exception e, Hashtable lastData);
        
        public DataEvent OnData;
       
        public ConnectedEvent OnConnected;
        public DisconnectedEvent OnDisconnected;
        
        public LoggedInEvent OnLoggedIn;
        public LoggedOutEvent OnLoggedOut;
        
        public JoinedChannelEvent OnJoinedChannel;
        public LeftChannelEvent OnLeftChannel;
        
        public UserListRecievedEvent OnUserListRecieved;
        public ChannelListRecievedEvent OnChannelListRecieved;
        
        public ChatMessageRecievedEvent OnChatMessageRecieved;
        public PrivateMessageRecievedEvent OnPrivateMessageRecieved;

        //ServerSide Client data events.
        public PersistentDataRecievedEvent OnPersistentDataRecieved;
        public AllPersistentDataRecievedEvent OnAllPersistentDataRecieved;
        public PulicDataRecievedEvent OnPulicDataRecieved;
        public AllPulicDataRecievedEvent OnAllPulicDataRecieved;

        public ErrorMessageEvent OnErrorMessage;
        // If this event is called, I made a mistake somewhere, you should always track this just in case and inform me if it is called...
        public ParsingErrorEvent OnParsingError;

        TcpClient client;

        string userId;
        string userName;
        Player player;

        Hashtable playersById = new Hashtable();
        Hashtable channelsById = new Hashtable();

        public string Connect(string ip, int port)
        {
            try
            {
                // The TcpClient is a subclass of Socket, providing higher level 
                // functionality like streaming.
                client = new TcpClient(ip, port);

                // Start an asynchronous read invoking DoRead to avoid lagging the user
                // interface.
                client.GetStream().BeginRead(readBuffer, 0, READ_BUFFER_SIZE, new AsyncCallback(DoRead), null);
                // Make sure the window is showing before popping up connection dialog.

                OnConnected();
                return "Connection Succeeded";
            }
            catch (Exception e)
            {
                OnDisconnected(e.Message);
                return "Server not responding... reason:"+ e.Message;
            }
        }

        public string Login(string username, string password)
        {
            Dictionary<string, Object> loginInfos = new Dictionary<string, Object>();
            loginInfos.Add("type", "login");
            loginInfos.Add("name", username);
            loginInfos.Add("pass", password);
            return Send(loginInfos);
        }

        public string Logout(string username, string password)
        {
            Dictionary<string, Object> loginInfos = new Dictionary<string, Object>();
            loginInfos.Add("type", "logout");
            return Send(loginInfos);
        }

        public void Disconnect()
        {
            client.GetStream().Close(); 
        }

        public string data;
        public Hashtable LastData = new Hashtable();
        public string location="";
        
        private void DoRead(IAsyncResult ar)
        {
            int BytesRead;
            try
            {
                // Finish asynchronous read into readBuffer and return number of bytes read.
                BytesRead = client.GetStream().EndRead(ar);
                if (BytesRead < 1)
                {
                    // if no bytes were read server has close.  
                    OnDisconnected("Connexion was lost");
                    return;
                }
                // Convert the byte array the message was saved into, minus two for the
                // Chr(13) and Chr(10)
                data = Encoding.UTF8.GetString(readBuffer, 0, BytesRead);
                
                //Convert data to Json
                try
                {
                    LastData.Clear();
                    location = "Parse Fail";
                    LastData = JsonParser.JsonToHashTable(data);
                    location = "Data Fail: ";
                    OnDataRecieved(LastData);
                }
                catch(Exception e)
                { 
                    //Object could not be parsed
                    OnParsingError(e, LastData);
                }
                   
                // Start a new asynchronous read into readBuffer.
                client.GetStream().BeginRead(readBuffer, 0, READ_BUFFER_SIZE, new AsyncCallback(DoRead), null);

            }
            catch(Exception e)
            {
                OnParsingError(e, LastData);
                OnDisconnected("Internal error");
            }
        }

        public string Send(Dictionary<string, Object> data)
        {
            string json = JsonConvert.SerializeObject(data);
            SendData(json);
            return json;
        }

        // Use a StreamWriter to send a message to server.
        private void SendData(string data)
        {
            StreamWriter writer = new StreamWriter(client.GetStream());
            writer.Write(data+"\n");
            writer.Flush();
        }

        private void OnDataRecieved(Hashtable values)
        {
            location += values["type"];
            if(values["type"].Equals("err"))
            {
                OnErrorMessage(new ErrorMessage(values["id"]+""));
            }
            else if (values["type"].Equals("login"))
            {
                player = new Player(values["id"] + "", values["name"] + "", this);
                userName = player.getName();
                userId = player.getId() + "";

                if (playersById[values["id"] + ""] == null)
                    playersById.Add(values["id"] + "", player);

                OnLoggedIn(player);
            }
            else if (values["type"].Equals("logout"))
            {
                OnLoggedOut(values["msg"]+"");
            }
            else if (values["type"].Equals("join"))
            {
                Player newPlayer;
                Channel newChannel;

                if (playersById[values["id"] + ""] == null)
                {
                    newPlayer = new Player(values["id"] + "", values["name"] + "", this);
                    playersById.Add(values["id"] + "", newPlayer);
                }
                else
                    newPlayer = (Player)playersById[values["id"] + ""];

                if (channelsById[values["chan_id"] + ""] == null)
                {
                    newChannel = new Channel(values["chan_id"] + "", values["chan"] + "", this);
                    channelsById.Add(values["chan_id"] + "", newChannel);
                }
                else
                    newChannel = (Channel)channelsById[values["chan_id"] + ""];

                OnJoinedChannel(newPlayer, newChannel);
            }
            else if (values["type"].Equals("leave"))
            {
                Player newPlayer;
                Channel newChannel;

                if (playersById[values["id"] + ""] == null)
                {
                    newPlayer = new Player(values["id"] + "", values["name"] + "", this);
                    playersById.Add(values["id"] + "", newPlayer);
                }
                else
                    newPlayer = (Player)playersById[values["id"] + ""];

                if (channelsById[values["chan_id"] + ""] == null)
                {
                    newChannel = new Channel(values["chan_id"] + "", values["chan"] + "", this);
                    channelsById.Add(values["chan_id"] + "", newChannel);
                }
                else
                    newChannel = (Channel)channelsById[values["chan_id"] + ""];

                OnLeftChannel(newPlayer, newChannel);
            }
            else if (values["type"].Equals("msg"))
            {
                Player newPlayer;
                Channel newChannel;

                if (playersById[values["a_id"] + ""] == null)
                {
                    newPlayer = new Player(values["a_id"] + "", values["a_n"] + "", this);
                    playersById.Add(values["a_id"] + "", newPlayer);
                }
                else
                    newPlayer = (Player)playersById[values["a_id"] + ""];

                if (channelsById[values["c_id"] + ""] == null)
                {
                    newChannel = new Channel(values["c_id"] + "", values["c_n"] + "", this);
                    channelsById.Add(values["c_id"] + "", newChannel);
                }
                else
                    newChannel = (Channel)channelsById[values["c_id"] + ""];

                OnChatMessageRecieved(values["msg"] + "", newPlayer, newChannel);
            }
            else if (values["type"].Equals("pm"))
            {
                Player newPlayer;
                
                if (playersById[values["a_id"] + ""] == null)
                {
                    newPlayer = new Player(values["a_id"] + "", values["a_n"] + "", this);
                    playersById.Add(values["a_id"] + "", newPlayer);
                }
                else
                    newPlayer = (Player)playersById[values["a_id"] + ""];

                OnPrivateMessageRecieved(values["msg"] + "", newPlayer);
            }
            else if (values["type"].Equals("channels"))
            {
                if (values["channels"] != null)
                {
                    Hashtable channels = (Hashtable)values["channels"];

                    Dictionary<string, Channel> parsedChannels = new Dictionary<string, Channel>();

                    foreach (string s in channels.Keys)
                    {
                        if (channels[s] != null)
                        {
                            Hashtable channel = (Hashtable)channels[s];
                            Channel newChannel = new Channel(channel["id"] + "", channel["name"] + "", int.Parse(channel["users"] + ""), int.Parse(channel["maxUsers"] + ""), this);
                            parsedChannels.Add(channel["id"] + "", newChannel);

                            if (channelsById[channel["id"] + ""] == null)
                                channelsById.Add(channel["id"] + "", newChannel);
                        }
                    }

                    OnChannelListRecieved(parsedChannels);
                }
            }
            else if (values["type"].Equals("_p"))
            {
                OnPersistentDataRecieved(values["name"].ToString(), values["data"]);
            }
            else if (values["type"].Equals("_p_all"))
            {
                OnAllPersistentDataRecieved(values["data"]);
            }
            else if (values["type"].Equals("_d"))
            {
                OnPulicDataRecieved(values["n"].ToString(), values["d"], (Player)playersById[values["u"].ToString()]);
            }
            else if (values["type"].Equals("_d_all"))
            {
                OnAllPulicDataRecieved(values["d"], (Player)playersById[values["u"].ToString()]);
            }
            else
            {
                OnData(values);
            }
        }

        public void RequestPublicChannels()
        {
            Dictionary<string, Object> data = new Dictionary<string, object>();
            data.Add("type", "getChannels");
            Send(data);
        }

        public Player getMyPlayer() { return player; }
        public string getMyName() { return userName; }
        public int getMyId() { return int.Parse(userId); }

        public void SendChat(string message)
        {
            Dictionary<string, Object> data = new Dictionary<string,object>();
            data.Add("type", "chat");
            data.Add("action", "msg");
            data.Add("msg", message);
            Send(data);
        }

        public void JoinChannel(string channelName)
        {
            Dictionary<string, Object> data = new Dictionary<string, object>();
            data.Add("type", "join");
            data.Add("name", channelName);
            Send(data);
        }

        public void SetPublicData(string name, Object _data)
        {
            Dictionary<string, Object> data = new Dictionary<string, object>();
            data.Add("type", "_d");
            data.Add("n", name);
            data.Add("d", _data);
            Send(data);
        }

        public void SetPersistentData(string name, Object _data)
        {
            Dictionary<string, Object> data = new Dictionary<string, object>();
            data.Add("type", "_p");
            data.Add("n", name);
            data.Add("d", _data);
            Send(data);
        }

        public void getAllPersistentData()
        {
            Dictionary<string, Object> data = new Dictionary<string, object>();
            data.Add("type", "_p_all");
            Send(data);
        }

        public void getPersistentData(string name)
        {
            Dictionary<string, Object> data = new Dictionary<string, object>();
            data.Add("type", "_gp");
            data.Add("name", name); 
            Send(data);
        }
    } 
}
