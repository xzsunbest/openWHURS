using System;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.IO;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace ComputerGraphics
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        Color BackColor1 = Color.White;
        Color ForeColor1 = Color.Black;
        public const double PI = 3.141592653589793;
        public const int H = 512;
        public const int W = 512;
        public const int N = 64;
        public const int MAX_STEP = 64;
        public const int MAX_DEPTH = 3;
        public const float MAX_DISTANCE = 2.0f;
        public const float EPSILON = 1e-6f;
        public const float BIAS = 1e-4f;
        public int MenuID, PressNum, PointNum, SaveNumber, FirstX, FirstY, OldX, OldY, XL, XR, YU, YD;
        private static double A, B, C;

        Point[] group = new Point[100];
        Point[] pointsgroup = new Point[4];
        Point3D[] modegroup = new Point3D[8];

        private static void Swap<T>(ref T a, ref T b)
        {
            T t = a;
            a = b;
            b = t;
        }

        private Point3D InputProjectionDirection()
        {
            Form MyInputForm = new Form();
            Label label1 = new Label();
            Label label2 = new Label();
            Label label3 = new Label();
            label1.Text = "X";
            label2.Text = "Y";
            label3.Text = "Z";
            label1.Location = new Point(10, 10);
            label2.Location = new Point(10, 40);
            label3.Location = new Point(10, 70);
            NumericUpDown numericUpDown1 = new NumericUpDown();
            NumericUpDown numericUpDown2 = new NumericUpDown();
            NumericUpDown numericUpDown3 = new NumericUpDown();
            numericUpDown1.Minimum = 0; numericUpDown1.Maximum = 500;
            numericUpDown1.Increment = 10; numericUpDown1.Value = 150;
            numericUpDown2.Minimum = 0; numericUpDown2.Maximum = 500;
            numericUpDown2.Increment = 10; numericUpDown2.Value = 150;
            if (MenuID == 41)
            {
                numericUpDown3.Minimum = -800; numericUpDown3.Maximum = -300;
                numericUpDown3.Increment = 10; numericUpDown3.Value = -500;
            }
            else if (MenuID == 42)
            {
                numericUpDown3.Minimum = 300; numericUpDown3.Maximum = 800;
                numericUpDown3.Increment = 10; numericUpDown3.Value = 500;
            }
            numericUpDown1.Location = new Point(120, 10);
            numericUpDown2.Location = new Point(120, 40);
            numericUpDown3.Location = new Point(120, 70);
            Button button1 = new Button();
            Button button2 = new Button();
            button1.Text = "确定";
            button1.DialogResult = DialogResult.OK;
            button2.Text = "取消";
            button2.DialogResult = DialogResult.Cancel;
            button1.Location = new Point(10, 100);
            button2.Location = new Point(button1.Width + 20, 100);
            if (MenuID == 41) MyInputForm.Text = "请输入平行投影方向向量";
            else if (MenuID == 42) MyInputForm.Text = "请输入透视投影方向向量";
            MyInputForm.HelpButton = true;
            MyInputForm.FormBorderStyle = FormBorderStyle.FixedDialog;
            MyInputForm.MaximizeBox = false;
            MyInputForm.MinimizeBox = false;
            MyInputForm.AcceptButton = button1;
            MyInputForm.CancelButton = button2;
            MyInputForm.StartPosition = FormStartPosition.CenterScreen;
            MyInputForm.Controls.Add(label1);
            MyInputForm.Controls.Add(label2);
            MyInputForm.Controls.Add(label3);
            MyInputForm.Controls.Add(numericUpDown1);
            MyInputForm.Controls.Add(numericUpDown2);
            MyInputForm.Controls.Add(numericUpDown3);
            MyInputForm.Controls.Add(button1);
            MyInputForm.Controls.Add(button2);
            Point3D vp = new Point3D(150, 150, -500);
            if (MyInputForm.ShowDialog() == DialogResult.Cancel)
            {
                MyInputForm.Close();
                return vp;
            }
            vp = new Point3D((float)numericUpDown1.Value, (float)numericUpDown2.Value, (float)numericUpDown3.Value);
            MyInputForm.Close();
            return vp;
        }

        private void Projection1()
        {
            modegroup[0] = new Point3D(100, 100, 100);
            modegroup[1] = new Point3D(200, 100, 100);
            modegroup[2] = new Point3D(200, 200, 100);
            modegroup[3] = new Point3D(100, 200, 100);
            modegroup[4] = new Point3D(100, 100, 200);
            modegroup[5] = new Point3D(200, 100, 200);
            modegroup[6] = new Point3D(200, 200, 200);
            modegroup[7] = new Point3D(100, 200, 200);
            Point3D vp = new Point3D(50, 30, 500);
            if (MenuID == 41 || MenuID == 42) vp = InputProjectionDirection();
            for (int i = 0; i < 8; i++)
            {
                if (MenuID == 41) group[i] = ParallelP(vp, modegroup[i]);
                else if (MenuID == 42) group[i] = PerspectiveP(vp, modegroup[i]);
                else if (MenuID == 43) group[i] = SimpleP(modegroup[i]);
            }             
            Graphics g = CreateGraphics();
            g.Clear(BackColor1);
            g.DrawLine(Pens.Red, group[0], group[1]);
            g.DrawLine(Pens.Red, group[1], group[2]);
            g.DrawLine(Pens.Red, group[2], group[3]);
            g.DrawLine(Pens.Red, group[3], group[0]);
            g.DrawLine(Pens.Red, group[4], group[5]);
            g.DrawLine(Pens.Red, group[5], group[6]);
            g.DrawLine(Pens.Red, group[6], group[7]);
            g.DrawLine(Pens.Red, group[7], group[4]);
            g.DrawLine(Pens.Red, group[0], group[4]);
            g.DrawLine(Pens.Red, group[1], group[5]);
            g.DrawLine(Pens.Red, group[2], group[6]);
            g.DrawLine(Pens.Red, group[3], group[7]);
        }

        private int[,] ReadDEM()
        {
            int[,] D = new int[200, 200];
            FileStream fp = new FileStream("DEM.DAT", FileMode.Open, FileAccess.Read);
            BinaryReader r = new BinaryReader(fp);
            for (int i = 0; i < 200; i++)
                for (int j = 0; j < 200; j++)
                    D[i, j] = r.ReadByte();
            return D;
        }

        private void DrawPixel(Graphics g, int dx, int dy, int size, int x, int y, int z)
        {
            x = x + 400;
            y = -y + 400;
            Point[] pts = new Point[4];
            pts[0].X = x - dx;
            pts[0].Y = y + dy;
            pts[1].X = x - dx;
            pts[1].Y = y + dy - z;
            pts[2].X = x - dx + size;
            pts[2].Y = y + dy - z;
            pts[3].X = x - dx + size;
            pts[3].Y = y + dy;
            g.FillPolygon(Brushes.White, pts);
            g.DrawPolygon(Pens.Black, pts);
            pts[0].X = x;
            pts[0].Y = y - z;
            pts[1].X = x - dx;
            pts[1].Y = y + dy - z;
            pts[2].X = x - dx + size;
            pts[2].Y = y + dy - z;
            pts[3].X = x + size;
            pts[3].Y = y - z;
            g.FillPolygon(Brushes.White, pts);
            g.DrawPolygon(Pens.Black, pts);
            pts[0].X = x + size;
            pts[0].Y = y;
            pts[1].X = x;
            pts[1].Y = y - z;
            pts[2].X = x - dx + size;
            pts[2].Y = y + dy - z;
            pts[3].X = x - dx + size;
            pts[3].Y = y + dy;
            g.FillPolygon(Brushes.White, pts);
            g.DrawPolygon(Pens.Black, pts);
        }

        private void Form1_MouseMove(object sender, MouseEventArgs e)
        {
            Graphics g = CreateGraphics();
            Pen BackPen = new Pen(BackColor1, 1);
            Pen MyPen = new Pen(ForeColor1, 1);
            if ((MenuID == 1 || MenuID == 2) && PressNum == 1)
            {
                if (!(e.X == OldX && e.Y == OldY))
                {
                    g.DrawLine(BackPen, FirstX, FirstY, OldX, OldY);
                    g.DrawLine(MyPen, FirstX, FirstY, e.X, e.Y);
                    OldX = e.X;
                    OldY = e.Y;
                }
            }
            if ((MenuID == 5 || MenuID == 6) && PressNum == 1)
            {
                if (!(e.X == OldX && e.Y == OldY))
                {
                    double r = Math.Sqrt((FirstX - OldX) * (FirstX - OldX) + (FirstY - OldY) * (FirstY - OldY));
                    int r1 = (int)(r + 0.5);
                    g.DrawEllipse(BackPen, FirstX - r1, FirstY - r1, 2 * r1, 2 * r1);
                    r = Math.Sqrt((FirstX - e.X) * (FirstX - e.X) + (FirstY - e.Y) * (FirstY - e.Y));
                    r1 = (int)(r + 0.5);
                    g.DrawEllipse(MyPen, FirstX - r1, FirstY - r1, 2 * r1, 2 * r1);
                    OldX = e.X;
                    OldY = e.Y;
                }
            }
            if ((MenuID == 31 || MenuID == 24 || MenuID == 32) && PressNum > 0)
            {
                if (!(e.X == OldX && e.Y == OldY))
                {
                    g.DrawLine(BackPen, group[PressNum - 1].X, group[PressNum - 1].Y, OldX, OldY);
                    g.DrawLine(MyPen, group[PressNum - 1].X, group[PressNum - 1].Y, e.X, e.Y);
                    OldX = e.X;
                    OldY = e.Y;
                }
            }
            if ((MenuID == 107 || MenuID == 108 || MenuID == 109) && PressNum > 0)
            {
                if (!(group[SaveNumber].X == e.X && group[SaveNumber].Y == e.Y))
                {
                    g.DrawLine(BackPen, group[SaveNumber].X - 5, group[SaveNumber].Y, group[SaveNumber].X + 5, group[SaveNumber].Y);
                    g.DrawLine(BackPen, group[SaveNumber].X, group[SaveNumber].Y - 5, group[SaveNumber].X, group[SaveNumber].Y + 5);
                    if (MenuID == 107) Bezier1(0);
                    else if (MenuID == 108) BSample1(0);
                    else if (MenuID == 109) Hermite1(0);
                    g.DrawLine(MyPen, e.X - 5, e.Y, e.X + 5, e.Y);
                    g.DrawLine(MyPen, e.X, e.Y - 5, e.X, e.Y + 5);
                    group[SaveNumber].X = e.X;
                    group[SaveNumber].Y = e.Y;
                    if (MenuID == 107) Bezier1(1);
                    else if (MenuID == 108) BSample1(1);
                    else if (MenuID == 109) Hermite1(1);
                }
            }
        }

        private void Form1_MouseClick(object sender, MouseEventArgs e)
        {
            Graphics g = CreateGraphics();
            Pen MyPen = new Pen(Color.Red, 1);
            if (MenuID == 1 || MenuID == 2)
            {
                if (PressNum == 0)
                {
                    FirstX = e.X;
                    FirstY = e.Y;
                    OldX = e.X;
                    OldY = e.Y;
                }
                else
                {
                    if (MenuID == 1)
                    {
                        DDALine1(FirstX, FirstY, e.X, e.Y);
                    }
                    else if (MenuID == 2)
                    {
                        MidLine1(FirstX, FirstY, e.X, e.Y);
                    }
                }
                PressNum++;
                if (PressNum >= 2) PressNum = 0;
            }
            if (MenuID == 5 || MenuID == 6)
            {
                if (PressNum == 0)
                {
                    FirstX = e.X;
                    FirstY = e.Y;
                }
                else
                {
                    if (FirstX == e.X && FirstY == e.Y) return;
                    if (MenuID == 5) BresenhamCircle1(FirstX, FirstY, e.X, e.Y);
                    else if (MenuID == 6) ZCircle1(FirstX, FirstY, e.X, e.Y);
                }
                PressNum++;
                if (PressNum >= 2) PressNum = 0;
            }
            if (MenuID == 24 || MenuID == 31 || MenuID == 32 || MenuID == 33)
            {
                if (e.Button == MouseButtons.Left)
                {
                    group[PressNum].X = e.X;
                    group[PressNum].Y = e.Y;
                    if (PressNum > 0)
                    {
                        g.DrawLine(Pens.Red, group[PressNum - 1], group[PressNum]);
                    }
                    PressNum++;
                }
                if (e.Button == MouseButtons.Right)
                {
                    g.DrawLine(Pens.Red, group[PressNum - 1], group[0]);
                    if (MenuID == 31) ScanLineFill1();
                    else if (MenuID == 24) WindowCut1();
                    else if (MenuID == 32) EdgeFill1();
                    else if (MenuID == 33) MenuID = 106;
                    PressNum = 0;
                }
            }
            if (MenuID == 21 || MenuID == 22 || MenuID == 23)
            {
                if (PressNum == 0)
                {
                    FirstX = e.X;
                    FirstY = e.Y;
                    PressNum++;
                }
                else
                {
                    if (MenuID == 21) CohenCut1(FirstX, FirstY, e.X, e.Y);
                    else if (MenuID == 22) MidCut1(FirstX, FirstY, e.X, e.Y);
                    else if (MenuID == 23) LiangCut1(FirstX, FirstY, e.X, e.Y);
                    PressNum = 0;
                }
            }
            if (MenuID == 7 || MenuID == 8 || MenuID == 10)
            {
                if (e.Button == MouseButtons.Left)
                {
                    group[PointNum].X = e.X;
                    group[PointNum++].Y = e.Y;
                    g.DrawLine(Pens.Black, e.X - 5, e.Y, e.X + 5, e.Y);
                    g.DrawLine(Pens.Black, e.X, e.Y - 5, e.X, e.Y + 5);
                    PressNum = 1;
                }
                if (e.Button == MouseButtons.Right && PointNum > 3)
                {
                    if (MenuID == 7)
                    {
                        Bezier1(1);
                        MenuID = 107;
                    }
                    else if (MenuID == 8)
                    {
                        BSample1(1);
                        MenuID = 108;
                    }
                    else if (MenuID == 10)
                    {
                        Hermite1(1);
                        MenuID = 109;
                    }
                    PressNum = 0;
                }
            }
            if (MenuID == 107 || MenuID == 108 || MenuID == 109)
            {
                if (e.Button == MouseButtons.Left && PressNum == 0)
                {
                    for (int i = 0; i < PointNum; i++)
                    {
                        if ((e.X >= group[i].X - 5) && (e.X >= group[i].X + 5) && (e.Y >= group[i].Y - 5) && (e.Y >= group[i].Y + 5))
                        {
                            SaveNumber = i;
                            PressNum = 1;
                        }
                    }
                }
                if (e.Button == MouseButtons.Right && PointNum > 3)
                {
                    PressNum = 0;
                }
            }
            if (MenuID == 11)
            {
                if (PressNum == 0)
                {
                    FirstX = e.X;
                    FirstY = e.Y;
                }
                else
                {
                    for(int i = 0; i < 4; i++)
                    {
                        pointsgroup[i].X += e.X - FirstX;
                        pointsgroup[i].Y += e.Y - FirstY;
                    }
                    g.DrawPolygon(Pens.Blue, pointsgroup);
                }
                PressNum++;
                if (PressNum >= 2) PressNum = 0;
            }
            if (MenuID == 12)
            {
                if (PressNum == 0)
                {
                    FirstX = e.X;
                    FirstY = e.Y;
                }
                else
                {
                    double a;
                    if (e.X == FirstX && e.Y == FirstY) return;
                    if (e.X == FirstX && e.Y > FirstY) a = PI / 2.0;
                    else if (e.X == FirstX && e.Y < FirstY) a = 3.0 * PI / 2.0;
                    else a = Math.Atan((double)(e.Y - FirstY) / (double)(e.X - FirstX));
                    a = 180 * a / PI;
                    int x0 = 150, y0 = 150;
                    Matrix myMatrix = new Matrix();
                    myMatrix.Translate(-x0, -y0);
                    myMatrix.Rotate((float)a, MatrixOrder.Append);
                    myMatrix.Translate(x0, y0, MatrixOrder.Append);
                    g.Transform = myMatrix;
                    g.DrawPolygon(Pens.Blue, pointsgroup);
                }
                PressNum++;
                if (PressNum >= 2) PressNum = 0;
            }
            if (MenuID == 14 || MenuID == 15)
            {
                if (PressNum == 0)
                {
                    FirstX = e.X;
                    FirstY = e.Y;
                }
                else
                {
                    g.DrawLine(Pens.CadetBlue, FirstX, FirstY, e.X, e.Y);
                    if (MenuID == 14) TransSymmetry1(FirstX, FirstY, e.X, e.Y);
                    else if (MenuID == 15) TransShear1(FirstX, FirstY, e.X, e.Y);
                }
                PressNum++;
                if (PressNum >= 2) PressNum = 0;
            }
        }

        private void Form1_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            Graphics g = CreateGraphics();
            Pen MyPen = new Pen(Color.White, 1);
            if (MenuID == 106)
            {
                FloodFill1(e.X, e.Y);
                MenuID = 33;
            }
            if (MenuID == 107 || MenuID == 108 || MenuID == 109)
            {
                for (int i = 0; i < PointNum; i++)
                {
                    g.DrawLine(MyPen, group[i].X - 5, group[i].Y, group[i].X + 5, group[i].Y);
                    g.DrawLine(MyPen, group[i].X, group[i].Y - 5, group[i].X, group[i].Y + 5);
                }
                if (MenuID == 107)
                {
                    Bezier1(2);
                    MenuID = 7;
                }
                else if (MenuID == 108)
                {
                    BSample1(2);
                    MenuID = 8;
                }
                else if (MenuID == 109)
                {
                    Hermite1(2);
                    MenuID = 10;
                }
                PressNum = 0;
                PointNum = 0;
            }
        }
    }
}
