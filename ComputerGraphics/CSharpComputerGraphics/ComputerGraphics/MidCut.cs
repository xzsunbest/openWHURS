using System;
using System.Drawing;
using System.Windows.Forms;

namespace ComputerGraphics
{
    public partial class Form1 : Form
    {
        private void MidCut_Click(object sender, EventArgs e)
        {
            MenuID = 22;
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

        private bool LineIsOutOfWindow(int x1, int y1, int x2, int y2)
        {
            if (x1 < XL && x2 < XL) return true;
            else if (x1 > XR && x2 > XR) return true;
            else if (y1 > YU && y2 > YU) return true;
            else if (y1 < YD && y2 < YD) return true;
            else return false;
        }

        private bool PointIsOutOfWindow(int x, int y)
        {
            if (x < XL) return true;
            else if (x > XR) return true;
            else if (y > YU) return true;
            else if (y < YD) return true;
            else return false;
        }

        private Point FindNearestPoint(int x1, int y1, int x2, int y2)
        {
            int x = 0, y = 0;
            Point p = new Point(0, 0);
            if (!PointIsOutOfWindow(x1, y1))
            {
                p.X = x1;
                p.Y = y1;
                return p;
            }
            while (!(Math.Abs(x1 - x2) <= 1 && Math.Abs(y1 - y2) <= 1))
            {
                x = (x1 + x2) / 2;
                y = (y1 + y2) / 2;
                if (LineIsOutOfWindow(x1, y1, x, y))
                {
                    x1 = x;
                    y1 = y;
                }
                else
                {
                    x2 = x;
                    y2 = y;
                }
            }
            if (PointIsOutOfWindow(x1, y1))
            {
                p.X = x2;
                p.Y = y2;
            }
            else
            {
                p.X = x1;
                p.Y = y1;
            }
            return p;
        }

        private void MidCut1(int x1, int y1, int x2, int y2)
        {
            Graphics g = CreateGraphics();
            g.DrawLine(Pens.Red, x1, y1, x2, y2);
            Point p1, p2;
            if (LineIsOutOfWindow(x1, y1, x2, y2)) return;
            p1 = FindNearestPoint(x1, y1, x2, y2);
            if (PointIsOutOfWindow(p1.X, p1.Y)) return;
            p2 = FindNearestPoint(x2, y2, x1, y1);
            Pen MyPen = new Pen(Color.Yellow, 3);
            g.DrawLine(MyPen, p1, p2);
        }
    }
}
