using System;
using System.Drawing;
using System.Windows.Forms;

namespace ComputerGraphics
{
    public partial class Form1 : Form
    {
        private void TransMove_Click(object sender, EventArgs e)
        {
            MenuID = 11;
            PressNum = 0;
            Graphics g = CreateGraphics();
            g.Clear(BackColor1);
            pointsgroup[0] = new Point(100, 100);
            pointsgroup[1] = new Point(200, 100);
            pointsgroup[2] = new Point(200, 200);
            pointsgroup[3] = new Point(100, 200);
            g.DrawPolygon(Pens.Red, pointsgroup);
        }
    }
}
