using System;
using System.Drawing;
using System.Windows.Forms;

namespace ComputerGraphics
{
    public partial class Form1 : Form
    {
        private void LiangCut_Click(object sender, EventArgs e)
        {
            MenuID = 23;
            PressNum = 0;
            Graphics g = CreateGraphics();
            g.Clear(BackColor1);
            XL = 100;
            XR = 400;
            YD = 100;
            YU = 400;
            pointsgroup[0] = new Point(XL, YD);
            pointsgroup[1] = new Point(XR, YD);
            pointsgroup[2] = new Point(XR, YU);
            pointsgroup[3] = new Point(XL, YU);
            g.DrawPolygon(Pens.Blue, pointsgroup);
        }

        private void LiangCut1(int x1, int y1, int x2, int y2)
        {
            Graphics g = CreateGraphics();
            g.DrawLine(Pens.Red, x1, y1, x2, y2);
            float tsx, tsy, tex, tey;
            if (x1 == x2)
            {
                tsx = 0;
                tex = 1;
            }
            else if (x1 < x2)
            {
                tsx = (float)(XL - x1) / (float)(x2 - x1);
                tex = (float)(XR - x1) / (float)(x2 - x1);
            }
            else
            {
                tsx = (float)(XR - x1) / (float)(x2 - x1);
                tex = (float)(XL - x1) / (float)(x2 - x1);
            }
            if (y1 == y2)
            {
                tsy = 0;
                tey = 1;
            }
            else if (y1 < y2)
            {
                tsy = (float)(YD - y1) / (float)(y2 - y1);
                tey = (float)(YU - y1) / (float)(y2 - y1);
            }
            else
            {
                tsy = (float)(YU - y1) / (float)(y2 - y1);
                tey = (float)(YD - y1) / (float)(y2 - y1);
            }
            tsx = Math.Max(tsx, tsy);
            tsx = Math.Max(tsx, 0);
            tex = Math.Min(tex, tey);
            tex = Math.Min(tex, 1);
            if (tsx < tex)
            {
                int xx1, yy1, xx2, yy2;
                xx1 = (int)(x1 + (x2 - x1) * tsx);
                yy1 = (int)(y1 + (y2 - y1) * tsx);
                xx2 = (int)(x1 + (x2 - x1) * tex);
                yy2 = (int)(y1 + (y2 - y1) * tex);
                Pen MyPen = new Pen(Color.Yellow, 3);
                g.DrawLine(MyPen, xx1, yy1, xx2, yy2);
            }
        }
    }
}
