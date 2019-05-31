using System;
using System.Drawing;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace ComputerGraphics
{
    public partial class Form1 : Form
    {
        [DllImport("gdi32.dll")]
        static extern int GetPixel(IntPtr hDC, int x, int y);
        [DllImport("gdi32.dll")]
        static extern int SetPixel(IntPtr hDC, int x, int y, int color);

        private void FloodFill_Click(object sender, EventArgs e)
        {
            MenuID = 33;
            PressNum = 0;
            Graphics g = CreateGraphics();
            g.Clear(BackColor1);
        }

        private void FloodFill1(int x0, int y0)
        {
            int savex, xleft, xright, pflag, x, y, num;
            Point[] stack_ptr = new Point[200];
            Graphics g = CreateGraphics();
            IntPtr hdc = g.GetHdc();
            IntPtr hpen = CreatePen(0, 1, ColorTranslator.ToWin32(Color.Red));
            SetROP2(hdc, 13);
            IntPtr pOldPen = SelectObject(hdc, hpen);
            num = 0;
            stack_ptr[num++] = new Point(x0, y0);
            while (num > 0)
            {
                x = stack_ptr[--num].X;
                y = stack_ptr[num].Y;
                SetPixel(hdc, x, y, ColorTranslator.ToWin32(Color.Red));
                savex = x;
                x++;
                while (GetPixel(hdc, x, y) != ColorTranslator.ToWin32(Color.Red))
                {
                    SetPixel(hdc, x++, y, ColorTranslator.ToWin32(Color.Red));
                }
                xright = x - 1;
                x = savex - 1;
                while (GetPixel(hdc, x, y) != ColorTranslator.ToWin32(Color.Red))
                {
                    SetPixel(hdc, x--, y, ColorTranslator.ToWin32(Color.Red));
                }
                xleft = x + 1;
                x = xleft;
                y++;
                pflag = 1;
                while (x < xright)
                {
                    if (GetPixel(hdc, x, y) != ColorTranslator.ToWin32(Color.Red) && pflag == 1)
                    {
                        stack_ptr[num].X = x;
                        stack_ptr[num++].Y = y;
                        x++;
                    }
                    if (GetPixel(hdc, x, y) == ColorTranslator.ToWin32(Color.Red))
                        pflag = 1;
                    else
                        pflag = 0;
                    x++;
                }
                x = xleft;
                y -= 2;
                pflag = 1;
                while (x < xright)
                {
                    if (GetPixel(hdc, x, y) != ColorTranslator.ToWin32(Color.Red) && pflag == 1)
                    {
                        stack_ptr[num].X = x;
                        stack_ptr[num++].Y = y;
                        x++;
                    }
                    if (GetPixel(hdc, x, y) == ColorTranslator.ToWin32(Color.Red))
                        pflag = 1;
                    else
                        pflag = 0;
                    x++;
                }
            }
            SelectObject(hdc, pOldPen);
            g.ReleaseHdc(hdc);
        }
    }
}
