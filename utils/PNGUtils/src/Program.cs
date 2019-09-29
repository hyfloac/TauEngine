using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Security.Cryptography;
using System.Text;
using System.Windows.Forms;

namespace PNGUtils
{
    static class PasswordHasher
    {
        public static byte[] SHA512Hash(byte[] bytes)
        {
            using(SHA512 sha = new SHA512Managed())
            {
                return sha.ComputeHash(bytes);
            }
        }

        public static byte[] SHA512Hash(string msg)
        {
            return SHA512Hash(Encoding.UTF8.GetBytes(msg));
        }

        public static byte[] Combine(byte[] first, byte[] second)
        {
            byte[] ret = new byte[first.Length + second.Length];
            Buffer.BlockCopy(first, 0, ret, 0, first.Length);
            Buffer.BlockCopy(second, 0, ret, first.Length, second.Length);
            return ret;
        }

        private static byte[] F(byte[] password, byte[] salt, ulong iterCount, uint i)
        {
            byte[] u = SHA512Hash(Combine(Combine(password, salt), BitConverter.GetBytes(i)));

            for(ulong j = 0; j < iterCount; ++j)
            {
                byte[] uj = SHA512Hash(password + Convert.ToBase64String(u));
                for(int k = 0; k < u.Length && k < uj.Length; ++k)
                {
                    u[k] ^= uj[k];
                }
            }

            return u;
        }

        public static byte[] PBKDF2(byte[] password, byte[] salt, ulong iterCount, uint dkLen)
        {
            if(dkLen % 512 != 0) return null;
            uint tCount = dkLen / 512;
            byte[] dk = new byte[dkLen / 8];

            for(uint i = 0; i < tCount; ++i)
            {
                byte[] t = F(password, salt, iterCount, i);
                for(uint j = 0; j < t.Length; ++j)
                {
                    dk[i * 64 + j] = t[j];
                }
            }

            return dk;
        }

        private static uint RotL(uint a, int b)
        {
            return (a << b) | (a >> (32 - b));
        }
        
        private static void QR(ref uint a, ref uint b, ref uint c, ref uint d)
        {
            b ^= RotL(a + d, 7);
            c ^= RotL(b + a, 9);
            d ^= RotL(c + b, 13);
            a ^= RotL(d + c, 18);
        }

        private static void Salsa20(uint[] x, uint iter = 8)
        {
            for(int i = 0; i < iter; i += 2)
            {
                // Odd round
                QR(ref x[0],  ref x[4],  ref x[8],  ref x[12]); // column 1
                QR(ref x[5],  ref x[9],  ref x[13], ref x[1]); // column 2
                QR(ref x[10], ref x[14], ref x[2],  ref x[6]); // column 3
                QR(ref x[15], ref x[3],  ref x[7],  ref x[11]); // column 4
                // Even round                       
                QR(ref x[0],  ref x[1],  ref x[2],  ref x[3]); // row 1
                QR(ref x[5],  ref x[6],  ref x[7],  ref x[4]); // row 2
                QR(ref x[10], ref x[11], ref x[8],  ref x[9]); // row 3
                QR(ref x[15], ref x[12], ref x[13], ref x[14]); // row 4
            }
        }

        private static T[] SubArray<T>(this T[] data, int index, int length)
        {
            T[] result = new T[length];
            Array.Copy(data, index, result, 0, length);
            return result;
        }

        private static byte[] BlockMix(byte[] block)
        {
            int r = block.Length / 128;

            uint[] b = new uint[block.Length / 4];
            for(uint i = 0; i < b.Length; ++i)
            {
                b[i] = BitConverter.ToUInt32(block, (int) i * 4);
            }

            uint[] y = new uint[b.Length];

            uint[] x = b.SubArray((2 * r - 1) * 16, 16);
            for(uint i = 0; i < 2 * r; ++i)
            {
                uint[] bi = b.SubArray((int) i * 16, 16);
                for(uint j = 0; j < x.Length; ++j)
                {
                    x[j] ^= bi[j];
                }

                Salsa20(x);
                for(uint j = 0; j < x.Length; ++j)
                {
                    y[i * 16 + j] = x[j];
                }
            }

            uint[] ret = new uint[y.Length];

            for(uint i = 0; i < y.Length / 16; i += 2)
            {
                for(uint j = 0; j < x.Length; ++j)
                {
                    ret[i * 16 + j] = y[i * 16 + j];
                }
            }

            for(uint i = 0; i < y.Length / 16; i += 2)
            {
                for(uint j = 0; j < x.Length; ++j)
                {
                    ret[i * 16 + j + y.Length / 2] = y[i * 16 + j];
                }
            }

            byte[] result = new byte[ret.Length * sizeof(int)];
            Buffer.BlockCopy(ret, 0, result, 0, result.Length);

            return result;
        }

        private static void ROMix(byte[] block, uint iterations)
        {
            List<byte[]> byteList = new List<byte[]>((int) iterations);

            for(uint i = 0; i < iterations; ++i)
            {
                byteList.Add(block);
                block = BlockMix(block);
            }

            BigInteger bIter = new BigInteger(iterations);

            for(uint i = 0; i < iterations; ++i)
            {
                byte[] reverse = (byte[]) byteList[(int) i].Clone();
                Array.Reverse(reverse);
                reverse = Combine(reverse, new byte[] {0});
                BigInteger bi = new BigInteger(reverse);
                BigInteger bj = bi % bIter;
                byte[] b = byteList[(int) bj];
                for(uint k = 0; k < b.Length; ++k)
                {
                    block[k] ^= b[k];
                }

                block = BlockMix(block);
            }
        }

        public static byte[] sxcrypt(byte[] password, byte[] salt, byte costFactor, uint blockSizeFactor, uint parallelizationFactor, ulong saltIterCount, ulong iterCount, uint dkLen)
        {
            uint blockSize = blockSizeFactor * 128;

            byte[] b = PBKDF2(password, salt, saltIterCount, blockSize * parallelizationFactor);

            for(uint i = 0; i < parallelizationFactor; ++i)
            {
                byte[] sub = b.SubArray((int) (i * blockSize / 8), (int) blockSize / 8);
                ROMix(sub, (uint) 1 << costFactor);
                for(uint j = 0; j < sub.Length; ++j)
                {
                    b[i * blockSize / 8 + j] = sub[j];
                }
            }

            return PBKDF2(password, b, iterCount, dkLen);
        }
    }

    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Console.Write(">");
            string passcode = Console.ReadLine();
            byte[] hash = PasswordHasher.sxcrypt(Encoding.UTF8.GetBytes(passcode), Encoding.UTF8.GetBytes("489ysg5h4eyjgt48=f0i4eT*U#$T(PI"), 64, 8, 4, 1000000, 100000, 2048);

            Console.WriteLine(Convert.ToBase64String(hash));
            Console.ReadKey();

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            // Application.Run(new Form1());
        }
    }
}
