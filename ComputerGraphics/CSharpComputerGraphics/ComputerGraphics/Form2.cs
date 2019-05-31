using System;
using System.Windows.Forms;

namespace ComputerGraphics
{
    public partial class MyForm : Form
    {
        public MyForm()
        {
            xscale = (float)1.0;
            yscale = (float)1.0;
            InitializeComponent();
        }

        private float xscale, yscale;
        public float Xscale { get { return xscale; } }
        public float Yscale { get { return yscale; } }

        private void button1_Click(object sender, EventArgs e)
        {
            xscale = (float)numericUpDown1.Value;
            yscale = (float)numericUpDown2.Value;
        }

        private void button2_Click(object sender, EventArgs e)
        {
        }
    }
}
