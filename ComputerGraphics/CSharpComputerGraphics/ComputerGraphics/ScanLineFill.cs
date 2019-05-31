using System;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;

namespace ComputerGraphics
{
    public partial class Form1 : Form
    {
        public struct EdgeInfo
        {
            int ymax, ymin;
            float k, xmin;
            public int YMax { get { return ymax; } set { ymax = value; } }
            public int YMin { get { return ymin; } set { ymin = value; } }
            public float XMin { get { return xmin; } set { xmin = value; } }
            public float K { get { return k; } set { k = value; } }
            public EdgeInfo(int x1, int y1, int x2, int y2)
            {
                ymax = y2;
                ymin = y1;
                xmin = (float)x1;
                k = (float)(x1 - x2) / (float)(y1 - y2);
            }
        }

        private void ScanLineFill_Click(object sender, EventArgs e)
        {
            MenuID = 31;
            PressNum = 0;
            Graphics g = CreateGraphics();
            g.Clear(BackColor1);
        }

        private void ScanLineFill1()
        {
            EdgeInfo[] edgelist = new EdgeInfo[100];
            int j = 0, yu = 0, yd = 1024;
            group[PressNum] = group[0];
            for (int i = 0; i < PressNum; i++)
            {
                if (group[i].Y > yu) yu = group[i].Y;
                if (group[i].Y < yd) yd = group[i].Y;
                if (group[i].Y != group[i + 1].Y)
                {
                    if (group[i].Y > group[i + 1].Y)
                    {
                        edgelist[j++] = new EdgeInfo(group[i + 1].X, group[i + 1].Y, group[i].X, group[i].Y);
                    }
                    else
                    {
                        edgelist[j++] = new EdgeInfo(group[i].X, group[i].Y, group[i + 1].X, group[i + 1].Y);
                    }
                }
            }
            Graphics g = CreateGraphics();
            for (int y = yd; y < yu; y++)
            {
                var sorted =
                    from item in edgelist
                    where y < item.YMax && y >= item.YMin
                    orderby item.XMin, item.K
                    select item;
                int flag = 0;
                foreach (var item in sorted)
                {
                    if (flag == 0)
                    {
                        FirstX = (int)(item.XMin + 0.5);
                        flag++;
                    }
                    else
                    {
                        g.DrawLine(Pens.Blue, (int)(item.XMin + 0.5), y, FirstX - 1, y);
                        flag = 0;
                    }
                }
                for (int i = 0; i < j; i++)
                {
                    if (y < edgelist[i].YMax - 1 && y > edgelist[i].YMin)
                    {
                        edgelist[i].XMin += edgelist[i].K;
                    }
                }
            }
        }
    }
}
