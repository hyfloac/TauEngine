using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace I18nEditor
{
    public static class DataCompressor
    {
        public static List<string> FindPatterns(sbyte patternCount, byte patternLen, byte lerp, byte lenLerp, params string[] strings)
        {
            Dictionary<string, int> patterns = new Dictionary<string, int>();
           
            foreach(string s in strings)
            {
                for(int i = 0; i < s.Length; i += lerp)
                {
                    for(int j = 4; j < Math.Min(s.Length - i + 1, patternLen); j += lenLerp)
                    {
                        string sub = s.Substring(i, j);
                        if(patterns.ContainsKey(sub))
                        {
                            patterns[sub] = patterns[sub] + 1;
                        }
                        else
                        {
                            patterns[sub] = 1;
                        }
                    }
                }
            }

            List<KeyValuePair<string, int>> patternsList = patterns.ToList();

            patternsList.RemoveAll((p) => p.Value <= 3);

            patternsList.Sort((pair0, pair1) =>
            {
                if(pair0.Value == 1 && pair1.Value == 1) { return 0; }
                if(pair0.Value == pair1.Value) { return -pair0.Key.Length.CompareTo(pair1.Key.Length); }

                if(pair0.Value == 1) return 1;
                if(pair1.Value == 1) return -1;

                if(pair0.Value > pair1.Value)
                {
                    // if(pair1.Value <= 3) { return 1; }
                    int cntDif = pair0.Value - pair1.Value;
                    int lenDif = pair0.Key.Length - pair1.Key.Length;
                    if(lenDif < 0 && cntDif < Math.Abs(lenDif)) { return 1; }
                    return -1;
                }

                // if(pair0.Value <= 3) { return -1; }
                int cntDif0 = pair1.Value - pair0.Value;
                int lenDif0 = pair1.Key.Length - pair0.Key.Length;
                if(lenDif0 < 0 && cntDif0 < Math.Abs(lenDif0))
                {
                    return -1;
                }
                return 1;
            });

            List<string> ret = new List<string>();

            for(int i = 0; i < Math.Min(patternsList.Count, patternCount); ++i)
            {
                ret.Add(patternsList[i].Key);
            }

            ret.Sort((s0, s1) => -s0.Length.CompareTo(s1.Length));

            return ret;
        }

        public static byte[] Compress(params string[] strings)
        {
            List<byte> bytes = new List<byte>();
            List<string> patterns = FindPatterns(32, 16, 6, 4, strings);
            
            bytes.Add((byte) patterns.Count);
            
            foreach(string pattern in patterns)
            {
                byte[] strBytes = Encoding.UTF8.GetBytes(pattern);
                bytes.Add((byte) strBytes.Length);
                bytes.AddRange(strBytes);
            }

            foreach(string s in strings)
            {
                string cpy = s;
                for(int i = 0; i < patterns.Count; ++i)
                {
                    string pattern = patterns[i];
                    int index = cpy.IndexOf(pattern, StringComparison.Ordinal);
                    if(index >= 0)
                    {
                        cpy = cpy.Remove(index, pattern.Length);
                        cpy = cpy.Insert(index, new String(new[] {(char) (i | 0x80)}));
                        --i;
                    }
                }

                byte[] stringEncoded = new byte[cpy.Length];
                for(int i = 0; i < cpy.Length; ++i)
                {
                    stringEncoded[i] = (byte) (cpy[i] & 0xFF);
                }
                bytes.Add((byte) stringEncoded.Length);
                bytes.Add((byte) ((stringEncoded.Length & 0xFF00) >> 8));
                bytes.AddRange(stringEncoded);
            }

            return bytes.ToArray();
        }

        public static string[] Decompress(byte[] bytes)
        {
            List<string> patterns = new List<string>();

            uint gIndex = 0;

            byte patternCount = bytes[gIndex++];

            StringBuilder sb = new StringBuilder();

            for(int i = 0; i < patternCount; ++i)
            {
                sb.Length = 0;
                byte strLen = bytes[gIndex++];
                for(int j = 0; j < strLen; ++j)
                {
                    sb.Append((char) bytes[gIndex++]);
                }
                patterns.Add(sb.ToString());
            }

            List<string> strings = new List<string>();
            
            while(gIndex < bytes.Length)
            {
                ushort cnt = bytes[gIndex++];
                cnt |= (ushort) (bytes[gIndex++] << 8);

                sb.Length = 0;
                for(int i = 0; i < cnt; ++i)
                {
                    if((bytes[gIndex] & 0x80) != 0)
                    {
                        sb.Append(patterns[bytes[gIndex] & 0x7F]);
                    }
                    else
                    {
                        sb.Append((char) bytes[gIndex]);
                    }

                    ++gIndex;
                }
                strings.Add(sb.ToString());
            }

            return strings.ToArray();
        }
    }
}
