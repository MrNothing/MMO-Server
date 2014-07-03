using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using CSharpClient;

namespace Connector
{
    public partial class Connector : Form
    {
        public Connector()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        Core core = new Core();
        private void button1_Click(object sender, EventArgs e)
        {
            core.OnConnected += new Core.ConnectedEvent(OnConnectionSuccess);
            core.OnDisconnected += new Core.DisconnectedEvent(OnConnectionLost);
            core.Connect("127.0.0.1", 6600);
        }

        private void OnConnectionSuccess()
        {
            label1.Text = "Connected!";
        }

        private void OnConnectionLost(string reason)
        {
            label1.Text = "Disconnected: " + reason;
        }
    }
}
