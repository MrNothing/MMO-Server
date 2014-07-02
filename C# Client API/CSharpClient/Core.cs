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

        public delegate void Data(Dictionary<string, Object> message);
        public delegate void Connected();
        public delegate void Disconnected(string reason);
        public delegate void LoggedIn(Player player);
        public delegate void LoggedOut(string reason);
        public delegate void JoinedChannel(Player player, Channel channel);
        public delegate void LeftChannel(Player player, Channel channel);
        public delegate void ErrorMessageEvt(ErrorMessage error);
        public delegate void UserListRecieved(Dictionary<string, Player> list);
        public delegate void ChannelListRecieved(Dictionary<string, Channel> list);
        public delegate void ChatMessageRecieved(string message, Player author, Channel room);
        public delegate void PrivateMessageRecieved(string message, Player author);
        public Data OnData;
        public Connected OnConnected;
        public Disconnected OnDisconnected;
        public LoggedIn OnLoggedIn;
        public LoggedOut OnLoggedOut;
        public JoinedChannel OnJoinedChannel;
        public LeftChannel OnLeftChannel;
        public ErrorMessageEvt OnErrorMessage;
        public UserListRecieved OnUserListRecieved;
        public ChannelListRecieved OnChannelListRecieved;
        public ChatMessageRecieved OnChatMessageRecieved;
        public PrivateMessageRecieved OnPrivateMessageRecieved;

        TcpClient client;

        string userId;
        string userName;
        Player player;

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

        public void Login(string username, string password)
        {
            Dictionary<string, Object> loginInfos = new Dictionary<string, Object>();
            loginInfos.Add("type", "login");
            loginInfos.Add("name", username);
            loginInfos.Add("pass", password);
            Send(loginInfos);
        }

        public void Logout(string username, string password)
        {
            Dictionary<string, Object> loginInfos = new Dictionary<string, Object>();
            loginInfos.Add("type", "logout");
            Send(loginInfos);
        }

        public void Disconnect()
        {
            client.GetStream().Close(); 
        }

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
                string data = Encoding.ASCII.GetString(readBuffer, 0, BytesRead - 2);

                //Convert data to Json
                try
                {
                    Dictionary<string, Object> values = JsonConvert.DeserializeObject<Dictionary<string, Object>>(data);
                    OnDataRecieved(values);
                }
                catch
                { 
                    //Object could not be parsed
                }
                   
                // Start a new asynchronous read into readBuffer.
                client.GetStream().BeginRead(readBuffer, 0, READ_BUFFER_SIZE, new AsyncCallback(DoRead), null);

            }
            catch(Exception e)
            {
                OnDisconnected(e.Message);
            }
        }

        public void Send(Dictionary<string, Object> data)
        {
            string json = JsonConvert.SerializeObject(data);
            SendData(json);
        }

        // Use a StreamWriter to send a message to server.
        private void SendData(string data)
        {
            StreamWriter writer = new StreamWriter(client.GetStream());
            writer.Write(data + (char)13);
            writer.Flush();
        }

        private void OnDataRecieved(Dictionary<string, Object> values)
        {
           
            if(values["type"].Equals("err"))
            {
                OnErrorMessage(new ErrorMessage(values["id"]+""));
            }
            else if (values["type"].Equals("login"))
            {
                player = new Player(values["id"] + "", values["name"] + "", this);
                userName = player.getName();
                userId = player.getId() + "";

                OnLoggedIn(player);
            }
            else if (values["type"].Equals("logout"))
            {
                OnLoggedOut(values["msg"]+"");
            }
            else if (values["type"].Equals("join"))
            {
                Player newPlayer = new Player(values["id"] + "", values["name"] + "", this);
                Channel newChannel = new Channel(values["chan_id"] + "", values["chan"] + "", this);

                OnJoinedChannel(newPlayer, newChannel);
            }
            else if (values["type"].Equals("leave"))
            {
                Player newPlayer = new Player(values["id"] + "", values["name"] + "", this);
                Channel newChannel = new Channel(values["chan_id"] + "", values["chan"] + "", this);

                OnLeftChannel(newPlayer, newChannel);
            }
            else if (values["type"].Equals("chat"))
            {
                
            }
            else
            {
                OnData(values);
            }
        }

        public void RequestPublicChannels()
        { 
        
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
    } 
}
