using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CSharpClient
{
    public class Channel
    {
        string Id;
        string Name;
        int Users;
        int MaxUsers;
        private Core core;

        public Channel(string id, string name, Core _core)
        {
            Id = id;
            Name = name;
            core = _core;
        }

        public Channel(string id, string name, int users, int maxUsers, Core _core)
        {
            Id = id;
            Name = name;
            Users = users;
            MaxUsers = maxUsers;
            core = _core;
        }

        public string getName() { return Name; }
        public int getId() { return int.Parse(Id); }
        public int getUsersCount() { return Users; }
        public int getMaxUsers() { return MaxUsers; }

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
            data.Add("channel", Name);
            data.Add("msg", message);
            core.Send(data);
        }

        public void Join()
        {
            Dictionary<string, Object> data = new Dictionary<string, object>();
            data.Add("type", "join");
            data.Add("name", Name);
            core.Send(data);
        }
    }
}
