using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace ComputerGraphics
{
    public partial class Form1 : Form
    {
        public struct Result
        {
            float sd, emissive, reflectivity;
            public float Sd { get { return sd; }set { sd = value; } }
            public float Emissive { get { return emissive; } set { emissive = value; } }
            public float Reflectivity { get { return reflectivity; } set { reflectivity = value; } }
            public Result(float s,float e)
            {
                sd = s;
                emissive = e;
                reflectivity = 0.0f;
            }
            public Result(float s,float e,float r)
            {
                sd = s;
                emissive = e;
                reflectivity = r;
            }
        }

        public Random rd = new Random();

        private void UniformSampling_Click(object sender, EventArgs e)
        {
            MenuID = 61;
            Bitmap bmp = new Bitmap(W, H, PixelFormat.Format24bppRgb);
            Rectangle rect = new Rectangle(0, 0, bmp.Width, bmp.Height);
            BitmapData bmpData =
                bmp.LockBits(rect, ImageLockMode.ReadWrite,
                bmp.PixelFormat);
            IntPtr ptr = bmpData.Scan0;
            int bytes = bmpData.Stride * bmp.Height;
            byte[] rgbValues = new byte[bytes];
            Marshal.Copy(ptr, rgbValues, 0, bytes);
            int counter = 0;
            for (int y = 0; y < H; y++)
            {
                for (int x = 0; x < W; x++, counter += 3)
                {
                    rgbValues[counter] = rgbValues[counter + 1] = rgbValues[counter + 2] =
                        (byte)((int)(Math.Min(Sample((float)x / W, (float)y / H) * 255.0f, 255.0f)));
                }
            }
            Marshal.Copy(rgbValues, 0, ptr, bytes);
            bmp.UnlockBits(bmpData);
            Graphics g = CreateGraphics();
            g.DrawImage(bmp, 0, 0);
        }

        private void StratifiedSampling_Click(object sender, EventArgs e)
        {
            MenuID = 62;
            Bitmap bmp = new Bitmap(W, H, PixelFormat.Format24bppRgb);
            Rectangle rect = new Rectangle(0, 0, bmp.Width, bmp.Height);
            BitmapData bmpData =
                bmp.LockBits(rect, ImageLockMode.ReadWrite,
                bmp.PixelFormat);
            IntPtr ptr = bmpData.Scan0;
            int bytes = bmpData.Stride * bmp.Height;
            byte[] rgbValues = new byte[bytes];
            Marshal.Copy(ptr, rgbValues, 0, bytes);
            int counter = 0;
            for (int y = 0; y < H; y++)
            {
                for (int x = 0; x < W; x++, counter += 3)
                {
                    rgbValues[counter] = rgbValues[counter + 1] = rgbValues[counter + 2] =
                        (byte)((int)(Math.Min(Sample((float)x / W, (float)y / H) * 255.0f, 255.0f)));
                }
            }
            Marshal.Copy(rgbValues, 0, ptr, bytes);
            bmp.UnlockBits(bmpData);
            Graphics g = CreateGraphics();
            g.DrawImage(bmp, W, 0);
        }

        private void JitteredSampling_Click(object sender, EventArgs e)
        {
            MenuID = 63;
            Bitmap bmp = new Bitmap(W, H, PixelFormat.Format24bppRgb);
            Rectangle rect = new Rectangle(0, 0, bmp.Width, bmp.Height);
            BitmapData bmpData =
                bmp.LockBits(rect, ImageLockMode.ReadWrite,
                bmp.PixelFormat);
            IntPtr ptr = bmpData.Scan0;
            int bytes = bmpData.Stride * bmp.Height;
            byte[] rgbValues = new byte[bytes];
            Marshal.Copy(ptr, rgbValues, 0, bytes);
            int counter = 0;
            for (int y = 0; y < H; y++)
            {
                for (int x = 0; x < W; x++, counter += 3)
                {
                    rgbValues[counter] = rgbValues[counter + 1] = rgbValues[counter + 2] =
                        (byte)((int)(Math.Min(Sample((float)x / W, (float)y / H) * 255.0f, 255.0f)));
                }
            }
            Marshal.Copy(rgbValues, 0, ptr, bytes);
            bmp.UnlockBits(bmpData);
            Graphics g = CreateGraphics();
            g.DrawImage(bmp, 2 * W, 0);
        }

        private float CircleSDF(float x, float y, float cx, float cy, float r)
        {
            float ux = x - cx, uy = y - cy;
            return (float)(Math.Sqrt(ux * ux + uy * uy)) - r;
        }

        private float BoxSDF(float x, float y, float cx, float cy, float theta, float sx, float sy)
        {
            float costheta = (float)Math.Cos(theta), sintheta = (float)Math.Sin(theta);
            float dx = Math.Abs((x - cx) * costheta + (y - cy) * sintheta) - sx;
            float dy = Math.Abs((y - cy) * costheta - (x - cx) * sintheta) - sy;
            float ax = Math.Max(dx, 0.0f), ay = Math.Max(dy, 0.0f);
            return (float)(Math.Min(Math.Max(dx, dy), 0.0f) + Math.Sqrt(ax * ax + ay * ay));
        }

        private float PlaneSDF(float x, float y, float px, float py, float nx, float ny)
        {
            return (x - px) * nx + (y - py) * ny;
        }

        private float Trace(float ox, float oy, float dx, float dy)
        {
            if (MenuID == 61 || MenuID == 62 || MenuID == 63)
            {
                float t = 0.0f;
                for (int i = 0; i < MAX_STEP && t < MAX_DISTANCE; i++)
                {
                    float sd = CircleSDF(ox + dx * t, oy + dy * t, 0.5f, 0.5f, 0.1f);
                    if (sd < EPSILON)
                        return 2.0f;
                    t += sd;
                }
                return 0.0f;
            }
            else
            {
                float t = 0.001f;
                for (int i = 0; i < MAX_STEP && t < MAX_DISTANCE; i++)
                {
                    Result r = Scene(ox + dx * t, oy + dy * t);
                    if (r.Sd < EPSILON)
                        return r.Emissive;
                    t += r.Sd;
                }
                return 0.0f;
            }
        }

        private float Trace(float ox, float oy, float dx, float dy, int depth)
        {
            float t = 0.0f;
            for (int i = 0; i < MAX_STEP && t < MAX_DISTANCE; i++)
            {
                float x = ox + dx * t, y = oy + dy * t;
                Result r = Scene(x, y);
                if (r.Sd < EPSILON)
                {
                    float sum = r.Emissive;
                    if (depth < MAX_DEPTH && r.Reflectivity > 0.0f)
                    {
                        float nx = 0.0f, ny = 0.0f, rx = 0.0f, ry = 0.0f;
                        Gradient(x, y, ref nx, ref ny);
                        Reflect(dx, dy, nx, ny,ref rx, ref ry);
                        sum += r.Reflectivity * Trace(x + nx * BIAS, y + ny * BIAS, rx, ry, depth + 1);
                    }
                    return sum;
                }
                t += r.Sd;
            }
            return 0.0f;
        }

        private void Gradient(float x, float y, ref float nx, ref float ny)
        {
            nx = (Scene(x + EPSILON, y).Sd - Scene(x - EPSILON, y).Sd) * (0.5f / EPSILON);
            ny = (Scene(x, y + EPSILON).Sd - Scene(x, y - EPSILON).Sd) * (0.5f / EPSILON);
        }

        private void Reflect(float ix, float iy, float nx, float ny, ref float rx, ref float ry)
        {
            float idotn2 = (ix * nx + iy * ny) * 2.0f;
            rx = ix - idotn2 * nx;
            ry = iy - idotn2 * ny;
        }

        private float Sample(float x, float y)
        {
            float sum = 0.0f;
            for (int i = 0; i < N; i++)
            {
                float a = 0.0f;
                if (MenuID == 61) a = 2 * (float)PI * rd.Next();
                else if (MenuID == 62) a = 2 * (float)PI * i / N;
                else if (MenuID == 63 || MenuID == 64 || MenuID == 65) a = 2 * (float)PI * (i + (float)rd.Next()) / N;
                if (MenuID == 61 || MenuID == 62 || MenuID == 63 || MenuID == 64) sum += Trace(x, y, (float)Math.Cos(a), (float)Math.Sin(a));
                else if (MenuID == 65) sum += Trace(x, y, (float)Math.Cos(a), (float)Math.Sin(a), 0);
            }
            return sum / N;
        }

        private void ConstructiveSolidGeometry_Click(object sender, EventArgs e)
        {
            MenuID = 64;
            Bitmap bmp = new Bitmap(W, H, PixelFormat.Format24bppRgb);
            Rectangle rect = new Rectangle(0, 0, bmp.Width, bmp.Height);
            BitmapData bmpData =
                bmp.LockBits(rect, ImageLockMode.ReadWrite,
                bmp.PixelFormat);
            IntPtr ptr = bmpData.Scan0;
            int bytes = bmpData.Stride * bmp.Height;
            byte[] rgbValues = new byte[bytes];
            Marshal.Copy(ptr, rgbValues, 0, bytes);
            int counter = 0;
            for (int y = 0; y < H; y++)
            {
                for (int x = 0; x < W; x++, counter += 3)
                {
                    rgbValues[counter] = rgbValues[counter + 1] = rgbValues[counter + 2] =
                        (byte)((int)(Math.Min(Sample((float)x / W, (float)y / H) * 255.0f, 255.0f)));
                }
            }
            Marshal.Copy(rgbValues, 0, ptr, bytes);
            bmp.UnlockBits(bmpData);
            Graphics g = CreateGraphics();
            g.DrawImage(bmp, 0, 0);
        }

        private Result UnionOp(Result a, Result b)
        {
            return a.Sd < b.Sd ? a : b;
        }

        private Result IntersectOp(Result a, Result b)
        {
            Result r = a.Sd > b.Sd ? b : a;
            r.Sd = a.Sd > b.Sd ? a.Sd : b.Sd;
            return r;
        }

        private Result SubtractOp(Result a, Result b)
        {
            Result r = a;
            r.Sd = (a.Sd > -b.Sd) ? a.Sd : -b.Sd;
            return r;
        }

        private Result ComplementOp(Result a)
        {
            a.Sd = -a.Sd;
            return a;
        }

        Result Scene(float x, float y)
        {
            if (MenuID == 64)
            {
                Result r1 = new Result(CircleSDF(x, y, 0.3f, 0.3f, 0.10f), 2.0f);
                Result r2 = new Result(CircleSDF(x, y, 0.3f, 0.7f, 0.05f), 0.8f);
                Result r3 = new Result(CircleSDF(x, y, 0.7f, 0.5f, 0.10f), 0.0f);
                return UnionOp(UnionOp(r1, r2), r3);
                //Result a = new Result(CircleSDF(x, y, 0.4f, 0.5f, 0.20f), 1.0f);
                //Result b = new Result(CircleSDF(x, y, 0.6f, 0.5f, 0.20f), 0.8f);
                //return UnionOp(a, b);
                //return IntersectOp(a, b);
                //return SubtractOp(a, b);
                //return SubtractOp(b, a);
            }
            else
            {
                Result a = new Result(CircleSDF(x, y, 0.4f, 0.2f, 0.1f), 2.0f, 0.0f);
                Result b = new Result(BoxSDF(x, y, 0.5f, 0.8f, 2 * (float)PI / 16.0f, 0.1f, 0.1f), 0.0f, 0.9f);
                Result c = new Result(BoxSDF(x, y, 0.8f, 0.5f, 2 * (float)PI / 16.0f, 0.1f, 0.1f), 0.0f, 0.9f);
                Result d = new Result(PlaneSDF(x, y, 0.0f, 0.5f, 0.0f, -1.0f), 0.0f, 0.9f);
                Result e = new Result(CircleSDF(x, y, 0.5f, 0.5f, 0.4f), 0.0f, 0.9f);
                //return UnionOp(UnionOp(a, b), c);
                return UnionOp(a, SubtractOp(d, e));
            }
        }

        private void Reflection_Click(object sender, EventArgs e)
        {
            MenuID = 65;
            Bitmap bmp = new Bitmap(W, H, PixelFormat.Format24bppRgb);
            Rectangle rect = new Rectangle(0, 0, bmp.Width, bmp.Height);
            BitmapData bmpData =
                bmp.LockBits(rect, ImageLockMode.ReadWrite,
                bmp.PixelFormat);
            IntPtr ptr = bmpData.Scan0;
            int bytes = bmpData.Stride * bmp.Height;
            byte[] rgbValues = new byte[bytes];
            Marshal.Copy(ptr, rgbValues, 0, bytes);
            int counter = 0;
            for (int y = 0; y < H; y++)
            {
                for (int x = 0; x < W; x++, counter += 3)
                {
                    rgbValues[counter] = rgbValues[counter + 1] = rgbValues[counter + 2] =
                        (byte)((int)(Math.Min(Sample((float)x / W, (float)y / H) * 255.0f, 255.0f)));
                }
            }
            Marshal.Copy(rgbValues, 0, ptr, bytes);
            bmp.UnlockBits(bmpData);
            Graphics g = CreateGraphics();
            g.DrawImage(bmp, 2 * W, 0);
        }
    }
}
