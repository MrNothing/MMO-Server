using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CSharpClient
{
    class ErrorMessage
    {
        public string Id;
        public string Message="?";
        public ErrorMessage(string id)
        {
            Id = id;

            if (id.Equals("0"))
                Message = "no such type";
            else if (id.Equals("0c"))
                Message = "no chat action";
            else if (id.Equals("0p"))
                Message = "parsing error";
            else if (id.Equals("0a"))
                Message = "no such action";
            else if (id.Equals("1"))
                Message = "this username is already taken!";
            else if (id.Equals("2"))
                Message = "no name specifed";
            else if (id.Equals("3"))
                Message = "already logged in!";
            else if (id.Equals("4"))
                Message = "channel not found!";
            else if (id.Equals("5"))
                Message = "No channel specified!";
            else if (id.Equals("6"))
                Message = "You are not in a channel!";
            else if (id.Equals("7"))
                Message = "Player not found!";
            else if (id.Equals("8"))
                Message = "You are not in this room!";
            else if (id.Equals("9"))
                Message = "You have already joined this channel!";
            else if (id.Equals("10"))
                Message = "You must be logged in first!";
            else
                Message = "Unknown error";
        }
    }
}
