using System;
using System.Drawing;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace ComputerGraphics
{
    public partial class Form1 : Form
    {
        [DllImport("gdi32.dll")]
        static extern IntPtr CreatePen(int fnPenStyle, int width, int color);
        [DllImport("gdi32.dll")]
        static extern int SetROP2(IntPtr hdc, int rop);
        [DllImport("gdi32.dll")]
        static extern int MoveToEx(IntPtr hdc, int x, int y, IntPtr lppoint);
        [DllImport("gdi32.dll")]
        static extern int LineTo(IntPtr hdc, int X, int Y);
        [DllImport("gdi32.dll")]
        static extern IntPtr SelectObject(IntPtr hdc, IntPtr obj);

        private void EdgeFill_Click(object sender, EventArgs e)
        {
            MenuID = 32;
            PressNum = 0;
            Graphics g = CreateGraphics();
            g.Clear(BackColor1);
        }

        private void EdgeFill1()
        {
            int xr, x1, y1, x2, y2, y;
            float m, x;

            Graphics g = CreateGraphics();
            IntPtr hdc = g.GetHdc();
            IntPtr hpen = CreatePen(0, 1, ColorTranslator.ToWin32(Color.Red));
            SetROP2(hdc, 7);
            IntPtr pOldPen = SelectObject(hdc, hpen);
            xr = 0;
            for (int i = 0; i < PressNum; i++)
            {
                xr = Math.Max(xr, group[i].X);
            }
            for (int i = 0; i < PressNum; i++)
            {
                x1 = group[i].X;
                y1 = group[i].Y;
                if (i == PressNum - 1)
                {
                    x2 = group[0].X;
                    y2 = group[0].Y;
                }
                else
                {
                    x2 = group[i + 1].X;
                    y2 = group[i + 1].Y;
                }
                if (y1 != y2)
                {
                    if (y1 > y2)
                    {
                        Swap(ref y1, ref y2);
                        Swap(ref x1, ref x2);
                    }
                    m = (float)(x2 - x1) / (float)(y2 - y1);
                    x = x1;
                    for (y = y1 + 1; y <= y2; y++)
                    {
                        x += m;
                        MoveToEx(hdc, (int)x, y, IntPtr.Zero);
                        LineTo(hdc, xr, y);
                    }
                }
            }
            SelectObject(hdc, pOldPen);
            g.ReleaseHdc(hdc);
        }
    }
}
