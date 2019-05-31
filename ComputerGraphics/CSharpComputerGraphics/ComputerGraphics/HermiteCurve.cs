using System;
using System.Drawing;
using System.Windows.Forms;

namespace ComputerGraphics
{
    public partial class Form1 : Form
    {
        private void HermiteCurve_Click(object sender, EventArgs e)
        {
            MenuID = 10;
            PressNum = 0;
            PointNum = 0;
            Graphics g = CreateGraphics();
            g.Clear(BackColor1);
        }

        private void Hermite1(int mode)
        {
            Point[] p = new Point[300];
            int i = 0, j = 0;
            p[i++] = group[j++];
            p[i++] = group[j++];
            while (j <= PointNum - 2)
            {
                p[i++] = group[j++];
                p[i].X = (group[j].X + group[j - 1].X) / 2;
                p[i++].Y = (group[j].Y + group[j - 1].Y) / 2;
                p[i++] = group[j++];
            }
            for (j = 0; j < i - 3; j += 3)
            {
                Hermite_4(mode, p[j], p[j + 1], p[j + 2], p[j + 3]);
            }
        }

        private void Hermite_4(int mode, Point p1, Point p2, Point p3, Point p4)
        {
            int i, n;
            Graphics g = CreateGraphics();
            Point m0 = new Point(p2.X - p1.X, p2.Y - p1.Y);
            Point m1 = new Point(p4.X - p3.X, p4.Y - p3.Y);
            Point p = new Point();
            Point oldp = new Point();
            double t1, t2, t3, t4, dt;
            Pen MyPen = new Pen(Color.Red, 1);
            n = 100;
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
            dt = 1.0 / n;
            for (i = 1; i <= n; i++)
            {
                t1 = 2 * Math.Pow(i * dt, 3) - 3 * Math.Pow(i * dt, 2) + 1;
                t2 = 2 * (Math.Pow(i * dt, 3) - 2 * Math.Pow(i * dt, 2) + i * dt);
                t3 = 3 * (Math.Pow(i * dt, 3) - Math.Pow(i * dt, 2));
                t4 = -2 * Math.Pow(i * dt, 3) + 3 * Math.Pow(i * dt, 2);
                p.X = (int)(t1 * p1.X + t2 * m0.X + t3 * m1.X + t4 * p4.X);
                p.Y = (int)(t1 * p1.Y + t2 * m0.Y + t3 * m1.Y + t4 * p4.Y);
                g.DrawLine(MyPen, oldp, p);
                oldp = p;
            }
        }
    }
}
