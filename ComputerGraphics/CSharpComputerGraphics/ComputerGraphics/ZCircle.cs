using System;
using System.Drawing;
using System.Windows.Forms;

namespace ComputerGraphics
{
    public partial class Form1 : Form
    {
        private void ZCircle_Click(object sender, EventArgs e)
        {
            MenuID = 6;
            PressNum = 0;
            Graphics g = CreateGraphics();
            g.Clear(BackColor1);
        }

        private void ZCircle1(int x1, int y1, int x2, int y2)
        {
            int r, d, x, y, x0, y0;
            Graphics g = CreateGraphics();
            r = (int)(Math.Sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) + 0.5);
            d = 0; x0 = x1; y0 = y1; x = x0; y = y0 + r;
            while (y > y0)
            {
                g.DrawRectangle(Pens.Red, x, y, 1, 1);
                g.DrawRectangle(Pens.Red, -x + 2 * x0, y, 1, 1);
                g.DrawRectangle(Pens.Red, x, -y + 2 * y0, 1, 1);
                g.DrawRectangle(Pens.Red, -x + 2 * x0, -y + 2 * y0, 1, 1);
                if (d < 0)
                {
                    x++;
                    d = d + 2 * (x - x0) + 1;
                }
                else
                {
                    y--;
                    d = d - 2 * (y - y0) + 1;
                }
            }
        }
    }
}
