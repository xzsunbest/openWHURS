using System;
using System.Drawing;
using System.Windows.Forms;

namespace ComputerGraphics
{
    public partial class Form1 : Form
    {
        private void BSampleCurve_Click(object sender, EventArgs e)
        {
            MenuID = 8;
            PressNum = 0;
            PointNum = 0;
            Graphics g = CreateGraphics();
            g.Clear(BackColor1);
        }

        private void BSample1(int mode)
        {
            for (int i = 0; i < PointNum - 3; i++)
            {
                BSample_4(mode, group[i], group[i + 1], group[i + 2], group[i + 3]);
            }
        }

        private void BSample_4(int mode, Point p0, Point p1, Point p2, Point p3)
        {
            Graphics g = CreateGraphics();
            Point p = new Point();
            Point oldp = new Point();
            Pen MyPen = new Pen(Color.Red, 1);
            int n = 100;
            if (mode == 2)
            {
                MyPen = new Pen(Color.Red, 1);
            }
            if (mode == 1)
            {
                MyPen = new Pen(Color.Black, 1);
            }
            if (mode == 0)
            {
                MyPen = new Pen(Color.White, 1);
            }
            oldp = p1;
            double dt = 1.0 / n;
            for (double t = 0.0; t <= 1.0; t += dt)
            {
                double t1 = Math.Pow(1.0 - t, 3);
                double t2 = 3.0 * t * t * t - 6.0 * t * t + 4.0;
                double t3 = -3.0 * t * t * t + 3.0 * t * t + 3.0 * t + 1.0;
                double t4 = t * t * t;
                p.X = (int)((t1 * p0.X + t2 * p1.X + t3 * p2.X + t4 * p3.X) / 6.0);
                p.Y = (int)((t1 * p0.Y + t2 * p1.Y + t3 * p2.Y + t4 * p3.Y) / 6.0);
                if (t > 0)
                {
                    g.DrawLine(MyPen, oldp, p);
                }
                oldp = p;
            }
        }
    }
}
