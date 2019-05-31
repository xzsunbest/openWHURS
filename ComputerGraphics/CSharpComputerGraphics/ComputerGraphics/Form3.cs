using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ComputerGraphics
{
    public partial class MyForm2 : Form
    {
        public MyForm2()
        {
            x0 = 100;
            y0 = 100;
            output = "Hello world!";
            InitializeComponent();
        }

        private int x0, y0;
        private string output;
        public int X0 { get { return x0; } }
        public int Y0 { get { return y0; } }
        public string Output { get { return output; } }

        private void button1_Click(object sender, EventArgs e)
        {
            x0 = (int)numericUpDown1.Value;
            y0 = (int)numericUpDown2.Value;
            output = textBox1.Text;
        }

        private void button2_Click(object sender, EventArgs e)
        {
        }
    }
}
