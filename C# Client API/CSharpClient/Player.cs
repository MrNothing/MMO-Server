using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CSharpClient
{
    public class Player
    {
        private string Id;
        private string Name;
        
        private Core core;

        public Player(string id, string name, Core _core)
        {
            Id = id;
            Name = name;
            core = _core;
        }

        public string getName(){ return Name; }
        public int getId() { return int.Parse(Id); }

        public void SendPrivateMessage(string message)
        {
            Dictionary<string, Object> data = new Dictionary<string, object>();
            data.Add("type", "chat");
            data.Add("action", "pm");
            data.Add("to", Name);
        }
    }
}
