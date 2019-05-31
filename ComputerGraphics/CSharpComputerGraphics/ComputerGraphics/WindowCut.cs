using System;
using System.Drawing;
using System.Windows.Forms;

namespace ComputerGraphics
{
    public partial class Form1 : Form
    {
        private void WindowCut_Click(object sender, EventArgs e)
        {
            MenuID = 24;
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

        private void EdgeClipping(int linecode)
        {
            float x, y;
            int n, i, number1;
            Point[] q = new Point[200];
            number1 = 0;
            if (linecode == 0)
            {
                for (n = 0; n < PressNum; n++)
                {
                    if (group[n].X < XL && group[n + 1].X < XL)
                    {
                    }
                    else if (group[n].X >= XL && group[n + 1].X >= XL)
                    {
                        q[number1++] = group[n + 1];
                    }
                    else if (group[n].X >= XL && group[n + 1].X < XL)
                    {
                        y = group[n].Y + (float)(group[n + 1].Y - group[n].Y) / (float)(group[n + 1].X - group[n].X) * (float)(XL - group[n].X);
                        q[number1].X = XL;
                        q[number1++].Y = (int)y;
                    }
                    else if (group[n].X < XL && group[n + 1].X >= XL)
                    {
                        y = group[n].Y + (float)(group[n + 1].Y - group[n].Y) / (float)(group[n + 1].X - group[n].X) * (float)(XL - group[n].X);
                        q[number1].X = XL;
                        q[number1++].Y = (int)y;
                        q[number1++] = group[n + 1];
                    }
                }

                for (i = 0; i < number1; i++)
                {
                    group[i] = q[i];
                }
                group[number1] = q[0];
                PressNum = number1;
            }

            if (linecode == 1)
            {
                for (n = 0; n < PressNum; n++)
                {
                    if (group[n].Y >= YU && group[n + 1].Y >= YU)
                    {
                    }
                    else if (group[n].Y < YU && group[n + 1].Y < YU)
                    {
                        q[number1++] = group[n + 1];
                    }
                    else if (group[n].Y < YU && group[n + 1].Y >= YU)
                    {
                        x = group[n].X + (float)(group[n + 1].X - group[n].X) / (float)(group[n + 1].Y - group[n].Y) * (float)(YU - group[n].Y);
                        q[number1].X = (int)x;
                        q[number1++].Y = YU;
                    }
                    else if (group[n].Y >= YU && group[n + 1].Y < YU)
                    {
                        x = group[n].X + (float)(group[n + 1].X - group[n].X) / (float)(group[n + 1].Y - group[n].Y) * (float)(YU - group[n].Y);
                        q[number1].X = (int)x;
                        q[number1++].Y = YU;
                        q[number1++] = group[n + 1];
                    }
                }

                for (i = 0; i < number1; i++)
                {
                    group[i] = q[i];
                }
                group[number1] = q[0];
                PressNum = number1;
            }

            if (linecode == 2)
            {
                for (n = 0; n < PressNum; n++)
                {
                    if (group[n].X >= XR && group[n + 1].X >= XR)
                    {
                    }
                    else if (group[n].X < XR && group[n + 1].X < XR)
                    {
                        q[number1++] = group[n + 1];
                    }
                    else if (group[n].X < XR && group[n + 1].X >= XR)
                    {
                        y = group[n].Y + (float)(group[n + 1].Y - group[n].Y) / (float)(group[n + 1].X - group[n].X) * (float)(XR - group[n].X);
                        q[number1].X = XR;
                        q[number1++].Y = (int)y;
                    }
                    else if (group[n].X >= XR && group[n + 1].X < XR)
                    {
                        y = group[n].Y + (float)(group[n + 1].Y - group[n].Y) / (float)(group[n + 1].X - group[n].X) * (float)(XR - group[n].X);
                        q[number1].X = XR;
                        q[number1++].Y = (int)y;
                        q[number1++] = group[n + 1];
                    }
                }

                for (i = 0; i < number1; i++)
                {
                    group[i] = q[i];
                }
                group[number1] = q[0];
                PressNum = number1;
            }

            if (linecode == 3)
            {
                for (n = 0; n < PressNum; n++)
                {
                    if (group[n].Y < YD && group[n + 1].Y < YD)
                    {
                    }
                    else if (group[n].Y >= YD && group[n + 1].Y >= YD)
                    {
                        q[number1++] = group[n + 1];
                    }
                    else if (group[n].Y >= YD && group[n + 1].Y < YD)
                    {
                        x = group[n].X + (float)(group[n + 1].X - group[n].X) / (float)(group[n + 1].Y - group[n].Y) * (float)(YD - group[n].Y);
                        q[number1].X = (int)x;
                        q[number1++].Y = YD;
                    }
                    else if (group[n].Y < YD && group[n + 1].Y >= YD)
                    {
                        x = group[n].X + (float)(group[n + 1].X - group[n].X) / (float)(group[n + 1].Y - group[n].Y) * (float)(YD - group[n].Y);
                        q[number1].X = (int)x;
                        q[number1++].Y = YD;
                        q[number1++] = group[n + 1];
                    }
                }

                for (i = 0; i < number1; i++)
                {
                    group[i] = q[i];
                }
                group[number1] = q[0];
                PressNum = number1;
            }
        }

        private void WindowCut1()
        {
            group[PressNum] = group[0];
            EdgeClipping(0);
            EdgeClipping(1);
            EdgeClipping(2);
            EdgeClipping(3);
            Graphics g = CreateGraphics();
            Pen MyPen = new Pen(Color.Yellow, 3);
            for (int i = 0; i < PressNum; i++)
            {
                g.DrawLine(MyPen, group[i], group[i + 1]);
            }
        }
    }
}
