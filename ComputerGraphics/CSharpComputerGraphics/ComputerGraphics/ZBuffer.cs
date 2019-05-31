using System;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace ComputerGraphics
{
    public partial class Form1 : Form
    {
        public struct My3DPolygon
        {
            public Color pcolor;
            public int number;
            public Point3D[] Points;
        }
        My3DPolygon[] PGroup = new My3DPolygon[6];

        private void ZBuffer_Click(object sender, EventArgs e)
        {
            MenuID = 53;
            ReadPolygon();
            ZBufferDraw();
        }

        private void ReadPolygon()
        {
            PGroup[0].pcolor = Color.Red;
            PGroup[0].number = 4;
            PGroup[0].Points = new Point3D[4];
            PGroup[0].Points[0] = new Point3D(150, 130, 310);
            PGroup[0].Points[1] = new Point3D(340, 150, 540);
            PGroup[0].Points[2] = new Point3D(380, 410, 1100);
            PGroup[0].Points[3] = new Point3D(160, 380, 820);

            PGroup[1].pcolor = Color.Green;
            PGroup[1].number = 5;
            PGroup[1].Points = new Point3D[5];
            PGroup[1].Points[0] = new Point3D(160, 120, 440);
            PGroup[1].Points[1] = new Point3D(410, 110, 930);
            PGroup[1].Points[2] = new Point3D(520, 550, 1570);
            PGroup[1].Points[3] = new Point3D(350, 430, 1130);
            PGroup[1].Points[4] = new Point3D(170, 220, 560);

            PGroup[2].pcolor = Color.Blue;
            PGroup[2].number = 6;
            PGroup[2].Points = new Point3D[6];
            PGroup[2].Points[0] = new Point3D(600, 600, 1300);
            PGroup[2].Points[1] = new Point3D(770, 500, 1370);
            PGroup[2].Points[2] = new Point3D(960, 420, 1480);
            PGroup[2].Points[3] = new Point3D(830, 300, 1230);
            PGroup[2].Points[4] = new Point3D(720, 230, 1050);
            PGroup[2].Points[5] = new Point3D(650, 210, 960);

            PGroup[3].pcolor = Color.Yellow;
            PGroup[3].number = 6;
            PGroup[3].Points = new Point3D[6];
            PGroup[3].Points[0] = new Point3D(220, 200, 710);
            PGroup[3].Points[1] = new Point3D(350, 340, 1055);
            PGroup[3].Points[2] = new Point3D(590, 660, 1815);
            PGroup[3].Points[3] = new Point3D(300, 600, 1550);
            PGroup[3].Points[4] = new Point3D(180, 320, 930);
            PGroup[3].Points[5] = new Point3D(150, 220, 715);

            PGroup[4].pcolor = Color.SkyBlue;
            PGroup[4].number = 6;
            PGroup[4].Points = new Point3D[6];
            PGroup[4].Points[0] = new Point3D(600, 680, 2260);
            PGroup[4].Points[1] = new Point3D(300, 670, 1840);
            PGroup[4].Points[2] = new Point3D(200, 520, 1440);
            PGroup[4].Points[3] = new Point3D(190, 430, 1250);
            PGroup[4].Points[4] = new Point3D(380, 220, 1020);
            PGroup[4].Points[5] = new Point3D(540, 300, 1340);

            PGroup[5].pcolor = Color.Brown;
            PGroup[5].number = 6;
            PGroup[5].Points = new Point3D[6];
            PGroup[5].Points[0] = new Point3D(400, 670, 1470);
            PGroup[5].Points[1] = new Point3D(680, 610, 1690);
            PGroup[5].Points[2] = new Point3D(690, 340, 1430);
            PGroup[5].Points[3] = new Point3D(500, 210, 1110);
            PGroup[5].Points[4] = new Point3D(200, 180, 780);
            PGroup[5].Points[5] = new Point3D(400, 320, 1120);
        }

        private void ZBufferDraw()
        {
            double[,] Z = new double[1000, 800];
            for (int i = 0; i < 1000; i++)
                for (int j = 0; j < 800; j++)
                    Z[i, j] = 0.0;
            for (int i = 0; i < 6; i++)
            {
                ZBufferPolygon(PGroup[i], Z);
            }
        }

        private void ZBufferPolygon(My3DPolygon PG, double[,] Z)
        {
            if (!GetPolygen(PG))
            {
                return;
            }
            Point[] g1 = new Point[100];
            EdgeInfo[] edgelist = new EdgeInfo[100];
            int j = 0, yu = 0, yd = 1024;
            for (int i = 0; i < PG.number; i++)
            {
                g1[i].X = (int)(PG.Points[i].X + 0.5);
                g1[i].Y = (int)(PG.Points[i].Y + 0.5);
            }
            g1[PG.number].X = (int)(PG.Points[0].X + 0.5);
            g1[PG.number].Y = (int)(PG.Points[0].Y + 0.5);

            for (int i = 0; i < PG.number; i++)
            {
                if (g1[i].Y > yu) yu = g1[i].Y;
                if (g1[i].Y < yd) yd = g1[i].Y;
                if (g1[i].Y != g1[i + 1].Y)
                {
                    if (g1[i].Y > g1[i + 1].Y)
                    {
                        edgelist[j++] = new EdgeInfo(g1[i + 1].X, g1[i + 1].Y, g1[i].X, g1[i].Y);
                    }
                    else
                    {
                        edgelist[j++] = new EdgeInfo(g1[i].X, g1[i].Y, g1[i + 1].X, g1[i + 1].Y);
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
                        Pen p = new Pen(PG.pcolor);
                        for (int x = FirstX; x < (int)(item.XMin + 0.5); x++)
                        {
                            double z = A * x + B * y + C;
                            if (z > Z[x, y])
                            {
                                Z[x, y] = z;
                                g.DrawRectangle(p, x, y, 1, 1);
                            }
                        }
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

        private bool GetPolygen(My3DPolygon PG)
        {
            int d;
            int i = 0;
            do
            {
                d = (int)((PG.Points[i].X - PG.Points[i + 2].X) * (PG.Points[i].Y - PG.Points[i + 1].Y)
                    - (PG.Points[i].X - PG.Points[i + 1].X) * (PG.Points[i].Y - PG.Points[i + 2].Y));
                i++;
            } while (d == 0 || i + 2 < PG.number);
            if (d != 0)
            {
                i--;
                A = ((PG.Points[i].Z - PG.Points[i + 2].Z) * (PG.Points[i].Y - PG.Points[i + 1].Y)
                    - (PG.Points[i].Z - PG.Points[i + 1].Z) * (PG.Points[i].Y - PG.Points[i + 2].Y)) / d;
                B = ((PG.Points[i].Z - PG.Points[i + 1].Z) * (PG.Points[i].X - PG.Points[i + 2].X)
                    - (PG.Points[i].Z - PG.Points[i + 2].Z) * (PG.Points[i].X - PG.Points[i + 1].X)) / d;
                C = PG.Points[i].Z - A * PG.Points[i].X - B * PG.Points[i].Y;
                return true;
            }
            else
                return false;
        }
    }
}