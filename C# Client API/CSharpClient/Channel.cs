using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CSharpClient
{
    class Channel
    {
        string Id;
        string Name;
        private Core core;

        public Channel(string id, string name, Core _core)
        {
            Id = id;
            Name = name;
            core = _core;
        }

        public string getName() { return Name; }
        public int getId() { return int.Parse(Id); }

        public void requestAllUsers()
        {
            Dictionary<string, Object> data = new Dictionary<string, object>();
            data.Add("type", "getUsers");
            data.Add("id", Id);
            core.Send(data);
        }

        public void SendChat(string message)
        {
            Dictionary<string, Object> data = new Dictionary<string, object>();
            data.Add("type", "chat");
            data.Add("action", "msg");
            data.Add("channel", Id);
            data.Add("msg", message);
            core.Send(data);
        }
    }
}
