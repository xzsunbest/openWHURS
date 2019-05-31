using System;
using System.Drawing;
using System.Windows.Forms;

namespace ComputerGraphics
{
    public partial class Form1 : Form
    {
        private void MidLine1(int x0, int y0, int x1, int y1)
        {
            int x, y, d, flag;
            Graphics g = CreateGraphics();
            if (x0 == x1 && y0 == y1) return;
            if (x0 == x1)
            {
                if (y0 > y1)
                {
                    Swap(ref y0, ref y1);
                }
                for (y = y0; y <= y1; y++)
                {
                    g.DrawRectangle(Pens.Red, x1, y, 1, 1);
                }
                return;
            }
            else if (y0 == y1)
            {
                if (x0 > x1)
                {
                    Swap(ref x0, ref x1);
                }
                for (x = x0; x <= x1; x++)
                {
                    g.DrawRectangle(Pens.Red, x, y0, 1, 1);
                }
                return;
            }
            if (x0 > x1)
            {
                Swap(ref x0, ref x1);
                Swap(ref y0, ref y1);
            }
            flag = 0;
            if (x1 - x0 > y1 - y0 && y1 - y0 > 0) flag = 1;
            else if (x1 - x0 > y0 - y1 && y0 - y1 > 0)
            {
                flag = 2;
                y0 = -y0; y1 = -y1;
            }
            else if (y1 - y0 > x1 - x0)
            {
                flag = 3;
                Swap(ref x0, ref y0);
                Swap(ref x1, ref y1);
            }
            else if (y0 - y1 > x1 - x0)
            {
                flag = 4;
                y0 = -y0; y1 = -y1;
                Swap(ref x0, ref y0);
                Swap(ref x1, ref y1);
            }
            x = x0; y = y0; d = (x1 - x0) - 2 * (y1 - y0);
            while (x < x1 + 1)
            {
                if (flag == 1) g.DrawRectangle(Pens.Red, x, y, 1, 1);
                if (flag == 2) g.DrawRectangle(Pens.Red, x, -y, 1, 1);
                if (flag == 3) g.DrawRectangle(Pens.Red, y, x, 1, 1);
                if (flag == 4) g.DrawRectangle(Pens.Red, y, -x, 1, 1);
                x++;
                if (d > 0)
                {
                    d = d - 2 * (y1 - y0);
                }
                else
                {
                    y++;
                    d = d - 2 * ((y1 - y0) - (x1 - x0));
                }
            }
        }

        private void MidLine_Click(object sender, EventArgs e)
        {
            MenuID = 2;
            PressNum = 0;
            Graphics g = CreateGraphics();
            g.Clear(BackColor1);
        }
    }
}
