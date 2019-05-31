using System;
using System.Drawing;
using System.Windows.Forms;

namespace ComputerGraphics
{
    public partial class Form1 : Form
    {
        private void CohenCut_Click(object sender, EventArgs e)
        {
            MenuID = 21;
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

        private int Encode(int x, int y)
        {
            int code = 0;
            if (x >= XL && x <= XR && y >= YD && y <= YU) code = 0;
            if (x < XL && y >= YD && y <= YU) code = 1;
            if (x > XR && y >= YD && y <= YU) code = 2;
            if (x >= XL && x <= XR && y > YU) code = 8;
            if (x >= XL && x <= XR && y < YD) code = 4;
            if (x <= XL && y > YU) code = 9;
            if (x >= XR && y > YU) code = 10;
            if (x <= XL && y < YD) code = 5;
            if (x >= XR && y < YD) code = 6;
            return code;
        }

        private void CohenCut1(int x1, int y1, int x2, int y2)
        {
            int code1 = 0, code2 = 0, code, x = 0, y = 0;
            Graphics g = CreateGraphics();
            g.DrawLine(Pens.Red, x1, y1, x2, y2);
            code1 = Encode(x1, y1);
            code2 = Encode(x2, y2);
            while (code1 != 0 || code2 != 0)
            {
                if ((code1 & code2) != 0) return;
                code = code1;
                if (code1 == 0) code = code2;
                if ((1 & code) != 0)
                {
                    x = XL;
                    y = y1 + (y2 - y1) * (x - x1) / (x2 - x1);
                }
                if ((2 & code) != 0)
                {
                    x = XR;
                    y = y1 + (y2 - y1) * (x - x1) / (x2 - x1);
                }
                if ((4 & code) != 0)
                {
                    y = YD;
                    x = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
                }
                if ((8 & code) != 0)
                {
                    y = YU;
                    x = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
                }
                if (code == code1)
                {
                    x1 = x;
                    y1 = y;
                    code1 = Encode(x, y);
                }
                else
                {
                    x2 = x;
                    y2 = y;
                    code2 = Encode(x, y);
                }
            }
            Pen MyPen = new Pen(Color.Yellow, 3);
            g.DrawLine(MyPen, x1, y1, x2, y2);
        }
    }
}
