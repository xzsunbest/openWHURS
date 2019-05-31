using System;
using System.Drawing;
using System.Windows.Forms;

namespace ComputerGraphics
{
    public partial class Form1 : Form
    {
        private void BezierCurve_Click(object sender, EventArgs e)
        {
            MenuID = 7;
            PressNum = 0;
            PointNum = 0;
            Graphics g = CreateGraphics();
            g.Clear(BackColor1);
        }

        private void Bezier1(int mode)
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
                Bezier_4(mode, p[j], p[j + 1], p[j + 2], p[j + 3]);
            }
        }

        private void Bezier_4(int mode, Point p1, Point p2, Point p3, Point p4)
        {
            int i, n;
            Graphics g = CreateGraphics();
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
                t1 = Math.Pow(1.0 - i * dt, 3);
                t2 = Math.Pow(1.0 - i * dt, 2) * i * dt;
                t3 = Math.Pow(i * dt, 2) * (1.0 - i * dt);
                t4 = Math.Pow(i * dt, 3);
                p.X = (int)(t1 * p1.X + 3 * t2 * p2.X + 3 * t3 * p3.X + t4 * p4.X);
                p.Y = (int)(t1 * p1.Y + 3 * t2 * p2.Y + 3 * t3 * p3.Y + t4 * p4.Y);
                g.DrawLine(MyPen, oldp, p);
                oldp = p;
            }
        }

        private void Bezier_41(int mode, Point p1, Point p2, Point p3, Point p4)
        {
            Graphics g = CreateGraphics();
            Point p = new Point();
            Point oldp = new Point();
            double t, dt;
            Point[] g1 = new Point[4];
            g1[0] = p1;
            g1[1] = p2;
            g1[2] = p3;
            g1[3] = p4;
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
            dt = 1.0 / n;
            for(int i = 1; i <= n; i++)
            {
                t = i * dt;
                for(int k = 3; k > 0; k--)
                {
                    for(int j = 0; j < k; j++)
                    {
                        g1[j].X = (int)((1.0 - t) * g1[j].X + t * g1[j + 1].X);
                        g1[j].Y = (int)((1.0 - t) * g1[j].Y + t * g1[j + 1].Y);
                    }
                }
                p = g1[0];
                g.DrawLine(MyPen, oldp, p);
                oldp = p;
            }
        }
    }
}
